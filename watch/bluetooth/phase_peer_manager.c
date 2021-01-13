//
// Created by Carl Friess on 07/01/2021.
//

#include "phase_peer_manager.h"

#include <assert.h>
#include <stdbool.h>

#include "ble_db_discovery.h"
#include "fds.h"
#include "nrf_ble_gatts_c.h"
#include "nrf_log.h"
#include "peer_manager_handler.h"

#include "phase_ancs.h"
#include "phase_ble.h"
#include "phase_cts.h"
#include "phase_gatt.h"
#include "phase_gatts.h"


#define SEC_PARAM_BOND              1                       /**< Perform bonding. */
#define SEC_PARAM_MITM              0                       /**< Man In The Middle protection requirement. */
#define SEC_PARAM_LESC              0                       /**< LE Secure Connections not enabled. */
#define SEC_PARAM_KEYPRESS          0                       /**< Keypress notifications not enabled. */
#define SEC_PARAM_IO_CAPABILITIES   BLE_GAP_IO_CAPS_NONE    /**< I/O capabilities. */
#define SEC_PARAM_OOB               0                       /**< Out Of Band data availability. */
#define SEC_PARAM_MIN_KEY_SIZE      7                       /**< Minimum encryption key size. */
#define SEC_PARAM_MAX_KEY_SIZE      16                      /**< Maximum encryption key size. */

#define NUM_PEER_SERVICES           3

// List of whitelisted peers
pm_peer_id_t whitelist[BLE_GAP_WHITELIST_ADDR_MAX_COUNT];
uint32_t whitelist_len;

// Peer services database
// The size of peer_services must to be a multiple of 4 bytes due to internals
// of the flash storage library.
static ble_gatt_db_srv_t peer_services[4] = {0};
static ble_gatt_db_srv_t *peer_service_cts = &peer_services[0];
static ble_gatt_db_srv_t *peer_service_gatts = &peer_services[1];
static ble_gatt_db_srv_t *peer_service_ancs = &peer_services[2];

// Number of undiscovered peer services
static int peer_services_undiscovered = NUM_PEER_SERVICES;


static void store_handles(uint16_t conn_handle) {

    ret_code_t err;
    pm_peer_id_t peer_id;

    assert(peer_services_undiscovered == 0);

    // Get the peer ID based on the connection handle
    err = pm_peer_id_get(conn_handle, &peer_id);
    APP_ERROR_CHECK(err);

    // Attempt to store the remote database to flash
    err = pm_peer_data_remote_db_store(peer_id, peer_services,
                                       sizeof(peer_services), NULL);
    if (err == NRF_ERROR_STORAGE_FULL) {
        err = fds_gc();
    }
    APP_ERROR_CHECK(err);

}


/**@brief Handler that should be called when the CTS service has been
 * discovered to eventually store the discovered handles in flash.
 */
void discovered_cts(uint16_t conn_handle, const ble_cts_c_handles_t *cts) {

    // Write discovered handles to database
    peer_service_cts->charateristics[0].characteristic.handle_value =
            cts->cts_handle;
    peer_service_cts->charateristics[0].cccd_handle = cts->cts_cccd_handle;

    // Check if all services have been discovered
    if (--peer_services_undiscovered == 0) {
        store_handles(conn_handle);
    }

}


/**@brief Handler that should be called when the GATTS service has been
 * discovered to eventually store the discovered handles in flash.
 */
void discovered_gatts(uint16_t conn_handle, const ble_gatt_db_char_t *gatts) {

    // Write discovered handle to database
    peer_service_gatts->charateristics[0] = *gatts;

    // Check if all services have been discovered
    if (--peer_services_undiscovered == 0) {
        store_handles(conn_handle);
    }

}


/**@brief Handler that should be called when the ANCS service has been
 * discovered to eventually store the discovered handles in flash.
 */
