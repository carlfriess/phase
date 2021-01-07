//
// Created by Carl Friess on 31/12/2020.
//

#ifndef PHASE_BLUETOOTH_H
#define PHASE_BLUETOOTH_H

#include <stdbool.h>

void services_init(void);

void conn_params_init(void);

void buttons_leds_init(bool *p_erase_bonds);

void peer_manager_init(void);

#endif //PHASE_BLUETOOTH_H
