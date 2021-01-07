//
// Created by Carl Friess on 07/01/2021.
//

#ifndef PHASE_PHASE_GATT_H
#define PHASE_PHASE_GATT_H

#include "ble_db_discovery.h"
#include "nrf_ble_gatt.h"

/**@brief Returns the GATT Queue instance.
 */
nrf_ble_gq_t *get_gatt_queue(void);

/**@brief Returns the discovery database instance.
 */
ble_db_discovery_t *get_discovery_db(void);

/**@brief Function for initializing the GATT module.
 */
void gatt_init(void);

/**
 * @brief Database discovery collector initialization.
 */
void db_discovery_init(void);

#endif //PHASE_PHASE_GATT_H