void discovered_ancs(uint16_t conn_handle, const ble_ancs_c_service_t *ancs) {

    // Write discovered handles to database
    peer_service_ancs->charateristics[0].characteristic =
            ancs->control_point_char;
    peer_service_ancs->charateristics[1].characteristic =
            ancs->notif_source_char;
    peer_service_ancs->charateristics[1].cccd_handle =
            ancs->notif_source_cccd.handle;
    peer_service_ancs->charateristics[2].characteristic =
            ancs->data_source_char;
    peer_service_ancs->charateristics[2].cccd_handle =
            ancs->data_source_cccd.handle;

    // Check if all services have been discovered
    if (--peer_services_undiscovered == 0) {
        store_handles(conn_handle);
    }

}


static bool load_handles(pm_peer_id_t peer_id, uint16_t conn_handle) {

    ret_code_t err;
    ble_cts_c_handles_t cts_handles;
    ble_ancs_c_service_t ancs_handles;

    // Attempt to load the remote database from flash
    uint32_t data_len = sizeof(peer_services);
    err = pm_peer_data_remote_db_load(peer_id, peer_services, &data_len);
    if (err == NRF_ERROR_NOT_FOUND) {
        return false;
    }
    APP_ERROR_CHECK(err);

    // Restore CTS client handles
    cts_handles.cts_handle =
            peer_service_cts->charateristics[0].characteristic.handle_value;
    cts_handles.cts_cccd_handle =
            peer_service_cts->charateristics[0].cccd_handle;
    err = ble_cts_c_handles_assign(get_cts(), conn_handle, &cts_handles);
    APP_ERROR_CHECK(err);

    // Restore GATTS client handles
    err = nrf_ble_gatts_c_handles_assign(get_gatts_c(), conn_handle,
                                         &peer_service_gatts->charateristics[0]);
    APP_ERROR_CHECK(err);

    // Restore ANCS client handles
    ancs_handles.control_point_char =
            peer_service_ancs->charateristics[0].characteristic;
    ancs_handles.notif_source_char =
            peer_service_ancs->charateristics[1].characteristic;
    ancs_handles.notif_source_cccd.handle =
            peer_service_ancs->charateristics[1].cccd_handle;
    ancs_handles.data_source_char =
            peer_service_ancs->charateristics[2].characteristic;
    ancs_handles.data_source_cccd.handle =
            peer_service_ancs->charateristics[2].cccd_handle;
    err = nrf_ble_ancs_c_handles_assign(get_ancs(), conn_handle, &ancs_handles);
    APP_ERROR_CHECK(err);

    return true;
}


/**@brief Function for handling Peer Manager events.
 *
 * @param[in] p_evt  Peer Manager event.
 */
