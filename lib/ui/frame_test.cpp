//
// Created by Carl Friess on 02/01/2021.
//

#include "gtest/gtest.h"
#include "phase-ui.h"

TEST(FrameTest, Area) {

    phase::ui::Frame frame = {
            .origin = {0, 0},
            .width = 64,
            .height = 64,
    };

    ASSERT_EQ(frame.area(), 64 * 64);

}
