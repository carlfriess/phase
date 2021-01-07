//
// Created by Carl Friess on 07/01/2021.
//

#ifndef PHASE_PHASE_BLE_H
#define PHASE_PHASE_BLE_H

#include <stdbool.h>

#include "nrf_ble_qwr.h"

/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt. Uses
 * parameters from sdk_config.h.
 */
void ble_stack_init(void);

/**@brief Function for initializing the advertising functionality.
 *
 * @details Encodes the required advertising data and passes it to the stack.
 * Also builds a structure to be passed to the stack when starting advertising.
 */
void advertising_init(void);

/**@brief Function for starting advertising.
 */
void advertising_start(bool erase_bonds);

#endif //PHASE_PHASE_BLE_H
