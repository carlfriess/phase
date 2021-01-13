//
// Created by Carl Friess on 08/01/2021.
//

#include "phase_ancs.h"

#include "nrf_delay.h"
#include "nrf_log.h"

#include "phase_bluetooth_handlers.h"
#include "phase_gatt.h"
#include "phase_peer_manager.h"


BLE_ANCS_C_DEF(ancs);


static uint8_t notif_attr_appid[BLE_ANCS_ATTR_DATA_MAX];
static uint8_t notif_attr_title[BLE_ANCS_ATTR_DATA_MAX];
static uint8_t notif_attr_message[BLE_ANCS_ATTR_DATA_MAX];


/**@brief Returns the ANCS Client instance.
 */
ble_ancs_c_t *get_ancs(void) {
    return &ancs;
}


/**@brief Function for printing out errors that originated from the Notification
 * Provider (iOS).
 *
 * @param[in] err_code_np Error code received from NP.
 */
static void err_code_print(uint16_t err_code_np) {
    switch (err_code_np) {
        case BLE_ANCS_NP_UNKNOWN_COMMAND:
            NRF_LOG_WARNING("ANCS error: Command ID was not recognized by the "
                            "Notification Provider");
            break;

        case BLE_ANCS_NP_INVALID_COMMAND:
            NRF_LOG_WARNING("ANCS error: Command failed to be parsed on the "
                            "Notification Provider");
            break;

        case BLE_ANCS_NP_INVALID_PARAMETER:
            NRF_LOG_WARNING("ANCS error: Parameter does not refer to an "
                            "existing object on the Notification Provider");
            break;

        case BLE_ANCS_NP_ACTION_FAILED:
            NRF_LOG_WARNING("Error: Perform Notification Action Failed on the "
                            "Notification Provider");
            break;

        default:
            break;
    }
}


/**@brief Function for handling the Apple Notification Service client.
 *
 * @details This function is called for all events in the Apple Notification client that
 *          are passed to the application.
 *
 * @param[in] evt  Event received from the Apple Notification Service client.
 */
static void ancs_c_evt_handler(ble_ancs_c_evt_t *evt) {

    ret_code_t ret;

    switch (evt->evt_type) {
        case BLE_ANCS_C_EVT_DISCOVERY_COMPLETE:
            ret = nrf_ble_ancs_c_handles_assign(&ancs, evt->conn_handle,
                                                &evt->service);
            APP_ERROR_CHECK(ret);

            // Register discovered service with peer manager
            discovered_ancs(evt->conn_handle, &evt->service);

            // Delay because we cannot add a CCCD too close to starting
            // encryption (iOS specific).
            nrf_delay_ms(100);

            ret = ble_ancs_c_notif_source_notif_enable(&ancs);
            APP_ERROR_CHECK(ret);

            ret = ble_ancs_c_data_source_notif_enable(&ancs);
            APP_ERROR_CHECK(ret);
            break;

        case BLE_ANCS_C_EVT_NOTIF:
            // Check if we want to display this notification
            if (evt->notif.evt_id == BLE_ANCS_EVENT_ID_NOTIFICATION_ADDED &&
                !evt->notif.evt_flags.pre_existing) {
                // Request the notification attributes
                nrf_ble_ancs_c_request_attrs(&ancs, &evt->notif);
            }
            break;

        case BLE_ANCS_C_EVT_NOTIF_ATTRIBUTE:
            if (evt->attr.attr_id == BLE_ANCS_NOTIF_ATTR_ID_MESSAGE) {
                bluetooth_notification_add((char *) notif_attr_appid,
                                           (char *) notif_attr_title,
                                           (char *) notif_attr_message);
            }
            break;

        case BLE_ANCS_C_EVT_APP_ATTRIBUTE:
            break;

        case BLE_ANCS_C_EVT_DISCOVERY_FAILED:
            NRF_LOG_DEBUG("Apple Notification Center Service not discovered on "
                          "the server.");
            break;

        case BLE_ANCS_C_EVT_NP_ERROR:
            err_code_print(evt->err_code_np);
            break;

        default:
            // No implementation needed.
            break;
    }

}


/**@brief Function for handling the Apple Notification Service client errors.
 *
 * @param[in] nrf_error  Error code containing information about the error.
 */
static void apple_notification_error_handler(uint32_t nrf_error) {
    APP_ERROR_HANDLER(nrf_error);
}


/**@brief Function for initializing GATT Service client.
 */
void ancs_init(void) {

    ret_code_t err;
    ble_ancs_c_init_t ancs_c_init;

    // Init the Apple Notification Center Service client module.
    memset(&ancs_c_init, 0, sizeof(ancs_c_init));

    err = nrf_ble_ancs_c_attr_add(&ancs, BLE_ANCS_NOTIF_ATTR_ID_APP_IDENTIFIER,
                                  notif_attr_appid, sizeof(notif_attr_appid));
    APP_ERROR_CHECK(err);

    err = nrf_ble_ancs_c_attr_add(&ancs, BLE_ANCS_NOTIF_ATTR_ID_TITLE,
                                  notif_attr_title, sizeof(notif_attr_title));
    APP_ERROR_CHECK(err);

    err = nrf_ble_ancs_c_attr_add(&ancs, BLE_ANCS_NOTIF_ATTR_ID_MESSAGE,
                                  notif_attr_message,
                                  sizeof(notif_attr_message));
    APP_ERROR_CHECK(err);

    ancs_c_init.evt_handler = ancs_c_evt_handler;
    ancs_c_init.error_handler = apple_notification_error_handler;
    ancs_c_init.p_gatt_queue = get_gatt_queue();

    err = ble_ancs_c_init(&ancs, &ancs_c_init);
    APP_ERROR_CHECK(err);

}
