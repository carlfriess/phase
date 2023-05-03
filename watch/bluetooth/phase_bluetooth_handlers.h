//
// Created by Carl Friess on 07/01/2021.
//

#ifndef PHASE_PHASE_BLUETOOTH_HANDLERS_H
#define PHASE_PHASE_BLUETOOTH_HANDLERS_H

#include <time.h>

/**@brief Handler that is called when the bluetooth connection state changes.
 */
void bluetooth_state_handler(bool connected);

/**@brief Handler that is called with the current time reported by the Current
 * Time Service.
 */
void bluetooth_time_handler(time_t time);

/**@brief Handler that is called when a notification is received.
 */
void bluetooth_notification_add(char *appid, char *title, char *msg);

#endif //PHASE_PHASE_BLUETOOTH_HANDLERS_H
