//
// Created by Carl Friess on 08/01/2021.
//

#include "phase_gatts.h"

#include "app_error.h"
#include "nrf_ble_gatts_c.h"
#include "nrf_log.h"

#include "phase_gatt.h"
#include "phase_peer_manager.h"


NRF_BLE_GATTS_C_DEF(gatts_c);


/**@brief Returns the GATT Service client instance.
 */
nrf_ble_gatts_c_t *get_gatts_c(void) {
    return &gatts_c;
}


/**@brief Function for handling events from the GATT Servive client module.

   @param[in] p_evt GATT Service event.
*/
static void gatts_c_evt_handler(nrf_ble_gatts_c_evt_t *p_evt) {
    ret_code_t err;

    switch (p_evt->evt_type) {
        case NRF_BLE_GATTS_C_EVT_DISCOVERY_COMPLETE:
            NRF_LOG_DEBUG("GATT Service and Service Changed characteristic "
                          "found on server.");
            err = nrf_ble_gatts_c_handles_assign(&gatts_c,
                                                 p_evt->conn_handle,
                                                 &p_evt->params
                                                         .srv_changed_char);
            APP_ERROR_CHECK(err);

            // Register discovered service with peer manager
            discovered_gatts(p_evt->conn_handle,
                             &p_evt->params.srv_changed_char);

            err = nrf_ble_gatts_c_enable_indication(&gatts_c, true);
            APP_ERROR_CHECK(err);
            break;

        case NRF_BLE_GATTS_C_EVT_DISCOVERY_FAILED:
            NRF_LOG_DEBUG("GATT Service or Service Changed characteristic not "
                          "found on server.");
            break;

        case NRF_BLE_GATTS_C_EVT_DISCONN_COMPLETE:
            NRF_LOG_DEBUG(
                    "GATTS Service client disconnected connection handle %i.",
                    p_evt->conn_handle);
            break;

        case NRF_BLE_GATTS_C_EVT_SRV_CHANGED:
            NRF_LOG_DEBUG("Service Changed indication received.");

            // Discover peer's services
            NRF_LOG_INFO("Rediscovering peer's services...");
            ble_db_discovery_t *discovery_db = get_discovery_db();
            memset(discovery_db, 0, sizeof(ble_db_discovery_t));
            err = ble_db_discovery_start(discovery_db, p_evt->conn_handle);
            APP_ERROR_CHECK(err);
            break;

        default:
            break;
    }
}


/**@brief Function for initializing GATT Service client.
 */
void gatts_c_init(void) {

    ret_code_t err;
    nrf_ble_gatts_c_init_t gatts_c_init;

    memset(&gatts_c_init, 0, sizeof(gatts_c_init));

    gatts_c_init.evt_handler = gatts_c_evt_handler;
    gatts_c_init.p_gatt_queue = get_gatt_queue();

    err = nrf_ble_gatts_c_init(&gatts_c, &gatts_c_init);
    APP_ERROR_CHECK(err);

}