//
// Created by Carl Friess on 01/01/2021.
//

#ifndef PHASE_UI_H
#define PHASE_UI_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void ui_init(void);
void ui_update(void);
void ui_set_time(uint8_t hr, uint8_t min);
void ui_set_date(const char *weekday, uint8_t day, const char *month);

#ifdef __cplusplus
}
#endif

#endif //PHASE_UI_H
