//
// Created by Carl Friess on 07/05/2022.
//

#include "backlight.h"

#include "app_pwm.h"
#include "app_timer.h"

APP_TIMER_DEF(backlight_fade_timer);
APP_TIMER_DEF(backlight_sleep_timer);
APP_PWM_INSTANCE(PWM2, 2);

static volatile unsigned int backlight_step;
static volatile int backlight_target;

static void backlight_fade_handler(void *ctx) {

    static int current = BACKLIGHT_DEFAULT;
    ret_code_t err;

    // Update duty cycle
    if (current < backlight_target) {
        current += backlight_step;
        if (current > backlight_target) {
            current = backlight_target;
        }
    } else if (current > backlight_target) {
        current -= backlight_step;
        if (current < backlight_target) {
            current = backlight_target;
        }
    }

    // Increase display brightness
    while ((err = app_pwm_channel_duty_set(&PWM2, 0, 100 - current)) ==
           NRF_ERROR_BUSY);
    APP_ERROR_CHECK(err);

    // Check if the target has been reached
    if (backlight_target == current) {
        err = app_timer_stop(backlight_fade_timer);
        APP_ERROR_CHECK(err);
    }
}

static void backlight_sleep_handler(void *ctx) {
    backlight_fade(BACKLIGHT_DEFAULT, BACKLIGHT_SLOW);
}

void backlight_init(uint8_t bkl_pwm) {

    ret_code_t err;

    // Configure the PWM2 instance
    app_pwm_config_t pwm_config = APP_PWM_DEFAULT_CONFIG_1CH(1000L, bkl_pwm);
    err = app_pwm_init(&PWM2, &pwm_config, NULL);
    APP_ERROR_CHECK(err);

    // Enable PWM2
    app_pwm_enable(&PWM2);

    // Set the initial duty cycle
    while ((err = app_pwm_channel_duty_set(&PWM2, 0,
                                           100 - BACKLIGHT_DEFAULT)) ==
           NRF_ERROR_BUSY);
    APP_ERROR_CHECK(err);

    // Create an app timer for fading
    err = app_timer_create(&backlight_fade_timer, APP_TIMER_MODE_REPEATED,
                           backlight_fade_handler);
    APP_ERROR_CHECK(err);

    // Create an app timer for fading after inactivity
    err = app_timer_create(&backlight_sleep_timer, APP_TIMER_MODE_SINGLE_SHOT,
                           backlight_sleep_handler);
    APP_ERROR_CHECK(err);

}

void backlight_fade(int target, unsigned int rate) {

    // Set parameters
    backlight_target = target;
    backlight_step = rate;

    // Execute first frame
    backlight_fade_handler(NULL);

    // Start the app timer @ 50Hz
    app_timer_start(backlight_fade_timer, APP_TIMER_TICKS(20), NULL);

}

void backlight_wake() {

    // Fade to active level
    backlight_fade(BACKLIGHT_ACTIVE, BACKLIGHT_FAST);

    // Start inactivity timer
    app_timer_stop(backlight_sleep_timer);
    app_timer_start(backlight_sleep_timer, APP_TIMER_TICKS(15000), NULL);

}
