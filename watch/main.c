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

#define CHUNK_SIZE  8

static uint8_t buf1[240 * CHUNK_SIZE * 3];
static uint8_t buf2[240 * CHUNK_SIZE * 3];
static uint8_t *bufs[] = {buf1, buf2};

static void swap_bufs(void) {
    uint8_t *tmp = bufs[1];
    bufs[1] = bufs[0];
    bufs[0] = tmp;
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

    // Show background image
    GC9A01_start_write();
    for (int x = 0; x < 240 + 1; x += CHUNK_SIZE) {
        if (x > 0) {
            spi_tx(bufs[0], sizeof(buf1));
        }
        if (x < 240) {
            memcpy(bufs[1], background + 240 * 3 * x, sizeof(buf1));
        }
        swap_bufs();
        while (!spi_done());
    }
    GC9A01_finish_write();

    // Start advertising
    advertising_start(erase_bonds);

    // Main loop
    for (;;) {
        idle_state_handle();
    }

}
