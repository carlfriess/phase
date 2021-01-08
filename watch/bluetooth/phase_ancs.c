//
// Created by Carl Friess on 08/01/2021.
//

#include "phase_ancs.h"

#include "nrf_delay.h"
#include "nrf_log.h"

#include "phase_gatt.h"


#define ATTR_DATA_SIZE                 BLE_ANCS_ATTR_DATA_MAX                 /**< Allocated size for attribute data. */


static ble_ancs_c_evt_notif_t m_notification_latest;                          /**< Local copy to keep track of the newest arriving notifications. */
static ble_ancs_c_attr_t m_notif_attr_latest;                            /**< Local copy of the newest notification attribute. */
static ble_ancs_c_attr_t m_notif_attr_app_id_latest;                     /**< Local copy of the newest app attribute. */

static uint8_t m_attr_appid[ATTR_DATA_SIZE];                                  /**< Buffer to store attribute data. */
static uint8_t m_attr_title[ATTR_DATA_SIZE];                                  /**< Buffer to store attribute data. */
static uint8_t m_attr_subtitle[ATTR_DATA_SIZE];                               /**< Buffer to store attribute data. */
static uint8_t m_attr_message[ATTR_DATA_SIZE];                                /**< Buffer to store attribute data. */
static uint8_t m_attr_message_size[ATTR_DATA_SIZE];                           /**< Buffer to store attribute data. */
static uint8_t m_attr_date[ATTR_DATA_SIZE];                                   /**< Buffer to store attribute data. */
static uint8_t m_attr_posaction[ATTR_DATA_SIZE];                              /**< Buffer to store attribute data. */
static uint8_t m_attr_negaction[ATTR_DATA_SIZE];                              /**< Buffer to store attribute data. */
static uint8_t m_attr_disp_name[ATTR_DATA_SIZE];                              /**< Buffer to store attribute data. */

BLE_ANCS_C_DEF(ancs);


/**@brief Returns the ANCS Client instance.
 */
ble_ancs_c_t *get_ancs(void) {
    return &ancs;
}


/**@brief String literals for the iOS notification categories. used then printing to UART. */
static char const *lit_catid[BLE_ANCS_NB_OF_CATEGORY_ID] =
        {
                "Other",
                "Incoming Call",
                "Missed Call",
                "Voice Mail",
                "Social",
                "Schedule",
                "Email",
                "News",
                "Health And Fitness",
                "Business And Finance",
                "Location",
                "Entertainment"
        };

/**@brief String literals for the iOS notification event types. Used then printing to UART. */
static char const *lit_eventid[BLE_ANCS_NB_OF_EVT_ID] =
        {
                "Added",
                "Modified",
                "Removed"
        };

/**@brief String literals for the iOS notification attribute types. Used when printing to UART. */
static char const *lit_attrid[BLE_ANCS_NB_OF_NOTIF_ATTR] =
        {
                "App Identifier",
                "Title",
                "Subtitle",
                "Message",
                "Message Size",
                "Date",
                "Positive Action Label",
                "Negative Action Label"
        };

/**@brief String literals for the iOS notification attribute types. Used When printing to UART. */
static char const *lit_appid[BLE_ANCS_NB_OF_APP_ATTR] =
        {
                "Display Name"
        };


/**@brief Function for printing an iOS notification.
 *
 * @param[in] p_notif  Pointer to the iOS notification.
 */
