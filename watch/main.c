#include "app_scheduler.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "bluetooth.h"

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

    // Start advertising
    advertising_start(erase_bonds);

    // Main loop
    for (;;) {
        idle_state_handle();
    }

}
