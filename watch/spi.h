//
// Created by Carl Friess on 27/12/2020.
//

#ifndef PHASE_DISPLAY_H
#define PHASE_DISPLAY_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void spi_init(uint8_t sck, uint8_t mosi);

void spi_tx(const uint8_t *data, size_t len);

bool spi_done(void);

#ifdef __cplusplus
}
#endif

#endif //PHASE_DISPLAY_H
