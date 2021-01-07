//
// Created by Carl Friess on 07/01/2021.
//

#include "phase_cts.h"

#include "app_error.h"
#include "ble_cts_c.h"
#include "nrf_log.h"

#include "phase_gatt.h"
#include "../ui.h"


static char const *day_of_week[] = {
        "Unknown",
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday",
        "Sunday"
};

static char const *month_of_year[] = {
        "Unknown",
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
        "December"
};

// Current Time Service client instance
BLE_CTS_C_DEF(cts);


/**@brief Returns the Current Time Service client instance.
 */
ble_cts_c_t *get_cts(void) {
    return &cts;
}


/**@brief Function for handling the Current Time Service errors.
 *
 * @param[in] p_evt  Event received from the Current Time Service client.
 */
static void current_time_print(ble_cts_c_evt_t *p_evt) {
    NRF_LOG_INFO("\r\nCurrent Time:");
    NRF_LOG_INFO("\r\nDate:");

    NRF_LOG_INFO("\tDay of week   %s", (uint32_t) day_of_week[p_evt->
            params.
            current_time.
            exact_time_256.
            day_date_time.
            day_of_week]);

    if (p_evt->params.current_time.exact_time_256.day_date_time.date_time.day ==
        0) {
        NRF_LOG_INFO("\tDay of month  Unknown");
    } else {
        NRF_LOG_INFO("\tDay of month  %i",
                     p_evt->params.current_time.exact_time_256.day_date_time.date_time.day);
    }

    NRF_LOG_INFO("\tMonth of year %s",
                 (uint32_t) month_of_year[p_evt->params.current_time.exact_time_256.day_date_time.date_time.month]);
    if (p_evt->params.current_time.exact_time_256.day_date_time.date_time.year ==
        0) {
        NRF_LOG_INFO("\tYear          Unknown");
    } else {
        NRF_LOG_INFO("\tYear          %i",
                     p_evt->params.current_time.exact_time_256.day_date_time.date_time.year);
    }
    NRF_LOG_INFO("\r\nTime:");
    NRF_LOG_INFO("\tHours     %i",
                 p_evt->params.current_time.exact_time_256.day_date_time.date_time.hours);
    NRF_LOG_INFO("\tMinutes   %i",
                 p_evt->params.current_time.exact_time_256.day_date_time.date_time.minutes);
    NRF_LOG_INFO("\tSeconds   %i",
                 p_evt->params.current_time.exact_time_256.day_date_time.date_time.seconds);
    NRF_LOG_INFO("\tFractions %i/256 of a second",
                 p_evt->params.current_time.exact_time_256.fractions256);

    NRF_LOG_INFO("\r\nAdjust reason:\r");
    NRF_LOG_INFO("\tDaylight savings %x",
                 p_evt->params.current_time.adjust_reason.change_of_daylight_savings_time);
    NRF_LOG_INFO("\tTime zone        %x",
                 p_evt->params.current_time.adjust_reason.change_of_time_zone);
    NRF_LOG_INFO("\tExternal update  %x",
                 p_evt->params.current_time.adjust_reason.external_reference_time_update);
    NRF_LOG_INFO("\tManual update    %x",
                 p_evt->params.current_time.adjust_reason.manual_time_update);

    ui_set_time(
            p_evt->params.current_time.exact_time_256.day_date_time.date_time.hours,
            p_evt->params.current_time.exact_time_256.day_date_time.date_time.minutes);
    ui_set_date(
            day_of_week[p_evt->params.current_time.exact_time_256.day_date_time.day_of_week],
            p_evt->params.current_time.exact_time_256.day_date_time.date_time.day,
            month_of_year[p_evt->params.current_time.exact_time_256.day_date_time.date_time.month]);
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
            current_time_print(p_evt);
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
