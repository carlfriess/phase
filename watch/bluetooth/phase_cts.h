//
// Created by Carl Friess on 07/01/2021.
//

#ifndef PHASE_PHASE_CTS_H
#define PHASE_PHASE_CTS_H

#include "ble_cts_c.h"

/**@brief Returns the Current Time Service client instance.
 */
ble_cts_c_t *get_cts(void);

/**@brief Function for initializing Current Time Service client.
 */
void cts_init(void);

#endif //PHASE_PHASE_CTS_H
