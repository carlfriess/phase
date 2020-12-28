#include <math.h>

#include "nrf_gpio.h"
#include "nrf_delay.h"
#include "boards.h"
#include "app_error.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "spi.h"
#include "GC9A01.h"

#define GC9A01_SCL  SPI_SCK_PIN
#define GC9A01_SDA  SPI_MOSI_PIN
#define GC9A01_RES  3   // P0.03
#define GC9A01_DC   4   // P0.04
#define GC9A01_CS   SPI_SS_PIN

void GC9A01_set_reset(uint8_t val) {
    nrf_gpio_pin_write(GC9A01_RES, val);
}

void GC9A01_set_data_command(uint8_t val) {
    nrf_gpio_pin_write(GC9A01_DC, val);
}

void GC9A01_set_chip_select(uint8_t val) {
    nrf_gpio_pin_write(GC9A01_CS, val);
}

void GC9A01_spi_tx(uint8_t *data, size_t len) {
    spi_tx(data, len);
    while (!spi_done()) {
        __WFE();
    }
}

void GC9A01_delay(uint16_t ms) {
    nrf_delay_ms(ms);
}

int main(void)
{
    bsp_board_init(BSP_INIT_LEDS);

    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    nrf_gpio_cfg_output(GC9A01_RES);
    nrf_gpio_cfg_output(GC9A01_DC);
    nrf_gpio_cfg_output(GC9A01_CS);

    spi_init(GC9A01_SCL, GC9A01_SDA);

    NRF_LOG_INFO("SPI example started.");

    GC9A01_init();

    while (1) {

        uint8_t color[3];

        // Triangle
        color[0] = 0xFF;
        color[1] = 0xFF;
        for (int x = 0; x < 240; x++) {
            for (int y = 0; y < 240; y++) {
                if (x < y) {
                    color[2] = 0xFF;
                } else {
                    color[2] = 0x00;
                }
                if (x == 0 && y == 0) {
                    GC9A01_write(color, sizeof(color));
                } else {
                    GC9A01_write_continue(color, sizeof(color));
                }
            }
        }

        nrf_delay_ms(1000);

        // Rainbow
        float frequency = 0.026;
        for (int x = 0; x < 240; x++) {
            color[0] = sin(frequency*x + 0) * 127 + 128;
            color[1] = sin(frequency*x + 2) * 127 + 128;
            color[2] = sin(frequency*x + 4) * 127 + 128;
            for (int y = 0; y < 240; y++) {
                if (x == 0 && y == 0) {
                    GC9A01_write(color, sizeof(color));
                } else {
                    GC9A01_write_continue(color, sizeof(color));
                }
            }
        }

        nrf_delay_ms(1000);

        // Checkerboard
        for (int x = 0; x < 240; x++) {
            color[0] = sin(frequency*x + 0) * 127 + 128;
            color[1] = sin(frequency*x + 2) * 127 + 128;
            color[2] = sin(frequency*x + 4) * 127 + 128;
            for (int y = 0; y < 240; y++) {
                if ((x / 10) % 2 ==  (y / 10) % 2) {
                    color[0] = 0xFF;
                    color[1] = 0xFF;
                    color[2] = 0xFF;
                } else {
                    color[0] = 0x00;
                    color[1] = 0x00;
                    color[2] = 0x00;
                }
                if (x == 0 && y == 0) {
                    GC9A01_write(color, sizeof(color));
                } else {
                    GC9A01_write_continue(color, sizeof(color));
                }
            }
        }

        nrf_delay_ms(1000);

        // Swiss flag
        color[0] = 0xFF;
        for (int x = 0; x < 240; x++) {
            for (int y = 0; y < 240; y++) {
                if ((x >= 1*48 && x < 4*48 && y >= 2*48 && y < 3*48) ||
                    (x >= 2*48 && x < 3*48 && y >= 1*48 && y < 4*48)) {
                    color[1] = 0xFF;
                    color[2] = 0xFF;
                } else {
                    color[1] = 0x00;
                    color[2] = 0x00;
                }
                if (x == 0 && y == 0) {
                    GC9A01_write(color, sizeof(color));
                } else {
                    GC9A01_write_continue(color, sizeof(color));
                }
            }
        }

        nrf_delay_ms(1000);

    }
    
}
