//
// Created by Carl Friess on 07/01/2021.
//

#ifndef PHASE_PHASE_BLUETOOTH_HANDLERS_H
#define PHASE_PHASE_BLUETOOTH_HANDLERS_H

#include <time.h>

/**@brief Handler that is called with the current time reported by the Current
 * Time Service.
 */
void bluetooth_time_handler(time_t time);

#endif //PHASE_PHASE_BLUETOOTH_HANDLERS_H