static void pm_evt_handler(pm_evt_t const *p_evt) {

    ret_code_t err;

    pm_handler_on_pm_evt(p_evt);
    pm_handler_flash_clean(p_evt);

    switch (p_evt->evt_id) {
        case PM_EVT_BONDED_PEER_CONNECTED:
            if (p_evt->peer_id != PM_PEER_ID_INVALID) {
                break;
            }
            if (!load_handles(p_evt->peer_id, p_evt->conn_handle)) {
                err = nrf_ble_gatts_c_handles_assign(get_gatts_c(),
                                                     p_evt->conn_handle, NULL);
                APP_ERROR_CHECK(err);

                // Discover peer's services
                ble_db_discovery_t *discovery_db = get_discovery_db();
                peer_services_undiscovered = NUM_PEER_SERVICES;
                memset(discovery_db, 0, sizeof(ble_db_discovery_t));
                err = ble_db_discovery_start(discovery_db, p_evt->conn_handle);
                APP_ERROR_CHECK(err);

            }


        case PM_EVT_CONN_SEC_SUCCEEDED:
            if (get_gatts_c()->srv_changed_char.characteristic.handle_value ==
                BLE_GATT_HANDLE_INVALID) {
                err = nrf_ble_gatts_c_handles_assign(get_gatts_c(),
                                                     p_evt->conn_handle, NULL);
                APP_ERROR_CHECK(err);

                // Discover peer's services
                ble_db_discovery_t *discovery_db = get_discovery_db();
                peer_services_undiscovered = NUM_PEER_SERVICES;
                memset(discovery_db, 0, sizeof(ble_db_discovery_t));
                err = ble_db_discovery_start(discovery_db, p_evt->conn_handle);
                APP_ERROR_CHECK(err);

            }
            break;

        case PM_EVT_PEERS_DELETE_SUCCEEDED:
            advertising_start(false);
            break;

        case PM_EVT_PEER_DATA_UPDATE_SUCCEEDED:
            // Note: You should check on what kind of white list policy your application should use.
            if (p_evt->params.peer_data_update_succeeded.flash_changed &&
                (p_evt->params.peer_data_update_succeeded.data_id ==
                 PM_PEER_DATA_ID_BONDING)) {

                NRF_LOG_DEBUG(
                        "New Bond, add the peer to the whitelist if possible");
                NRF_LOG_DEBUG("\tm_whitelist_peer_cnt %d, MAX_PEERS_WLIST %d",
                              whitelist_len + 1,
                              BLE_GAP_WHITELIST_ADDR_MAX_COUNT);

                if (whitelist_len < BLE_GAP_WHITELIST_ADDR_MAX_COUNT) {

                    // Bonded to a new peer, add it to the whitelist.
                    whitelist[whitelist_len++] = p_evt->peer_id;

                    // Update the whitelist in the Peer Manager.
                    err = pm_device_identities_list_set(whitelist,
                                                        whitelist_len);
                    if (err != NRF_ERROR_NOT_SUPPORTED) {
                        APP_ERROR_CHECK(err);
                    }

                    err = pm_whitelist_set(whitelist, whitelist_len);
                    APP_ERROR_CHECK(err);

                }

            }
            break;

        default:
            break;
    }

}


/**@brief Function for the Peer Manager initialization.
 */
void peer_manager_init(void) {

    ret_code_t err;
    ble_gap_sec_params_t sec_param;

    err = pm_init();
    APP_ERROR_CHECK(err);

    memset(&sec_param, 0, sizeof(ble_gap_sec_params_t));

    // Security parameters to be used for all security procedures.
    sec_param.bond = SEC_PARAM_BOND;
    sec_param.mitm = SEC_PARAM_MITM;
    sec_param.lesc = SEC_PARAM_LESC;
    sec_param.keypress = SEC_PARAM_KEYPRESS;
    sec_param.io_caps = SEC_PARAM_IO_CAPABILITIES;
    sec_param.oob = SEC_PARAM_OOB;
    sec_param.min_key_size = SEC_PARAM_MIN_KEY_SIZE;
    sec_param.max_key_size = SEC_PARAM_MAX_KEY_SIZE;
    sec_param.kdist_own.enc = 1;
    sec_param.kdist_own.id = 1;
    sec_param.kdist_peer.enc = 1;
    sec_param.kdist_peer.id = 1;

    err = pm_sec_params_set(&sec_param);
    APP_ERROR_CHECK(err);

    err = pm_register(pm_evt_handler);
    APP_ERROR_CHECK(err);

}


/**@brief Fetch the list of peer IDs from peer manager.
 *
 * @param[inout] p_peers   The buffer where to store the list of peer IDs.
 * @param[inout] p_size    In: The size of the @p p_peers buffer.
 *                         Out: The number of peers copied in the buffer.
 */
void peer_list_get(pm_peer_id_t *p_peers, uint32_t *p_size) {

    pm_peer_id_t peer_id = PM_PEER_ID_INVALID;
    uint32_t n = (*p_size < BLE_GAP_WHITELIST_ADDR_MAX_COUNT) ?
                 *p_size : BLE_GAP_WHITELIST_ADDR_MAX_COUNT;

    for (*p_size = 0;
         (peer_id = pm_next_peer_id_get(peer_id)) != PM_PEER_ID_INVALID &&
         *p_size < n; (*p_size)++) {
        p_peers[*p_size] = peer_id;
    }

}
