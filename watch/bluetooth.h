//
// Created by Carl Friess on 31/12/2020.
//

#ifndef PHASE_BLUETOOTH_H
#define PHASE_BLUETOOTH_H

#include <stdbool.h>

void advertising_start(bool erase_bonds);

void gatt_init(void);

void services_init(void);

void conn_params_init(void);

void buttons_leds_init(bool *p_erase_bonds);

void peer_manager_init(void);

void advertising_init(void);

void db_discovery_init(void);

#endif //PHASE_BLUETOOTH_H
