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

class MultilineTextView final : public TextView {
public:
    MultilineTextView(const struct font *font, Frame frame) : TextView(font,
                                                                       frame) {};

    ~MultilineTextView() override;

    void render(uint8_t *buffer, Frame frame) const override;

    size_t setText(const char *str) override;

    void setColor(const Color &color) override;

    void setTextAlignment(enum TextAlignment setting) override;
};

}
}
}

#endif //PHASE_PHASE_UI_WATCH_H
