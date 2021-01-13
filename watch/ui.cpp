//
// Created by Carl Friess on 01/01/2021.
//

#include "ui.h"

#include <cstdio>

#include "app_timer.h"
#include "nrf_log.h"

#include "GC9A01.h"
#include "spi.h"
#include "phase-ui.h"
#include "phase-ui-watch.h"
#include "font-quicksand-64.h"
#include "font-opensans-12.h"
#include "font-opensans-bold-12.h"
#include "icons.h"

#define NRF_LOG_UI_FRAME(frame) NRF_LOG_DEBUG("(%d, %d) %d %d", frame.origin.x, frame.origin.y, frame.width, frame.height)

#define CHUNK_SIZE  8
#define IMG_BUF_AREA    (240 * CHUNK_SIZE)
#define IMG_BUF_SIZE    (IMG_BUF_AREA * 3)

extern const uint8_t background[];

static uint8_t buf1[IMG_BUF_SIZE];
static uint8_t buf2[IMG_BUF_SIZE];
static uint8_t *bufs[] = {buf1, buf2};

static inline void swap_bufs() {
    uint8_t *tmp = bufs[1];
    bufs[1] = bufs[0];
    bufs[0] = tmp;
}

APP_TIMER_DEF(notification_timer);

phase::ui::View root(phase::ui::watch::display_frame);

phase::ui::TextView *time_view;
phase::ui::TextView *date_view;

phase::ui::watch::NotificationView *notification_view = nullptr;

static void notification_timer_handler(void *ctx) {
    ui_remove_notification();
}

void ui_init(void) {

    app_timer_create(&notification_timer, APP_TIMER_MODE_SINGLE_SHOT,
                     notification_timer_handler);

    root.setOpaque(false);

    // Add background image
    auto background_view = new phase::ui::ImageView(background,
                                                    root.getFrame());
    root.addChildView(background_view);

    phase::ui::Frame time_frame{};
    time_frame.origin.x = 0;
    time_frame.origin.y = 120 - 8 - font_quicksand_64.height / 2;
    time_frame.width = 240;
    time_frame.height = font_quicksand_64.height;
    time_view = new phase::ui::TextView(&font_quicksand_64, time_frame);
    time_view->setText("00:00");
    time_view->setOpaque(false);
    root.addChildView(time_view);

    phase::ui::Frame date_frame{};
    date_frame.origin.x = 0;
    date_frame.origin.y = 120 + 36;
    date_frame.width = 240;
    date_frame.height = font_opensans_12.height;
    date_view = new phase::ui::TextView(&font_opensans_12, date_frame);
    date_view->setVisible(false);
    date_view->setOpaque(false);
    root.addChildView(date_view);

}

void ui_update(void) {

    // Get the screen area that needs to be updated
    phase::ui::Frame dirty = root.getDirtyFrame();
    if (dirty.width <= 0 || dirty.height <= 0) {
        return;
    }
    NRF_LOG_DEBUG("Updating UI region:");
    NRF_LOG_UI_FRAME(dirty);

    // Set the display memory write area
    GC9A01_set_frame({
        .start = {
                .X = static_cast<uint16_t>(dirty.origin.x),
                .Y = static_cast<uint16_t>(dirty.origin.y)
        },
        .end = {
                .X = static_cast<uint16_t>(dirty.origin.x + dirty.width - 1),
                .Y = static_cast<uint16_t>(dirty.origin.y + dirty.height - 1),
        },
    });

    // Setup rendering frame
    phase::ui::Frame frame = dirty;
    size_t dirty_area = dirty.area();
    size_t steps = (dirty_area + IMG_BUF_AREA - 1) / IMG_BUF_AREA;
    frame.height = dirty_area / steps / dirty.width;

    // Start writing to display memory
    GC9A01_start_write();

    do {

        // Render the next part of the screen
        root.render(bufs[0], frame);

        // Wait for the last SPI transfer to complete
        while (!spi_done());

        // Start transferring the rendered buffer
        spi_tx(bufs[0], frame.area() * 3);

        // Move rendering frame
        frame.origin.y += frame.height;
        frame = frame.overlap(dirty);

        // Swap image buffers
        swap_bufs();

    } while (frame.height > 0);

    // Wait for the last SPI transfer to complete
    while (!spi_done());

    // Finish writing to display memory
    GC9A01_finish_write();

    // Mark the view hierarchy as clean
    root.clean();

}

void ui_set_datetime(time_t time) {
    static const std::string weekdays[] = {
            "Sunday",
            "Monday",
            "Tuesday",
            "Wednesday",
            "Thursday",
            "Friday",
    };
    static const std::string months[] = {
            "January",
            "February",
            "March",
            "April",
            "May",
            "June",
            "July",
            "August",
            "September",
            "October",
            "November",
            "December",
    };
    struct tm *time_struct = localtime(&time);
    ui_set_time(time_struct->tm_hour, time_struct->tm_min);
    ui_set_date(weekdays[time_struct->tm_wday].c_str(), time_struct->tm_mday,
                months[time_struct->tm_mon].c_str());
}

void ui_set_time(uint8_t hr, uint8_t min) {
    char str[6];
    snprintf(str, sizeof(str), "%02d:%02d", hr, min);
    time_view->setText(str);
}

void ui_set_date(const char *weekday, uint8_t day, const char *month) {
    char str[24];
    snprintf(str, sizeof(str), "%s, %d %s", weekday, day, month);
    date_view->setVisible(true);
    date_view->setText(str);
}

void ui_add_notification(char *appid, char *title, char *msg) {

    const uint8_t *icon = nullptr;
    phase::ui::Frame frame = {{0, 110}, 240, 130};

    // Remove any existing notification
    ui_remove_notification();

    // Try to find the icon for the given app
    auto it = icons.find(appid);
    if (it != icons.end()) {
        icon = it->second;
        frame.origin.y -= 60;
        frame.height += 60;
    }

    // Create a new NotificationView and add it to the root view
    notification_view = new phase::ui::watch::NotificationView(
            title,
            &font_opensans_bold_12,
            msg,
            &font_opensans_12,
            icon,
            frame);
    root.addChildView(notification_view);

    // Start the timer to remove the notification automatically after 10 sec
    app_timer_start(notification_timer, APP_TIMER_TICKS(10000), nullptr);

}

void ui_remove_notification() {
    app_timer_stop(notification_timer);
    if (notification_view) {
        root.removeChildView(notification_view);
        delete notification_view;
    }
}
