//
// Created by Carl Friess on 06/05/2022.
//

#ifndef PHASE_IMU_H
#define PHASE_IMU_H

#include <stdint.h>

#include "nrf_twi_mngr.h"

int8_t imu_init(const nrf_twi_mngr_t *twi_manager);

#endif //PHASE_IMU_H
