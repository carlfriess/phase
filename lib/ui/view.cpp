//
// Created by Carl Friess on 01/01/2021.
//

#include "phase-ui.h"

#include <cassert>
#include <cstring>

#include "util.h"

namespace phase {
namespace ui {

void View::render(uint8_t *buffer, Frame region) const {

    // Fill the background if this view is opaque
    if (opaque) {
        fill_frame_color(buffer, region, background_color, frame);
    }

    // Call render on all visible subviews
    for (View *view : subviews) {
        if (view->frame.overlaps(region) && view->visible) {
            view->render(buffer, region);
        }
    }

}

Frame View::getFrame() const {
    return frame;
}

void View::setVisible(bool on) {
    View::visible = on;
}

void View::setOpaque(bool on) {
    View::opaque = on;
}

void View::setBackgroundColor(const Color &color) {
    memcpy(background_color, color, sizeof(Color));
}

void View::addChildView(View *view) {
    assert(frame.contains(view->frame));
    subviews.push_back(view);
}

}
}
