//
// Created by Carl Friess on 06/05/2022.
//

#ifndef PHASE_IMU_H
#define PHASE_IMU_H

#include <stdint.h>

#include "nrf_twi_mngr.h"

/**@brief Handler that is called when the IMU raises a wrist wear wakeup event.
 */
void imu_wrist_wake(void);

int8_t imu_init(uint8_t int1, uint8_t int2, const nrf_twi_mngr_t *twi_manager);

#endif //PHASE_IMU_H
