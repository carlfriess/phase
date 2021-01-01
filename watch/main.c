#include "app_scheduler.h"
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

extern const uint8_t background[];

static void log_init(void) {
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}

static void power_management_init(void) {
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}

static void idle_state_handle(void) {
    app_sched_execute();
    if (NRF_LOG_PROCESS() == false) {
        nrf_pwr_mgmt_run();
    }
}

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

    // Initialize BLE stuff
    log_init();
    timers_init();
    buttons_leds_init(&erase_bonds);
    scheduler_init();
    power_management_init();
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
    for (;;) {
        idle_state_handle();
    }

}
