//
// Created by Carl Friess on 07/01/2021.
//

#ifndef PHASE_PHASE_BLE_H
#define PHASE_PHASE_BLE_H

/**@brief Function for initializing the BLE stack.
 *
 * @details Initializes the SoftDevice and the BLE event interrupt. Uses
 * parameters from sdk_config.h.
 */
void ble_stack_init(void);

#endif //PHASE_PHASE_BLE_H
