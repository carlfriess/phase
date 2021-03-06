//
// Created by Carl Friess on 07/01/2021.
//

#ifndef PHASE_PHASE_PEER_MANAGER_H
#define PHASE_PHASE_PEER_MANAGER_H

#include "ble_cts_c.h"
#include "nrf_ble_ancs_c.h"
#include "peer_manager.h"

// List of whitelisted peers
extern pm_peer_id_t whitelist[BLE_GAP_WHITELIST_ADDR_MAX_COUNT];
extern uint32_t whitelist_len;

/**@brief Function for the Peer Manager initialization.
 */
void peer_manager_init(void);

/**@brief Fetch the list of peer IDs from peer manager.
 *
 * @param[inout] p_peers   The buffer where to store the list of peer IDs.
 * @param[inout] p_size    In: The size of the @p p_peers buffer.
 *                         Out: The number of peers copied in the buffer.
 */
void peer_list_get(pm_peer_id_t *p_peers, uint32_t *p_size);

/**@brief Handler that should be called when the CTS service has been
 * discovered to eventually store the discovered handles in flash.
 */
void discovered_cts(uint16_t conn_handle, const ble_cts_c_handles_t *cts);

/**@brief Handler that should be called when the GATTS service has been
 * discovered to eventually store the discovered handles in flash.
 */
void discovered_gatts(uint16_t conn_handle, const ble_gatt_db_char_t *gatts);

/**@brief Handler that should be called when the ANCS service has been
 * discovered to eventually store the discovered handles in flash.
 */
void discovered_ancs(uint16_t conn_handle, const ble_ancs_c_service_t *ancs);

#endif //PHASE_PHASE_PEER_MANAGER_H
