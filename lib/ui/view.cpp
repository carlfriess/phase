//
// Created by Carl Friess on 01/01/2021.
//

#include "phase-ui.h"

#include "util.h"

namespace phase {
namespace ui {

void View::render(uint8_t *buffer, Frame region) const {

    if (opaque) {
        fill_frame_color(buffer, region, background_color, frame);
    }

    for (const View &view : subviews) {
        view.render(buffer, frame);
    }

}

}
}
