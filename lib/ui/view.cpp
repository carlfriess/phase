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
    // TODO: Don't render subviews that are covered by other subviews
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
    if (visible != on) {
        dirty = true;
    }
    visible = on;
}

void View::setOpaque(bool on) {
    if (opaque != on) {
        dirty = true;
    }
    opaque = on;
}

void View::setBackgroundColor(const Color &color) {
    if (memcmp(background_color, color, sizeof(Color))) {
        dirty = true;
    }
    memcpy(background_color, color, sizeof(Color));
}

void View::addChildView(View *view) {
    assert(frame.contains(view->frame));
    dirty = true;
    subviews.push_back(view);
}

bool View::isDirty() const {

    // Check if this view is dirty
    if (dirty) {
        return true;
    }

    // Check if any children are dirty
    for (View *view : subviews) {
        if (view->dirty) {
            return true;
        }
    }

    return false;
}

Frame View::getDirtyFrame() const {

    Frame df = {
            .origin = {0, 0},
            .width  = 0,
            .height = 0,
    };
    bool valid = false;

    // Check if this view is dirty
    if (dirty) {
        return frame;
    }

    // Check if any children are dirty
    for (View *view : subviews) {
        if (view->dirty) {
            if (!valid) {
                df = view->getDirtyFrame();
                valid = true;
            } else {
                df = df.expand(view->getDirtyFrame());
            }
        }
    }

    return df;
}

void View::clean() {
    dirty = false;
    for (View *view : subviews) {
        view->dirty = false;
    }
}

}
}
