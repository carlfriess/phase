//
// Created by Carl Friess on 07/01/2021.
//

#include "phase_cts.h"

#include "app_error.h"
#include "ble_cts_c.h"
#include "nrf_log.h"

#include "phase_bluetooth_handlers.h"
#include "phase_gatt.h"


// Current Time Service client instance
BLE_CTS_C_DEF(cts);


/**@brief Returns the Current Time Service client instance.
 */
ble_cts_c_t *get_cts(void) {
    return &cts;
}

static void parse_cts_time(exact_time_256_t time) {

    struct tm time_struct;
    time_struct.tm_year = time.day_date_time.date_time.year - 1900;
    time_struct.tm_mon = time.day_date_time.date_time.month - 1;
    time_struct.tm_mday = time.day_date_time.date_time.day;
    time_struct.tm_hour = time.day_date_time.date_time.hours;
    time_struct.tm_min = time.day_date_time.date_time.minutes;
    time_struct.tm_sec = time.day_date_time.date_time.seconds;

    time_t timestamp = mktime(&time_struct);
    if (timestamp >= 0) {
        bluetooth_time_handler(mktime(&time_struct));
    }

}


/**@brief Function for handling the Current Time Service client events.
 *
 * @details This function will be called for all events in the Current Time Service client that
 *          are passed to the application.
 *
 * @param[in] p_evt Event received from the Current Time Service client.
 */
static void on_cts_c_evt(ble_cts_c_t *p_cts, ble_cts_c_evt_t *p_evt) {
    ret_code_t err_code;

    switch (p_evt->evt_type) {
        case BLE_CTS_C_EVT_DISCOVERY_COMPLETE:
            NRF_LOG_INFO("Current Time Service discovered on server.");
            err_code = ble_cts_c_handles_assign(&cts, p_evt->conn_handle,
                                                &p_evt->params.char_handles);
            APP_ERROR_CHECK(err_code);

            // Automatically request the time from the server
            err_code = ble_cts_c_current_time_read(get_cts());
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_CTS_C_EVT_DISCOVERY_FAILED:
            NRF_LOG_INFO("Current Time Service not found on server. ");
            // CTS not found in this case we just disconnect. There is no reason to stay
            // in the connection for this simple app since it all wants is to interact with CT
            if (p_evt->conn_handle != BLE_CONN_HANDLE_INVALID) {
                err_code = sd_ble_gap_disconnect(p_evt->conn_handle,
                                                 BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
                APP_ERROR_CHECK(err_code);
            }
            break;

        case BLE_CTS_C_EVT_DISCONN_COMPLETE:
            NRF_LOG_INFO("Disconnect Complete.");
            break;

        case BLE_CTS_C_EVT_CURRENT_TIME:
            NRF_LOG_INFO("Current Time received.");
            parse_cts_time(p_evt->params.current_time.exact_time_256);
            break;

        case BLE_CTS_C_EVT_INVALID_TIME:
            NRF_LOG_INFO("Invalid Time received.");
            break;

        default:
            break;
    }
}


/**@brief Function for handling the Current Time Service errors.
 *
 * @param[in]  nrf_error  Error code containing information about the error.
 */
static void current_time_error_handler(uint32_t nrf_error) {
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for initializing Current Time Service client.
 */
void cts_init(void) {
    ret_code_t err_code;
    ble_cts_c_init_t cts_init = {0};

    // Initialize CTS.
    cts_init.evt_handler = on_cts_c_evt;
    cts_init.error_handler = current_time_error_handler;
    cts_init.p_gatt_queue = get_gatt_queue();
    err_code = ble_cts_c_init(&cts, &cts_init);
    APP_ERROR_CHECK(err_code);
}
