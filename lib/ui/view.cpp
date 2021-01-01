//
// Created by Carl Friess on 01/01/2021.
//

#include "phase-ui.h"

#include "util.h"

namespace phase {
namespace ui {

void View::render(uint8_t *buffer, Frame region) const {

    if (opaque) {
        fill_color(buffer, background_color, region.area());
    }

    for (const View &view : subviews) {
        view.render(buffer, frame);
    }

}

}
}
