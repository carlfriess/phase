//
// Created by Carl Friess on 07/01/2021.
//

#include "gatt.h"

#include "app_error.h"
#include "ble_cts_c.h"
#include "ble_db_discovery.h"
#include "nrf_ble_gatt.h"


NRF_BLE_GATT_DEF(m_gatt);           /**< GATT module instance. */
NRF_BLE_GQ_DEF(m_ble_gatt_queue,    /**< BLE GATT Queue instance. */
               NRF_SDH_BLE_PERIPHERAL_LINK_COUNT,
               NRF_BLE_GQ_QUEUE_SIZE);


/**@brief Function for initializing the GATT module.
 */
void gatt_init(void) {
    ret_code_t err_code = nrf_ble_gatt_init(&m_gatt, NULL);
    APP_ERROR_CHECK(err_code);
}


/**@brief Function for handling Database Discovery events.
 *
 * @details This function is a callback function to handle events from the
 * database discovery module. Depending on the UUIDs that are discovered, this
 * function should forward the events to their respective service instances.
 *
 * @param[in] p_event  Pointer to the database discovery event.
 */
static void db_disc_handler(ble_db_discovery_evt_t * p_evt)
{
    ble_cts_c_on_db_disc_evt(&m_cts_c, p_evt);
}


/**
 * @brief Database discovery collector initialization.
 */
void db_discovery_init(void)
{
    ble_db_discovery_init_t db_init;

    memset(&db_init, 0, sizeof(ble_db_discovery_init_t));

    db_init.evt_handler  = db_disc_handler;
    db_init.p_gatt_queue = &m_ble_gatt_queue;

    ret_code_t err_code = ble_db_discovery_init(&db_init);
    APP_ERROR_CHECK(err_code);
}