//
// Created by Carl Friess on 08/01/2021.
//

#ifndef PHASE_PHASE_ANCS_H
#define PHASE_PHASE_ANCS_H

#include "nrf_ble_ancs_c.h"

/**@brief Returns the ANCS Client instance.
 */
ble_ancs_c_t *get_ancs(void);

/**@brief Function for initializing GATT Service client.
 */
void ancs_init(void);

#endif //PHASE_PHASE_ANCS_H
