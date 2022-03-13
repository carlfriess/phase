//
// Created by Carl Friess on 11/03/2022.
//

#ifndef PHASE_HAPTICS_H
#define PHASE_HAPTICS_H

#include <stdint.h>

#include "nrf_twi_mngr.h"

void haptics_init(uint8_t hpt_en, const nrf_twi_mngr_t *twi_manager);

void haptics_play_effect(const nrf_twi_mngr_t *twi_manager, uint8_t effect);

#endif //PHASE_HAPTICS_H
