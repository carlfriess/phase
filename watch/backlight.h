//
// Created by Carl Friess on 07/05/2022.
//

#ifndef PHASE_BACKLIGHT_H
#define PHASE_BACKLIGHT_H

#include <stdint.h>

#define BACKLIGHT_DEFAULT   10
#define BACKLIGHT_ACTIVE    60

#define BACKLIGHT_FAST  10
#define BACKLIGHT_SLOW  1

void backlight_init(uint8_t bkl_pwm);

void backlight_fade(int target, unsigned int rate);

void backlight_wake(void);

#endif //PHASE_BACKLIGHT_H
