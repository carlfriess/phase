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

// Sensor initialization configuration
static struct bmi2_dev bmi2_dev = {

        // Device interface
        .intf = BMI2_I2C_INTF,
        .read_write_len = 0xFE,
        .read = bmi2_i2c_read,
        .write = bmi2_i2c_write,
        .delay_us = bmi2_delay_us,

        // Assign to NULL to load the default config file
        .config_file_ptr = NULL,

};

static int8_t bmi2_set_config(struct bmi2_dev *dev) {

    int8_t res;

    // List the sensors which are required to enable
    uint8_t sens_list[3] = {
            BMI2_ACCEL,
            BMI2_WRIST_WEAR_WAKE_UP,
            BMI2_WRIST_GESTURE
    };

    // Sensor configurations
    struct bmi2_sens_config config[2] = {
            {
                    .type = BMI2_WRIST_WEAR_WAKE_UP,
            },
            {
                    .type = BMI2_WRIST_GESTURE,
            },
    };

    // Enable the selected sensors
    res = bmi270_sensor_enable(sens_list, 3, dev);
    if (res != BMI2_OK) return res;

    // Get default configurations for the selected features
    res = bmi270_get_sensor_config(config, 2, dev);
    if (res != BMI2_OK) return res;

    // Configure wrist gesture feature
    config[1].cfg.wrist_gest.wearable_arm = BMI2_ARM_LEFT;

    // Set the new configuration
    res = bmi270_set_sensor_config(config, 2, dev);
    if (res != BMI2_OK) return res;

    return res;
}

static void int1_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {
    imu_wrist_wake();
}

static void int2_handler(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action) {

    int8_t res;
    struct bmi2_feat_sensor_data sens_data = {
            .type = BMI2_WRIST_GESTURE,
    };
    const char *gestures[6] = {
            "unknown_gesture",
            "push_arm_down",
            "pivot_up",
            "wrist_shake_jiggle",
            "flick_in",
            "flick_out",
    };

    // Fetch wrist data
    res = bmi270_get_feature_data(&sens_data, 1, &bmi2_dev);
    if (res != BMI2_OK) return;

    NRF_LOG_INFO("%s", gestures[sens_data.sens_data.wrist_gesture_output]);

}

int8_t imu_init(uint8_t int1, uint8_t int2, const nrf_twi_mngr_t *twi_manager) {

    int8_t res;

    // Store TWI manager reference for use by I2C handlers
    bmi2_dev.intf_ptr = (void *) twi_manager;

    // Select features and their pins to be mapped to
    struct bmi2_sens_int_config sens_int[2] = {
            {
                    .type = BMI2_WRIST_WEAR_WAKE_UP,
                    .hw_int_pin = BMI2_INT1,
            },
            {
                    .type = BMI2_WRIST_GESTURE,
                    .hw_int_pin = BMI2_INT2,
            },
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

    // Map the features to interrupt pins
    res = bmi270_map_feat_int(sens_int, 2, &bmi2_dev);
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

    return res;
}
