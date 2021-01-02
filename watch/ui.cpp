//
// Created by Carl Friess on 01/01/2021.
//

#include "ui.h"

#include "nrf_log.h"

#include "GC9A01.h"
#include "spi.h"
#include "phase-ui.h"
#include "font-quicksand-64.h"

#define NRF_LOG_UI_FRAME(frame) NRF_LOG_INFO("(%d, %d) %d %d", frame.origin.x, frame.origin.y, frame.width, frame.height)

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

phase::ui::View root({{0, 0}, 240, 240});

void ui_init(void) {

    root.setOpaque(false);

    // Add background image
    auto background_view = new phase::ui::ImageView(background,
                                                    root.getFrame());
    root.addChildView(background_view);

    phase::ui::Frame time_frame{};
    time_frame.origin.x = 20;
    time_frame.origin.y = 120 - font_quicksand_64.height / 2;
    time_frame.width = 200;
    time_frame.height = font_quicksand_64.height;
    auto time_view = new phase::ui::TextView(&font_quicksand_64, time_frame);
    time_view->setText("12:34");
    root.addChildView(time_view);

}

void ui_update(void) {

    // Get the screen area that needs to be updated
    phase::ui::Frame dirty = root.getDirtyFrame();
    if (dirty.width <= 0 || dirty.height <= 0) {
        return;
    }
    NRF_LOG_INFO("Updating UI region:");
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