static void notif_print(ble_ancs_c_evt_notif_t *p_notif) {
    NRF_LOG_INFO("\r\nNotification");
    NRF_LOG_INFO("Event:       %s", (uint32_t) lit_eventid[p_notif->evt_id]);
    NRF_LOG_INFO("Category ID: %s", (uint32_t) lit_catid[p_notif->category_id]);
    NRF_LOG_INFO("Category Cnt:%u", (unsigned int) p_notif->category_count);
    NRF_LOG_INFO("UID:         %u", (unsigned int) p_notif->notif_uid);

    NRF_LOG_INFO("Flags:");
    if (p_notif->evt_flags.silent == 1) {
        NRF_LOG_INFO(" Silent");
    }
    if (p_notif->evt_flags.important == 1) {
        NRF_LOG_INFO(" Important");
    }
    if (p_notif->evt_flags.pre_existing == 1) {
        NRF_LOG_INFO(" Pre-existing");
    }
    if (p_notif->evt_flags.positive_action == 1) {
        NRF_LOG_INFO(" Positive Action");
    }
    if (p_notif->evt_flags.negative_action == 1) {
        NRF_LOG_INFO(" Negative Action");
    }
}


/**@brief Function for printing iOS notification attribute data.
 *
 * @param[in] p_attr Pointer to an iOS notification attribute.
 */
static void notif_attr_print(ble_ancs_c_attr_t *p_attr) {
    if (p_attr->attr_len != 0) {
        NRF_LOG_INFO("%s: %s", (uint32_t) lit_attrid[p_attr->attr_id],
                     nrf_log_push((char *) p_attr->p_attr_data));
    } else if (p_attr->attr_len == 0) {
        NRF_LOG_INFO("%s: (N/A)", (uint32_t) lit_attrid[p_attr->attr_id]);
    }
}


/**@brief Function for printing iOS notification attribute data.
 *
 * @param[in] p_attr Pointer to an iOS App attribute.
 */
static void app_attr_print(ble_ancs_c_attr_t *p_attr) {
    if (p_attr->attr_len != 0) {
        NRF_LOG_INFO("%s: %s", (uint32_t) lit_appid[p_attr->attr_id],
                     (uint32_t) p_attr->p_attr_data);
    } else if (p_attr->attr_len == 0) {
        NRF_LOG_INFO("%s: (N/A)", (uint32_t) lit_appid[p_attr->attr_id]);
    }
}


/**@brief Function for printing out errors that originated from the Notification Provider (iOS).
 *
 * @param[in] err_code_np Error code received from NP.
 */
