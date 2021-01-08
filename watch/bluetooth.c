//
// Created by Carl Friess on 07/01/2021.
//

#include "bluetooth.h"

#include "app_error.h"

#include "bluetooth/phase_ble.h"
#include "bluetooth/phase_gap.h"
#include "bluetooth/phase_gatt.h"
#include "bluetooth/phase_peer_manager.h"
#include "bluetooth/phase_cts.h"

/**@brief Function for initializing the Bluetooth stack and services.
 */
void bluetooth_init(void) {
    ble_stack_init();
    gap_params_init();
    gatt_init();
    db_discovery_init();
    advertising_init();
    peer_manager_init();
    cts_init();
    conn_params_init();
}


/**@brief Start advertising via Bluetooth.
 *
 * @param   erase_bonds Whether to erase previously established bonds.
 */
void bluetooth_start_advertising(bool erase_bonds) {
    advertising_start(erase_bonds);
}


/**@brief Request current time from Current Time Service.
 */
void bluetooth_request_time(void) {
    ret_code_t err = ble_cts_c_current_time_read(get_cts());
    if (err != NRF_ERROR_NOT_FOUND) {
        APP_ERROR_CHECK(err);
    }
}
