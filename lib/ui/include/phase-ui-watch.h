//
// Created by Carl Friess on 05/01/2021.
//

#ifndef PHASE_PHASE_UI_WATCH_H
#define PHASE_PHASE_UI_WATCH_H

#include "phase-ui.h"

namespace phase {
namespace ui {
namespace watch {

constexpr Frame display_frame = {
        .origin = {0, 0},
        .width = 240,
        .height = 240,
};

Frame clip_frame_to_fit_circle(Frame frame, const Frame &circle);

}
}
}

#endif //PHASE_PHASE_UI_WATCH_H
