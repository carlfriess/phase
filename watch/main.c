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
#include "spi.h"
#include "ui.h"


// Scheduler configuration parameters
#define SCHED_MAX_EVENT_DATA_SIZE   APP_TIMER_SCHED_EVENT_DATA_SIZE    /**< Maximum size of scheduler events. Note that scheduler BLE stack events do not contain any data, as the events are being pulled from the stack in the event handler. */
#ifdef SVCALL_AS_NORMAL_FUNCTION
#define SCHED_QUEUE_SIZE            20                                  /**< Maximum number of events in the scheduler queue. More is needed in case of Serialization. */
#else
#define SCHED_QUEUE_SIZE            10                                  /**< Maximum number of events in the scheduler queue. */
#endif


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

int main(void) {

    bool erase_bonds;

    // Initialize logging
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    // Initialize app timer
    APP_ERROR_CHECK(app_timer_init());

    // Initialize event scheduler
    APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);

    // Initialize power management
    APP_ERROR_CHECK(nrf_pwr_mgmt_init());

    // Initialize BLE stuff
    buttons_leds_init(&erase_bonds);
    ble_stack_init();
    gap_params_init();
    gatt_init();
    db_discovery_init();
    advertising_init();
    peer_manager_init();
    services_init();
    conn_params_init();

    // Initialize display
    nrf_gpio_cfg_output(GC9A01_RES);
    nrf_gpio_cfg_output(GC9A01_DC);
    nrf_gpio_cfg_output(GC9A01_CS);
    spi_init(GC9A01_SCL, GC9A01_SDA);
    GC9A01_init();
    ui_init();
    ui_update();

    // Start advertising
    advertising_start(erase_bonds);

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
