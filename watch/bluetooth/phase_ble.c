//
// Created by Carl Friess on 07/01/2021.
//

#include "phase_ble.h"

#include "app_error.h"
#include "ble.h"
#include "ble_advertising.h"
#include "ble_srv_common.h"
#include "nrf_ble_qwr.h"
#include "nrf_log.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "peer_manager_handler.h"

#include "phase_ancs.h"
#include "phase_cts.h"
#include "phase_peer_manager.h"


#define APP_BLE_CONN_CFG_TAG    1       /**< A tag identifying the SoftDevice BLE configuration. */
#define APP_BLE_OBSERVER_PRIO   3       /**< Application's BLE observer priority. You shouldn't need to modify this value. */

#define APP_ADV_FAST_INTERVAL   0x0028  /**< Fast advertising interval (in units of 0.625 ms). The default value corresponds to 25 ms. */
#define APP_ADV_SLOW_INTERVAL   0x0C80  /**< Slow advertising interval (in units of 0.625 ms). The default value corresponds to 2 seconds. */
#define APP_ADV_FAST_DURATION   3000    /**< The advertising duration of fast advertising in units of 10 milliseconds. */
#define APP_ADV_SLOW_DURATION   0       /**< The advertising duration of slow advertising in units of 10 milliseconds. */


// Handle of the current connection
uint16_t cur_conn = BLE_CONN_HANDLE_INVALID;

// Advertising module instance
BLE_ADVERTISING_DEF(m_advertising);

// Queued Write module context
NRF_BLE_QWR_DEF(ble_qwr);

// Solicited UUIDs for advertising
static ble_uuid_t adv_solicited_uuids[1];


/**@brief Function for handling BLE events.
 *
 * @param[in]   p_ble_evt   Bluetooth stack event.
 * @param[in]   p_context   Unused.
 */
