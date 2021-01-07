//
// Created by Carl Friess on 07/01/2021.
//

#include "phase_peer_manager.h"

#include "ble_db_discovery.h"
#include "nrf_log.h"
#include "peer_manager_handler.h"

#include "phase_ble.h"
#include "phase_gatt.h"


#define SEC_PARAM_TIMEOUT           30                      /**< Time-out for pairing request or security request (in seconds). */
#define SEC_PARAM_BOND              1                       /**< Perform bonding. */
#define SEC_PARAM_MITM              0                       /**< Man In The Middle protection requirement. */
#define SEC_PARAM_LESC              0                       /**< LE Secure Connections not enabled. */
#define SEC_PARAM_KEYPRESS          0                       /**< Keypress notifications not enabled. */
#define SEC_PARAM_IO_CAPABILITIES   BLE_GAP_IO_CAPS_NONE    /**< I/O capabilities. */
#define SEC_PARAM_OOB               0                       /**< Out Of Band data availability. */
#define SEC_PARAM_MIN_KEY_SIZE      7                       /**< Minimum encryption key size. */
#define SEC_PARAM_MAX_KEY_SIZE      16                      /**< Maximum encryption key size. */


// Device reference handle to the current bonded central
static pm_peer_id_t cur_peer;

// List of whitelisted peers
pm_peer_id_t whitelist[BLE_GAP_WHITELIST_ADDR_MAX_COUNT];
uint32_t whitelist_len;


/**@brief Function for handling Peer Manager events.
 *
 * @param[in] p_evt  Peer Manager event.
 */
static void pm_evt_handler(pm_evt_t const *p_evt) {

    ret_code_t err_code;

    pm_handler_on_pm_evt(p_evt);
    pm_handler_flash_clean(p_evt);

    switch (p_evt->evt_id) {
        case PM_EVT_CONN_SEC_SUCCEEDED:
            cur_peer = p_evt->peer_id;

            // Discover peer's services.
            err_code = ble_db_discovery_start(get_discovery_db(),
                                              p_evt->conn_handle);
            APP_ERROR_CHECK(err_code);
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
                    whitelist[whitelist_len++] = cur_peer;

                    // The whitelist has been modified, update it in the Peer Manager.
                    err_code = pm_device_identities_list_set(whitelist,
                                                             whitelist_len);
                    if (err_code != NRF_ERROR_NOT_SUPPORTED) {
                        APP_ERROR_CHECK(err_code);
                    }

                    err_code = pm_whitelist_set(whitelist,
                                                whitelist_len);
                    APP_ERROR_CHECK(err_code);

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

    ret_code_t err_code;
    ble_gap_sec_params_t sec_param;

    err_code = pm_init();
    APP_ERROR_CHECK(err_code);

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

    err_code = pm_sec_params_set(&sec_param);
    APP_ERROR_CHECK(err_code);

    err_code = pm_register(pm_evt_handler);
    APP_ERROR_CHECK(err_code);

}


/**@brief Fetch the list of peer IDs from peer manager.
 *
 * @param[inout] p_peers   The buffer where to store the list of peer IDs.
 * @param[inout] p_size    In: The size of the @p p_peers buffer.
 *                         Out: The number of peers copied in the buffer.
 */
void peer_list_get(pm_peer_id_t *p_peers, uint32_t *p_size) {

    pm_peer_id_t peer_id;
    uint32_t n = (*p_size < BLE_GAP_WHITELIST_ADDR_MAX_COUNT) ?
                 *p_size : BLE_GAP_WHITELIST_ADDR_MAX_COUNT;

    for (*p_size = 0; (peer_id = pm_next_peer_id_get(PM_PEER_ID_INVALID)) !=
                      PM_PEER_ID_INVALID && *p_size < n; (*p_size)++) {
        p_peers[*p_size] = peer_id;
    }

}
