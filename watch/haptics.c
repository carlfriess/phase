//
// Created by Carl Friess on 11/03/2022.
//

#include "haptics.h"

#include "app_timer.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"

#define DRV2605_ADR 0x5A

#define SIZEOFARRAY(arr)    (sizeof(arr) / sizeof((arr)[0]))

APP_TIMER_DEF(haptics_low_power_timer);

static uint8_t hpt_en;

static void enter_low_power_mode(void *ctx) {

    // Put device in low power mode
    nrf_gpio_pin_write(hpt_en, 0);

}

void haptics_init(uint8_t hpt_en_pin, const nrf_twi_mngr_t *twi_manager) {

    ret_code_t err;

    hpt_en = hpt_en_pin;

    // Initialize app timer
    err = app_timer_create(&haptics_low_power_timer, APP_TIMER_MODE_SINGLE_SHOT,
                           enter_low_power_mode);

    // Enable DRV2605 haptic driver
    nrf_gpio_cfg_output(hpt_en);
    nrf_gpio_pin_write(hpt_en, 1);

    // Reset the device
    {
        uint8_t buf[2] = {0x01, 0x87};
        const nrf_twi_mngr_transfer_t transfers[] = {
                NRF_TWI_MNGR_WRITE(DRV2605_ADR, buf, sizeof(buf), 0),
        };
        err = nrf_twi_mngr_perform(twi_manager, NULL, transfers,
                                   SIZEOFARRAY(transfers), NULL);
        APP_ERROR_CHECK(err);
    }

    // Wait for device to complete reset
    {
        uint8_t reg = 0x01;
        uint8_t mode;
        const nrf_twi_mngr_transfer_t transfers[] = {
                NRF_TWI_MNGR_WRITE(DRV2605_ADR, &reg, 1, NRF_TWI_MNGR_NO_STOP),
                NRF_TWI_MNGR_READ(DRV2605_ADR, &mode, 1, 0)
        };
        do {
            err = nrf_twi_mngr_perform(twi_manager, NULL, transfers,
                                       SIZEOFARRAY(transfers), NULL);
        } while (err != NRF_SUCCESS || mode & 0x80);
    }

    // Execute auto calibration
    {
        // MODE = 7
        uint8_t reg01[2] = {0x01, 0x07};

        // RATED_VOLTAGE = 97
        // OD_CLAMP = 105
        uint8_t reg16[] = {0x16, 0x61, 0x69};

        // ERM_LRA = 1
        // FB_BRAKE_FACTOR = 2
        // LOOP_GAIN = 2
        // DRIVE_TIME = 28
        // SAMPLE_TIME = 3
        // BLANKING_TIME = 1
        // IDISS_TIME = 1
        uint8_t reg1a[] = {0x1A, 0xAA, 0x9C, 0xFA};

        // AUTO_CAL_TIME = 3
        uint8_t reg1e[] = {0x1E, 0x30};

        // GO = 1
        uint8_t reg0c[] = {0x0C, 0x01};

        const nrf_twi_mngr_transfer_t transfers[] = {
                NRF_TWI_MNGR_WRITE(DRV2605_ADR, reg01, sizeof(reg01), 0),
                NRF_TWI_MNGR_WRITE(DRV2605_ADR, reg16, sizeof(reg16), 0),
                NRF_TWI_MNGR_WRITE(DRV2605_ADR, reg1a, sizeof(reg1a), 0),
                NRF_TWI_MNGR_WRITE(DRV2605_ADR, reg1e, sizeof(reg1e), 0),
                NRF_TWI_MNGR_WRITE(DRV2605_ADR, reg0c, sizeof(reg0c), 0),
        };

        err = nrf_twi_mngr_perform(twi_manager, NULL, transfers,
                                   SIZEOFARRAY(transfers), NULL);
        APP_ERROR_CHECK(err);
    }

    // Wait for device to complete auto calibration
    {
        uint8_t reg = 0x0C;
        uint8_t go;
        const nrf_twi_mngr_transfer_t transfers[] = {
                NRF_TWI_MNGR_WRITE(DRV2605_ADR, &reg, 1, NRF_TWI_MNGR_NO_STOP),
                NRF_TWI_MNGR_READ(DRV2605_ADR, &go, 1, 0)
        };
        do {
            nrf_delay_ms(100);
            err = nrf_twi_mngr_perform(twi_manager, NULL, transfers,
                                       SIZEOFARRAY(transfers), NULL);
            APP_ERROR_CHECK(err);
        } while (go);
    }

    // Check calibration result
    {
        uint8_t reg = 0x00;
        uint8_t status;
        const nrf_twi_mngr_transfer_t transfers[] = {
                NRF_TWI_MNGR_WRITE(DRV2605_ADR, &reg, 1, NRF_TWI_MNGR_NO_STOP),
                NRF_TWI_MNGR_READ(DRV2605_ADR, &status, 1, 0)
        };

        err = nrf_twi_mngr_perform(twi_manager, NULL, transfers,
                                   SIZEOFARRAY(transfers), NULL);
        APP_ERROR_CHECK(err);
        if (status & 0x08) {
            NRF_LOG_ERROR("Haptics calibration FAILED!");
            APP_ERROR_HANDLER(NRF_ERROR_INTERNAL);
        } else {
            NRF_LOG_INFO("Haptics calibration passed!");
        }
    }

    // Configure for library playback
    {
        // LIBRARY_SEL = 6 (LRA Library)
        uint8_t reg03[] = {0x03, 0x06};

        // MODE = 0 (Internal trigger)
        uint8_t reg01[] = {0x01, 0x00};

        const nrf_twi_mngr_transfer_t transfers[] = {
                NRF_TWI_MNGR_WRITE(DRV2605_ADR, reg03, sizeof(reg03), 0),
                NRF_TWI_MNGR_WRITE(DRV2605_ADR, reg01, sizeof(reg01), 0),
        };

        err = nrf_twi_mngr_perform(twi_manager, NULL, transfers,
                                   SIZEOFARRAY(transfers), NULL);
        APP_ERROR_CHECK(err);
    }

    // Put device in low power mode
    nrf_gpio_pin_write(hpt_en, 0);

}

void haptics_play_effect(const nrf_twi_mngr_t *twi_manager, uint8_t effect) {

    ret_code_t err;

    // Stop the timer if it is already running
    app_timer_stop(haptics_low_power_timer);

    // Enable the device
    nrf_gpio_pin_write(hpt_en, 1);

    // Effect sequence + GO
    static uint8_t seq[] = {0x04,
                            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                            0x01};
    seq[1] = effect;

    static const nrf_twi_mngr_transfer_t transfers[] = {
            NRF_TWI_MNGR_WRITE(DRV2605_ADR, seq, sizeof(seq), 0),
    };

    static const nrf_twi_mngr_transaction_t transaction = {
            .p_transfers = transfers,
            .number_of_transfers = SIZEOFARRAY(transfers),
            .p_required_twi_cfg = NULL,
            .callback = NULL,
            .p_user_data = NULL,
    };

    err = nrf_twi_mngr_schedule(twi_manager, &transaction);
    APP_ERROR_CHECK(err);

    // Schedule the timer for disabling the haptic driver
    err = app_timer_start(haptics_low_power_timer, APP_TIMER_TICKS(500), NULL);
    APP_ERROR_CHECK(err);

}