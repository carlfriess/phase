//
// Created by Carl Friess on 01/01/2021.
//

#include "phase-ui.h"

#include "util.h"

namespace phase {
namespace ui {

void ImageView::render(uint8_t *buffer, Frame region) const {
    imgcpy(buffer, region, img, frame);
}

}
}
