//
// Created by Carl Friess on 06/01/2021.
//

#include "datetime.h"

#include "app_timer.h"
#include "nrf_log.h"

#include "ui.h"


#define TICK_INTERVAL   60  // seconds


APP_TIMER_DEF(datetime_timer);

static time_t current_time = 0;
static time_t offset = 0;

static void datetime_tick(void *ctx) {

    // Increment time
    current_time += TICK_INTERVAL - offset;

    // Update UI
    ui_set_datetime(current_time);

}

static void datetime_single_shot_tick(void *ctx) {

    ret_code_t err;

    // Switch timer to repeated mode
    err = app_timer_create(&datetime_timer, APP_TIMER_MODE_REPEATED,
                           datetime_tick);
    APP_ERROR_CHECK(err);

    // Restart the timer at usual tick interval
    app_timer_start(datetime_timer, APP_TIMER_TICKS(TICK_INTERVAL * 1000),
                    NULL);
    APP_ERROR_CHECK(err);

    // Do the regular tick
    datetime_tick(ctx);

    // Reset the offest
    offset = 0;

}

void datetime_set(time_t time) {

    ret_code_t err;

    // Stop the timer if it is already running
    app_timer_stop(datetime_timer);

    // Set the time
    offset = time % TICK_INTERVAL;
    current_time = time;

    // Initialize the timer in single shot mode
    err = app_timer_create(&datetime_timer, APP_TIMER_MODE_SINGLE_SHOT,
                           datetime_single_shot_tick);
    APP_ERROR_CHECK(err);

    // Start in single shot mode until the beginning of the next tick interval
    err = app_timer_start(datetime_timer,
                          APP_TIMER_TICKS((TICK_INTERVAL - offset) * 1000),
                          NULL);
    APP_ERROR_CHECK(err);

    struct tm *time_struct = localtime(&time);
    NRF_LOG_INFO("Time set to %02d:%02d:%02d", time_struct->tm_hour,
                 time_struct->tm_min, time_struct->tm_sec);

}
