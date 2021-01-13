#include "app_scheduler.h"
#include "app_timer.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "sdk_config.h"

#include "GC9A01.h"

#include "bluetooth.h"
#include "datetime.h"
#include "spi.h"
#include "ui.h"


// Scheduler configuration parameters
#define SCHED_MAX_EVENT_DATA_SIZE   APP_TIMER_SCHED_EVENT_DATA_SIZE    /**< Maximum size of scheduler events. Note that scheduler BLE stack events do not contain any data, as the events are being pulled from the stack in the event handler. */
#ifdef SVCALL_AS_NORMAL_FUNCTION
#define SCHED_QUEUE_SIZE            20                                  /**< Maximum number of events in the scheduler queue. More is needed in case of Serialization. */
#else
#define SCHED_QUEUE_SIZE            10                                  /**< Maximum number of events in the scheduler queue. */
#endif


APP_TIMER_DEF(time_sync_timer);


void GC9A01_set_reset(uint8_t val) {
    nrf_gpio_pin_write(GC9A01_RES, val);
}

void GC9A01_set_data_command(uint8_t val) {
    nrf_gpio_pin_write(GC9A01_DC, val);
}

void GC9A01_set_chip_select(uint8_t val) {
    nrf_gpio_pin_write(GC9A01_CS, val);
}

void GC9A01_spi_tx(uint8_t *data, size_t len) {
    spi_tx(data, len);
    while (!spi_done());
}

void GC9A01_delay(uint16_t ms) {
    nrf_delay_ms(ms);
}

void bluetooth_time_handler(time_t time) {
    datetime_set(time);
    ui_set_datetime(time);
}

void bluetooth_notification_add(char *appid, char *title, char *msg) {
    ui_add_notification(appid, title, msg);
}

static void time_sync_handler(void *ctx) {
    bluetooth_request_time();
}

int main(void) {

    ret_code_t err;

    // Initialize logging
    err = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err);
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    // Initialize app timer
    err = app_timer_init();
    APP_ERROR_CHECK(err);

    // Initialize event scheduler
    APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);

    // Initialize power management
    err = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err);

    // Initialize Bluetooth stack and services
    bluetooth_init();

    // Use an app timer to synchronize time hourly
    err = app_timer_create(&time_sync_timer, APP_TIMER_MODE_REPEATED,
                           time_sync_handler);
    APP_ERROR_CHECK(err);
    err = app_timer_start(time_sync_timer, APP_TIMER_TICKS(60 * 60 * 1000),
                          NULL);
    APP_ERROR_CHECK(err);

    // Initialize display
    nrf_gpio_cfg_output(GC9A01_RES);
    nrf_gpio_cfg_output(GC9A01_DC);
    nrf_gpio_cfg_output(GC9A01_CS);
    spi_init(GC9A01_SCL, GC9A01_SDA);
    GC9A01_init();
    ui_init();
    ui_update();

    // Start advertising
    bluetooth_start_advertising(false);

    // Main loop
    while (true) {

        // Execute event scheduler
        app_sched_execute();

        // Update UI
        ui_update();

        // Process log messages
        if (!NRF_LOG_PROCESS()) {
            nrf_pwr_mgmt_run();
        }

    }

}