static void err_code_print(uint16_t err_code_np) {
    switch (err_code_np) {
        case BLE_ANCS_NP_UNKNOWN_COMMAND:
            NRF_LOG_INFO(
                    "Error: Command ID was not recognized by the Notification Provider. ");
            break;

        case BLE_ANCS_NP_INVALID_COMMAND:
            NRF_LOG_INFO(
                    "Error: Command failed to be parsed on the Notification Provider. ");
            break;

        case BLE_ANCS_NP_INVALID_PARAMETER:
            NRF_LOG_INFO(
                    "Error: Parameter does not refer to an existing object on the Notification Provider. ");
            break;

        case BLE_ANCS_NP_ACTION_FAILED:
            NRF_LOG_INFO(
                    "Error: Perform Notification Action Failed on the Notification Provider. ");
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
 * @param[in] p_evt  Event received from the Apple Notification Service client.
 */
static void ancs_c_evt_handler(ble_ancs_c_evt_t *p_evt) {

    ret_code_t ret;

    switch (p_evt->evt_type) {
        case BLE_ANCS_C_EVT_DISCOVERY_COMPLETE:

            NRF_LOG_DEBUG("Apple Notification Center Service discovered on the "
                          "server.");

            ret = nrf_ble_ancs_c_handles_assign(&ancs, p_evt->conn_handle,
                                                &p_evt->service);
            APP_ERROR_CHECK(ret);

            // Delay because we cannot add a CCCD too close to starting
            // encryption (iOS specific).
            nrf_delay_ms(100);

            ret = ble_ancs_c_notif_source_notif_enable(&ancs);
            APP_ERROR_CHECK(ret);

            ret = ble_ancs_c_data_source_notif_enable(&ancs);
            APP_ERROR_CHECK(ret);

            NRF_LOG_DEBUG("Notifications Enabled.");
            break;

        case BLE_ANCS_C_EVT_NOTIF:
            m_notification_latest = p_evt->notif;
            notif_print(&m_notification_latest);
            break;

        case BLE_ANCS_C_EVT_NOTIF_ATTRIBUTE:
            m_notif_attr_latest = p_evt->attr;
            notif_attr_print(&m_notif_attr_latest);
            if (p_evt->attr.attr_id == BLE_ANCS_NOTIF_ATTR_ID_APP_IDENTIFIER) {
                m_notif_attr_app_id_latest = p_evt->attr;
            }
            break;
        case BLE_ANCS_C_EVT_DISCOVERY_FAILED:
            NRF_LOG_DEBUG("Apple Notification Center Service not discovered on "
                          "the server.");
            break;

        case BLE_ANCS_C_EVT_APP_ATTRIBUTE:
            app_attr_print(&p_evt->attr);
            break;
        case BLE_ANCS_C_EVT_NP_ERROR:
            err_code_print(p_evt->err_code_np);
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

    err = nrf_ble_ancs_c_attr_add(&ancs,
                                  BLE_ANCS_NOTIF_ATTR_ID_APP_IDENTIFIER,
                                  m_attr_appid,
                                  ATTR_DATA_SIZE);
    APP_ERROR_CHECK(err);

    err = nrf_ble_ancs_c_app_attr_add(&ancs,
                                      BLE_ANCS_APP_ATTR_ID_DISPLAY_NAME,
                                      m_attr_disp_name,
                                      sizeof(m_attr_disp_name));
    APP_ERROR_CHECK(err);

    err = nrf_ble_ancs_c_attr_add(&ancs,
                                  BLE_ANCS_NOTIF_ATTR_ID_TITLE,
                                  m_attr_title,
                                  ATTR_DATA_SIZE);
    APP_ERROR_CHECK(err);

    err = nrf_ble_ancs_c_attr_add(&ancs,
                                  BLE_ANCS_NOTIF_ATTR_ID_MESSAGE,
                                  m_attr_message,
                                  ATTR_DATA_SIZE);
    APP_ERROR_CHECK(err);

    err = nrf_ble_ancs_c_attr_add(&ancs,
                                  BLE_ANCS_NOTIF_ATTR_ID_SUBTITLE,
                                  m_attr_subtitle,
                                  ATTR_DATA_SIZE);
    APP_ERROR_CHECK(err);

    err = nrf_ble_ancs_c_attr_add(&ancs,
                                  BLE_ANCS_NOTIF_ATTR_ID_MESSAGE_SIZE,
                                  m_attr_message_size,
                                  ATTR_DATA_SIZE);
    APP_ERROR_CHECK(err);

    err = nrf_ble_ancs_c_attr_add(&ancs,
                                  BLE_ANCS_NOTIF_ATTR_ID_DATE,
                                  m_attr_date,
                                  ATTR_DATA_SIZE);
    APP_ERROR_CHECK(err);

    err = nrf_ble_ancs_c_attr_add(&ancs,
                                  BLE_ANCS_NOTIF_ATTR_ID_POSITIVE_ACTION_LABEL,
                                  m_attr_posaction,
                                  ATTR_DATA_SIZE);
    APP_ERROR_CHECK(err);

    err = nrf_ble_ancs_c_attr_add(&ancs,
                                  BLE_ANCS_NOTIF_ATTR_ID_NEGATIVE_ACTION_LABEL,
                                  m_attr_negaction,
                                  ATTR_DATA_SIZE);
    APP_ERROR_CHECK(err);

    ancs_c_init.evt_handler = ancs_c_evt_handler;
    ancs_c_init.error_handler = apple_notification_error_handler;
    ancs_c_init.p_gatt_queue = get_gatt_queue();

    err = ble_ancs_c_init(&ancs, &ancs_c_init);
    APP_ERROR_CHECK(err);

}
