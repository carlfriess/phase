//
// Created by Carl Friess on 01/01/2021.
//

#include "ui.h"

#include "GC9A01.h"
#include "spi.h"
#include "phase-ui.h"

#define CHUNK_SIZE  8

extern const uint8_t background[];

static uint8_t buf1[240 * CHUNK_SIZE * 3];
static uint8_t buf2[240 * CHUNK_SIZE * 3];
static uint8_t *bufs[] = {buf1, buf2};

static inline void swap_bufs() {
    uint8_t *tmp = bufs[1];
    bufs[1] = bufs[0];
    bufs[0] = tmp;
}

phase::ui::View root({{0, 0}, 240, 240});

void ui_init(void) {

    root.setOpaque(false);

    // Add background image
    auto backgroundView = new phase::ui::ImageView(background, root.getFrame());
    root.addChildView(backgroundView);

    // Add a square
    auto square = new phase::ui::View({{120, 120}, 10, 10});
    square->setBackgroundColor(phase::ui::red);
    root.addChildView(square);

}

void ui_update(void) {
    phase::ui::Frame frame = {
            .origin = {0, 0},
            .width = 240,
            .height = CHUNK_SIZE,
    };
    GC9A01_start_write();
    for (; frame.origin.y < 240 + 1; frame.origin.y += CHUNK_SIZE) {
        if (frame.origin.y > 0) {
            spi_tx(bufs[0], sizeof(buf1));
        }
        if (frame.origin.y < 240) {
            root.render(bufs[1], frame);
        }
        swap_bufs();
        while (!spi_done());
    }
    GC9A01_finish_write();
}