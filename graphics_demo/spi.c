//
// Created by Carl Friess on 27/12/2020.
//

#include "spi.h"

#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrfx_spim.h"
#include "nrfx_ppi.h"
#include "nrfx_timer.h"

#define SPI_INSTANCE  0     // SPI instance index
static const nrfx_spim_t spi = NRFX_SPIM_INSTANCE(SPI_INSTANCE);

#define TIMER_INSTANCE  1   // Timer instance index
static const nrfx_timer_t timer = NRFX_TIMER_INSTANCE(TIMER_INSTANCE);

static nrf_ppi_channel_t ppi_channel_spi;

static bool spi_xfer_done = true;

static void spi_event_handler(const nrfx_spim_evt_t *p_event, void *pVoid) {
    if (p_event->type == NRFX_SPIM_EVENT_DONE) {
        // Nothing left to do...
    }
}

static void timer_event_handler(nrf_timer_event_t event_type, void *p_context) {
    if (event_type == NRF_TIMER_EVENT_COMPARE0) {

        // Set the flag
        spi_xfer_done = true;

        // Disable the shortcut and PPI channel to suppress the SPI end event
        nrf_spim_shorts_disable(spi.p_reg, NRF_SPIM_SHORT_END_START_MASK);
        nrf_ppi_channel_disable(ppi_channel_spi);

        // Abort the suspended transfer
        nrfx_spim_abort(&spi);

    }
}

void spi_init(uint8_t sck, uint8_t mosi) {

    ret_code_t err;
    uint32_t spi_end_event;
    uint32_t timer_cc_event;
    uint32_t timer_count_task;
    uint32_t spi_suspend_task;
    nrf_ppi_channel_t ppi_channel_timer;

    // Initialize SPI master
    nrfx_spim_config_t spi_config = NRFX_SPIM_DEFAULT_CONFIG;
    spi_config.sck_pin = sck;
    spi_config.mosi_pin = mosi;
    spi_config.frequency = NRF_SPIM_FREQ_8M;
    err = nrfx_spim_init(&spi, &spi_config, spi_event_handler, NULL);
    APP_ERROR_CHECK(err);

    // Initialize timer
    nrfx_timer_config_t timer_cfg = NRFX_TIMER_DEFAULT_CONFIG;
    timer_cfg.mode = NRF_TIMER_MODE_LOW_POWER_COUNTER,
    timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_32,
    err = nrfx_timer_init(&timer, &timer_cfg, timer_event_handler);
    APP_ERROR_CHECK(err);

    // Get event addresses
    spi_end_event = nrfx_spim_end_event_get(&spi);
    timer_cc_event = nrfx_timer_event_address_get(&timer,
                                                  NRF_TIMER_EVENT_COMPARE0);

    // Get task addresses
    timer_count_task = nrfx_timer_task_address_get(&timer,
                                                   NRF_TIMER_TASK_COUNT);
    spi_suspend_task = nrf_spim_task_address_get(spi.p_reg,
                                                 NRF_SPIM_TASK_SUSPEND);

    // Allocate PPI channels
    err = nrfx_ppi_channel_alloc(&ppi_channel_spi);
    APP_ERROR_CHECK(err);
    err = nrfx_ppi_channel_alloc(&ppi_channel_timer);
    APP_ERROR_CHECK(err);

    // Configure PPI to increment timer on SPI end event
    err = nrfx_ppi_channel_assign(ppi_channel_spi, spi_end_event,
                                  timer_count_task);
    APP_ERROR_CHECK(err);

    // Configure PPI to suspend the SPI transfer when the timer clears
    err = nrfx_ppi_channel_assign(ppi_channel_timer, timer_cc_event,
                                  spi_suspend_task);
    APP_ERROR_CHECK(err);
    err = nrfx_ppi_channel_enable(ppi_channel_timer);
    APP_ERROR_CHECK(err);

    // Enable the timer
    nrfx_timer_enable(&timer);

}

void spi_tx(const uint8_t *data, size_t len) {

    ret_code_t err;

    // Clear flag
    spi_xfer_done = false;

    // Configure timer to stop SPI after appropriate number of transfers
    nrfx_timer_extended_compare(&timer, NRF_TIMER_CC_CHANNEL0, len,
                                NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

    // Enable the shortcut causing the SPI transfer to loop and ppi channel used to count iterations
    nrf_spim_shorts_enable(spi.p_reg, NRF_SPIM_SHORT_END_START_MASK);
    nrf_ppi_channel_enable(ppi_channel_spi);

    // Start the transfer in repeat mode
    nrfx_spim_xfer_desc_t xfer = NRFX_SPIM_XFER_TX(data, 1);
    err = nrfx_spim_xfer(&spi, &xfer, NRFX_SPIM_FLAG_REPEATED_XFER |
                                      NRFX_SPIM_FLAG_TX_POSTINC |
                                      NRFX_SPIM_FLAG_NO_XFER_EVT_HANDLER);
    APP_ERROR_CHECK(err);

}

bool spi_done(void) {
    return spi_xfer_done;
}
