//
// Created by Carl Friess on 28/02/2022.
//

#include "power.h"

#include "ui.h"

#include "app_timer.h"
#include "nrfx_gpiote.h"
#include "nrfx_saadc.h"
#include "nrfx_saadc_v2.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
APP_TIMER_DEF(vbat_timer);

static nrf_saadc_value_t sample;

struct power_status power_status = {
        .charging = false,
        .battery_voltage = 0,
};

static void chg_status_handler(nrfx_gpiote_pin_t pin,
                               nrf_gpiote_polarity_t action) {
    power_status.charging = !nrf_gpio_pin_read(pin);
    NRF_LOG_INFO(power_status.charging ? "Charging" : "Stopped charging");
    ui_set_power(&power_status);
}

static void vbat_handler(nrfx_saadc_evt_t const *evt) {
    if (evt->type == NRFX_SAADC_EVT_DONE) {
        uint32_t vbat = evt->data.done.p_buffer[0];
        vbat *= 5000;
        vbat >>= 10;
        power_status.battery_voltage = vbat;
        NRF_LOG_INFO("Battery voltage: %d mV", vbat);
        ui_set_power(&power_status);
    }
}

static void timer_handler(void *ctx) {

    ret_code_t err;

    // Launch an SAADC conversion on channel 0
    err = nrfx_saadc_simple_mode_set((1 << 0),
                                     NRF_SAADC_RESOLUTION_10BIT,
                                     NRF_SAADC_OVERSAMPLE_DISABLED,
                                     vbat_handler);
    APP_ERROR_CHECK(err);
    err = nrfx_saadc_buffer_set(&sample, 1);
    APP_ERROR_CHECK(err);
    err = nrfx_saadc_mode_trigger();
    APP_ERROR_CHECK(err);

}

void power_init(uint8_t chg_status) {

    ret_code_t err;

    // Initialize GPIOTE module
    err = nrfx_gpiote_init();
    APP_ERROR_CHECK(err);

    // Configure charge status input interrupts
    nrfx_gpiote_in_config_t chg_conf = NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    err = nrfx_gpiote_in_init(chg_status, &chg_conf, chg_status_handler);
    APP_ERROR_CHECK(err);
    nrfx_gpiote_in_event_enable(chg_status, true);

    // Read initial state of charge status pin
    power_status.charging = !nrf_gpio_pin_read(chg_status);

    // Configure an SAADC channel to measure VBAT
    err = nrfx_saadc_init(NRFX_SAADC_CONFIG_IRQ_PRIORITY);
    APP_ERROR_CHECK(err);
    nrfx_saadc_channel_t vbat_channel =
            NRFX_SAADC_DEFAULT_CHANNEL_SE(NRF_SAADC_INPUT_AIN0, 0);
    vbat_channel.channel_config.gain = NRF_SAADC_GAIN1;
    err = nrfx_saadc_channels_config(&vbat_channel, 1);
    APP_ERROR_CHECK(err);

    // Launch initial conversion
    timer_handler(NULL);

    // Use an app timer to measure battery voltage every minute
    err = app_timer_create(&vbat_timer, APP_TIMER_MODE_REPEATED, timer_handler);
    APP_ERROR_CHECK(err);
    err = app_timer_start(vbat_timer, APP_TIMER_TICKS(60 * 1000), NULL);
    APP_ERROR_CHECK(err);

}
