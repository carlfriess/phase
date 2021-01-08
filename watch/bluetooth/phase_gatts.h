//
// Created by Carl Friess on 08/01/2021.
//

#ifndef PHASE_PHASE_GATTS_H
#define PHASE_PHASE_GATTS_H

#include "nrf_ble_gatts_c.h"

/**@brief Returns the GATT Service client instance.
 */
nrf_ble_gatts_c_t *get_gatts_c(void);

/**@brief Function for initializing GATT Service client.
 */
void gatts_c_init(void);

#endif //PHASE_PHASE_GATTS_H
