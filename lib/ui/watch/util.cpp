//
// Created by Carl Friess on 05/01/2021.
//

#include "phase-ui-watch.h"

#include <cassert>
#include <math.h>

namespace phase {
namespace ui {
namespace watch {

static Coord horizontal_margin(Coord y, Frame circle) {
    assert(circle.width == circle.height);
    Coord radius = circle.height / 2;
    Coord offset = circle.origin.y + radius - y;
    Coord len = sqrtf(static_cast<float>(radius * radius) -
                      static_cast<float>(offset * offset));
    return radius - len;
}

Frame clip_frame_to_fit_circle(Frame frame, const Frame &circle) {

    // Calculate required margin at the top and bottom edge of the frame;
    Coord margin = horizontal_margin(frame.origin.y + frame.height, circle);
    margin = max(margin, horizontal_margin(frame.origin.y, circle));

    // Adjust horizontal bounds
    if (circle.origin.x + margin > frame.origin.x) {
        Coord left_margin = margin - (frame.origin.x - circle.origin.x);
        frame.origin.x += left_margin;
        frame.width -= left_margin;
    }
    if (frame.origin.x + frame.width >
        circle.origin.x + circle.width - margin) {
        frame.width -= margin - ((circle.origin.x + circle.width) -
                                 (frame.origin.x + frame.width));
    }

    return frame;
}

}
}
}
