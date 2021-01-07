//
// Created by Carl Friess on 07/01/2021.
//

#ifndef PHASE_PHASE_GAP_H
#define PHASE_PHASE_GAP_H

/**@brief Function for the GAP initialization.
 *
 * @details This function sets up all the necessary GAP (Generic Access Profile)
 * parameters of the device including the device name, appearance, and the
 * preferred connection parameters.
 */
void gap_params_init(void);

/**@brief Function for initializing the Connection Parameters module.
 */
void conn_params_init(void);

#endif //PHASE_PHASE_GAP_H
