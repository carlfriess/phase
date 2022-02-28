//
// Created by Carl Friess on 28/02/2022.
//

#ifndef PHASE_POWER_H
#define PHASE_POWER_H

#include <stdint.h>
#include <stdbool.h>

struct power_status {
    uint16_t battery_voltage;   // mV
    bool charging;
};

extern struct power_status power_status;

void power_init(uint8_t chg_status);

#endif //PHASE_POWER_H
