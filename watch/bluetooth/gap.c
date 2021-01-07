//
// Created by Carl Friess on 07/01/2021.
//

#include "gap.h"

#include <string.h>

#include "app_error.h"
#include "app_util.h"
#include "ble_gap.h"
#include "sdk_config.h"


#define MIN_CONN_INTERVAL   MSEC_TO_UNITS(500, UNIT_1_25_MS)            /**< Minimum acceptable connection interval (0.5 seconds). */
#define MAX_CONN_INTERVAL   MSEC_TO_UNITS(1000, UNIT_1_25_MS)           /**< Maximum acceptable connection interval (1 second). */
#define SLAVE_LATENCY       0                                           /**< Slave latency. */
#define CONN_SUP_TIMEOUT    MSEC_TO_UNITS(4000, UNIT_10_MS)             /**< Connection supervisory time-out (4 seconds). */


/**@brief Function for the GAP initialization.
 *
 * @details This function sets up all the necessary GAP (Generic Access Profile)
 * parameters of the device including the device name, appearance, and the
 * preferred connection parameters.
 */
void gap_params_init(void) {
    ret_code_t err_code;
    ble_gap_conn_params_t gap_conn_params;
    ble_gap_conn_sec_mode_t sec_mode;

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *) DEVICE_NAME,
                                          sizeof(DEVICE_NAME));
    APP_ERROR_CHECK(err_code);

    err_code = sd_ble_gap_appearance_set(BLE_APPEARANCE_GENERIC_WATCH);
    APP_ERROR_CHECK(err_code);

    memset(&gap_conn_params, 0, sizeof(gap_conn_params));

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    APP_ERROR_CHECK(err_code);
}