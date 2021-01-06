//
// Created by Carl Friess on 01/01/2021.
//

#include "phase-ui.h"

#include "util.h"

namespace phase {
namespace ui {

void ImageView::render(uint8_t *buffer, Frame region) const {
    if (mask) {
        imgcpy_mask(buffer, region, mask, img, frame);
    } else {
        imgcpy(buffer, region, img, frame);
    }
}

void ImageView::setMask(const uint8_t *buffer) {
    if (mask != buffer) {
        dirty = true;
        mask = buffer;
    }
}

}
}