static void ble_evt_handler(ble_evt_t const *p_ble_evt, void *p_context) {

    ret_code_t err_code;

    pm_handler_secure_on_connection(p_ble_evt);

    switch (p_ble_evt->header.evt_id) {
        case BLE_GAP_EVT_CONNECTED:
            NRF_LOG_INFO("Connected.");
            cur_conn = p_ble_evt->evt.gap_evt.conn_handle;
            err_code = nrf_ble_qwr_conn_handle_assign(&ble_qwr, cur_conn);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            NRF_LOG_INFO("Disconnected: %d",
                         p_ble_evt->evt.gap_evt.params.disconnected.reason);
            cur_conn = BLE_CONN_HANDLE_INVALID;
            if (p_ble_evt->evt.gap_evt.conn_handle == get_cts()->conn_handle) {
                get_cts()->conn_handle = BLE_CONN_HANDLE_INVALID;
            }
            if (p_ble_evt->evt.gap_evt.conn_handle == get_ancs()->conn_handle) {
                get_ancs()->conn_handle = BLE_CONN_HANDLE_INVALID;
            }
            break;

        case BLE_GAP_EVT_PHY_UPDATE_REQUEST: {
            NRF_LOG_DEBUG("PHY update request.");
            const ble_gap_phys_t phys = {
                    .rx_phys = BLE_GAP_PHY_AUTO,
                    .tx_phys = BLE_GAP_PHY_AUTO,
            };
            err_code = sd_ble_gap_phy_update(p_ble_evt->evt.gap_evt.conn_handle,
                                             &phys);
            APP_ERROR_CHECK(err_code);
            break;
        }

        case BLE_GATTC_EVT_TIMEOUT:
            // Disconnect on GATT Client timeout event.
            NRF_LOG_DEBUG("GATT Client Timeout.");
            err_code = sd_ble_gap_disconnect(
                    p_ble_evt->evt.gattc_evt.conn_handle,
                    BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_TIMEOUT:
            // Disconnect on GATT Server timeout event.
            NRF_LOG_DEBUG("GATT Server Timeout.");
            err_code = sd_ble_gap_disconnect(
                    p_ble_evt->evt.gatts_evt.conn_handle,
                    BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
            APP_ERROR_CHECK(err_code);
            break;

        default:
            // No implementation needed.
            break;
    }

}


/**@brief Function for handling Queued Write Module errors.
 *
 * @details A pointer to this function will be passed to each service which may
 * need to inform the application about an error.
 *
 * @param[in]   nrf_error   Error code containing information about the error.
 */
static void nrf_qwr_error_handler(uint32_t nrf_error) {
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt. Uses
 * parameters from sdk_config.h.
 */
void ble_stack_init(void) {

    ret_code_t err;
    nrf_ble_qwr_init_t qwr_init = {0};

    err = nrf_sdh_enable_request();
    APP_ERROR_CHECK(err);

    // Configure the BLE stack using the default settings.
    // Fetch the start address of the application RAM.
    uint32_t ram_start = 0;
    err = nrf_sdh_ble_default_cfg_set(APP_BLE_CONN_CFG_TAG, &ram_start);
    APP_ERROR_CHECK(err);

    // Enable BLE stack.
    err = nrf_sdh_ble_enable(&ram_start);
    APP_ERROR_CHECK(err);

    // Register a handler for BLE events.
    NRF_SDH_BLE_OBSERVER(ble_observer, APP_BLE_OBSERVER_PRIO, ble_evt_handler,
                         NULL);

    // Initialize Queued Write Module
    qwr_init.error_handler = nrf_qwr_error_handler;
    err = nrf_ble_qwr_init(&ble_qwr, &qwr_init);
    APP_ERROR_CHECK(err);

}


/**@brief Function for handling advertising events.
 *
 * @details This function will be called for advertising events which are passed
 * to the application.
 *
 * @param[in] ble_adv_evt  Advertising event.
 */
static void adv_evt_handler(ble_adv_evt_t ble_adv_evt) {

    ret_code_t err_code;

    switch (ble_adv_evt) {
        case BLE_ADV_EVT_FAST:
            NRF_LOG_INFO("Fast advertising");
            break;

        case BLE_ADV_EVT_SLOW:
            NRF_LOG_INFO("Slow advertising");
            break;

        case BLE_ADV_EVT_FAST_WHITELIST:
            NRF_LOG_INFO("Fast advertising with WhiteList");
            break;

        case BLE_ADV_EVT_SLOW_WHITELIST:
            NRF_LOG_INFO("Slow advertising with WhiteList");
            err_code = ble_advertising_restart_without_whitelist(
                    &m_advertising);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_ADV_EVT_IDLE:
            // sleep_mode_enter();
            break;

        case BLE_ADV_EVT_WHITELIST_REQUEST: {
            ble_gap_addr_t whitelist_addrs[BLE_GAP_WHITELIST_ADDR_MAX_COUNT];
            ble_gap_irk_t whitelist_irks[BLE_GAP_WHITELIST_ADDR_MAX_COUNT];
            uint32_t addr_cnt = BLE_GAP_WHITELIST_ADDR_MAX_COUNT;
            uint32_t irk_cnt = BLE_GAP_WHITELIST_ADDR_MAX_COUNT;

            err_code = pm_whitelist_get(whitelist_addrs, &addr_cnt,
                                        whitelist_irks, &irk_cnt);
            APP_ERROR_CHECK(err_code);
            NRF_LOG_DEBUG(
                    "pm_whitelist_get returns %d addr in whitelist and %d irk whitelist",
                    addr_cnt,
                    irk_cnt);

            // Apply the whitelist.
            err_code = ble_advertising_whitelist_reply(&m_advertising,
                                                       whitelist_addrs,
                                                       addr_cnt,
                                                       whitelist_irks,
                                                       irk_cnt);
            APP_ERROR_CHECK(err_code);
            break;
        }

        default:
            break;
    }

}


/**@brief Function for initializing the advertising functionality.
 *
 * @details Encodes the required advertising data and passes it to the stack.
 * Also builds a structure to be passed to the stack when starting advertising.
 */
void advertising_init(void) {

    ret_code_t err_code;
    ble_advertising_init_t init;

    // Add ANCS UUID to solicited UUIDs
    adv_solicited_uuids[0] = get_ancs()->service.service.uuid;

    memset(&init, 0, sizeof(init));

    init.advdata.name_type = BLE_ADVDATA_FULL_NAME;
    init.advdata.include_appearance = true;
    init.advdata.flags = BLE_GAP_ADV_FLAGS_LE_ONLY_LIMITED_DISC_MODE;
    init.advdata.uuids_solicited.uuid_cnt =
            sizeof(adv_solicited_uuids) / sizeof(adv_solicited_uuids[0]);
    init.advdata.uuids_solicited.p_uuids = adv_solicited_uuids;

    init.config.ble_adv_whitelist_enabled = true;
    init.config.ble_adv_fast_enabled = true;
    init.config.ble_adv_fast_interval = APP_ADV_FAST_INTERVAL;
    init.config.ble_adv_fast_timeout = APP_ADV_FAST_DURATION;
    init.config.ble_adv_slow_enabled = true;
    init.config.ble_adv_slow_interval = APP_ADV_SLOW_INTERVAL;
    init.config.ble_adv_slow_timeout = APP_ADV_SLOW_DURATION;

    init.evt_handler = adv_evt_handler;

    err_code = ble_advertising_init(&m_advertising, &init);
    APP_ERROR_CHECK(err_code);

    ble_advertising_conn_cfg_tag_set(&m_advertising, APP_BLE_CONN_CFG_TAG);

}


/**@brief Function for starting advertising.
 */
void advertising_start(bool erase_bonds) {

    ret_code_t err_code;

    if (erase_bonds == true) {

        // Delete peers in peer manager
        // Advertising is started by PM_EVT_PEERS_DELETE_SUCCEEDED event.
        NRF_LOG_INFO("Erase bonds!");
        err_code = pm_peers_delete();
        APP_ERROR_CHECK(err_code);

    } else {

        // Update whitelist
        memset(whitelist, PM_PEER_ID_INVALID, sizeof(whitelist));
        whitelist_len = (sizeof(whitelist) / sizeof(pm_peer_id_t));
        peer_list_get(whitelist, &whitelist_len);

        err_code = pm_whitelist_set(whitelist, whitelist_len);
        APP_ERROR_CHECK(err_code);

        // Setup the device identities list.
        // Some SoftDevices do not support this feature.
        err_code = pm_device_identities_list_set(whitelist, whitelist_len);
        if (err_code != NRF_ERROR_NOT_SUPPORTED) {
            APP_ERROR_CHECK(err_code);
        }

        err_code = ble_advertising_start(&m_advertising, BLE_ADV_MODE_FAST);
        APP_ERROR_CHECK(err_code);

    }

}
