//
// Created by Carl Friess on 05/01/2021.
//

#include "phase-ui-watch.h"

#include <cstring>

#include "fontem.h"

namespace phase {
namespace ui {
namespace watch {

void MultilineTextView::render(uint8_t *buffer, Frame frame) const {
    View::render(buffer, frame);
}

size_t MultilineTextView::setText(const std::string str) {

    // Deallocate all previous views
    for (View *view : subviews) {
        delete view;
    }
    subviews.clear();

    size_t offset = 0;
    Frame sub_frame = frame;
    sub_frame.height = font->height;

    while (offset < str.length()) {

        // Check if the next line will fit in the frame
        if (!frame.contains(sub_frame)) {
            break;
        }

        // Clip frame to fit in circular display area
        sub_frame = clip_frame_to_fit_circle(sub_frame, display_frame);

        // Create view for next line
        auto view = new TextView(font, sub_frame);
        view->setOpaque(false);
        offset += view->setText(str.substr(offset));
        addChildView(view);

        // Move sub-frame
        sub_frame.origin.y += sub_frame.height;

    }

    return offset;
}

void MultilineTextView::setColor(const Color &color) {
    for (View *view : subviews) {
        dynamic_cast<TextView *>(view)->setColor(color);
    }}

void MultilineTextView::setTextAlignment(TextView::TextAlignment setting) {
    for (View *view : subviews) {
        dynamic_cast<TextView *>(view)->setTextAlignment(setting);
    }
}

MultilineTextView::~MultilineTextView() {
    for (View *view : subviews) {
        delete view;
    }
}

}
}
}