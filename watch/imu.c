//
// Created by Carl Friess on 06/05/2022.
//

#include "imu.h"

#include "bmi2.h"
#include "bmi270.h"
#include "nrf_delay.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrfx_gpiote.h"


#define BMI270_ADR  BMI2_I2C_PRIM_ADDR

#define SIZEOFARRAY(arr)    (sizeof(arr) / sizeof((arr)[0]))

static BMI2_INTF_RETURN_TYPE bmi2_i2c_read(uint8_t reg, uint8_t *data,
                                           uint32_t len, void *intf_ptr) {

    // Get the TWI manager instance
    nrf_twi_mngr_t *twi_manager = (nrf_twi_mngr_t *) intf_ptr;

    // Execute transfers
    const nrf_twi_mngr_transfer_t transfers[] = {
            NRF_TWI_MNGR_WRITE(BMI270_ADR, &reg, 1, NRF_TWI_MNGR_NO_STOP),
            NRF_TWI_MNGR_READ(BMI270_ADR, data, len, 0),
    };
    ret_code_t err = nrf_twi_mngr_perform(twi_manager, NULL, transfers,
                                          SIZEOFARRAY(transfers), NULL);

    return err == NRF_SUCCESS ? BMI2_INTF_RET_SUCCESS : !BMI2_INTF_RET_SUCCESS;
}

static BMI2_INTF_RETURN_TYPE bmi2_i2c_write(uint8_t reg, const uint8_t *data,
                                            uint32_t len, void *intf_ptr) {

    // Get the TWI manager instance
    nrf_twi_mngr_t *twi_manager = (nrf_twi_mngr_t *) intf_ptr;

    // Check data size
    if (len > 0xFE) return !BMI2_INTF_RET_SUCCESS;

    // Prepare transmission buffer
    static uint8_t buf[0xFF];
    buf[0] = reg;
    memcpy(buf + 1, data, len);

    // Execute transfers
    const nrf_twi_mngr_transfer_t transfers[] = {
            NRF_TWI_MNGR_WRITE(BMI270_ADR, buf, len + 1, 0),
    };
    ret_code_t err = nrf_twi_mngr_perform(twi_manager, NULL, transfers,
                                          SIZEOFARRAY(transfers), NULL);

    return err == NRF_SUCCESS ? BMI2_INTF_RET_SUCCESS : !BMI2_INTF_RET_SUCCESS;
}

static void bmi2_delay_us(uint32_t period, void *intf_ptr) {
    nrf_delay_us(period);
}

static int8_t bmi2_set_config(struct bmi2_dev *bmi2_dev) {

    int8_t res;

    // List the sensors which are required to enable
    uint8_t sens_list[2] = {BMI2_ACCEL, BMI2_WRIST_WEAR_WAKE_UP};

    // Structure to define the type of the sensor and its configurations
    struct bmi2_sens_config config;

    // Configure type of feature
    config.type = BMI2_WRIST_WEAR_WAKE_UP;

    // Enable the selected sensors
    res = bmi270_sensor_enable(sens_list, 2, bmi2_dev);

    if (res == BMI2_OK) {

        // Get default configurations for the type of feature selected
        res = bmi270_get_sensor_config(&config, 1, bmi2_dev);

        if (res == BMI2_OK) {

            // Set the new configuration
            res = bmi270_set_sensor_config(&config, 1, bmi2_dev);

        }

    }

    return res;
}

static void int1_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
    NRF_LOG_INFO("IMU INT1");
}

static void int2_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
    NRF_LOG_INFO("IMU INT2");
}

int8_t imu_init(uint8_t int1, uint8_t int2, const nrf_twi_mngr_t *twi_manager) {

    int8_t res;

    // Initialize status of wrist wear wakeup interrupt
    uint16_t int_status = 0;

    // Sensor initialization configuration
    struct bmi2_dev bmi2_dev;

    // Select features and their pins to be mapped to
    struct bmi2_sens_int_config sens_int = {
            .type = BMI2_WRIST_WEAR_WAKE_UP,
            .hw_int_pin = BMI2_INT1
    };

    // Interrupt pin configuration
    struct bmi2_int_pin_config int_pin_cfg = {
            .pin_type = BMI2_INT_BOTH,
            .pin_cfg = {
                    {
                            .output_en = BMI2_INT_OUTPUT_ENABLE,
                    },
                    {
                            .output_en = BMI2_INT_OUTPUT_ENABLE,
                    },
            },
    };

    // Axes remap configuration
    struct bmi2_remap axes_remap = {
            .x = BMI2_Y,
            .y = BMI2_X,
            .z = BMI2_NEG_Z,
    };

    // Set device interface
    bmi2_dev.intf = BMI2_I2C_INTF;
    bmi2_dev.intf_ptr = (void *) twi_manager;
    bmi2_dev.read_write_len = 0xFE;
    bmi2_dev.read = bmi2_i2c_read;
    bmi2_dev.write = bmi2_i2c_write;
    bmi2_dev.delay_us = bmi2_delay_us;

    // Assign to NULL to load the default config file
    bmi2_dev.config_file_ptr = NULL;

    // Attempt to perform soft-reset of device
    uint8_t buf[2] = {0x7E, 0xB6};
    const nrf_twi_mngr_transfer_t transfers[] = {
            NRF_TWI_MNGR_WRITE(BMI270_ADR, buf, sizeof(buf), 0),
    };
    ret_code_t err = nrf_twi_mngr_perform(twi_manager, NULL, transfers,
                                          SIZEOFARRAY(transfers), NULL);
    if (err != NRF_SUCCESS) {
        NRF_LOG_ERROR("Failed to reset IMU");
        return BMI2_E_COM_FAIL;
    }
    nrf_delay_ms(2);

    // Initialize BMI270
    res = bmi270_init(&bmi2_dev);
    if (res != BMI2_OK) return res;

    // Set the sensor configuration
    res = bmi2_set_config(&bmi2_dev);
    if (res != BMI2_OK) return res;

    // Map the feature interrupt
    res = bmi270_map_feat_int(&sens_int, 1, &bmi2_dev);
    if (res != BMI2_OK) return res;

    // Enable the interrupt pins
    res = bmi2_set_int_pin_config(&int_pin_cfg, &bmi2_dev);
    if (res != BMI2_OK) return res;

    // Remap axes
    bmi2_set_remap_axes(&axes_remap, &bmi2_dev);
    if (res != BMI2_OK) return res;

    // Configure interrupt handlers
    nrfx_gpiote_in_config_t cfg = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
    err = nrfx_gpiote_in_init(int1, &cfg, int1_handler);
    APP_ERROR_CHECK(err);
    err = nrfx_gpiote_in_init(int2, &cfg, int2_handler);
    APP_ERROR_CHECK(err);
    nrfx_gpiote_in_event_enable(int1, true);
    nrfx_gpiote_in_event_enable(int2, true);

    NRF_LOG_INFO("Tilt the board in landscape position to trigger wrist wear wakeup\n");
    NRF_LOG_FLUSH();

    // Loop to print the wrist wear wakeup data when interrupt occurs
    while (1) {
        int_status = 0;

        // To get the interrupt status of the wrist wear wakeup
        res = bmi2_get_int_status(&int_status, &bmi2_dev);

        // To check the interrupt status of the wrist gesture
        if ((res == BMI2_OK) && (int_status & BMI270_WRIST_WAKE_UP_STATUS_MASK)) {
            NRF_LOG_INFO("Wrist wear wakeup detected\n");
            NRF_LOG_FLUSH();
            break;
        }
    }

    return res;
}
