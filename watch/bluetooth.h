//
// Created by Carl Friess on 07/01/2021.
//

#ifndef PHASE_BLUETOOTH_H
#define PHASE_BLUETOOTH_H

#include <stdbool.h>

// Include callback definitions
#include "bluetooth/phase_bluetooth_handlers.h"

/**@brief Function for initializing the Bluetooth stack and services.
 */
void bluetooth_init(void);

/**@brief Start advertising via Bluetooth.
 *
 * @param   erase_bonds Whether to erase previously established bonds.
 */
void bluetooth_start_advertising(bool erase_bonds);

/**@brief Request current time from Current Time Service.
 */
void bluetooth_request_time(void);

#endif //PHASE_BLUETOOTH_H
