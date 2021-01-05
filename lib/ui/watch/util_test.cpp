//
// Created by Carl Friess on 05/01/2021.
//

#include "gtest/gtest.h"
#include "phase-ui-watch.h"

using namespace phase::ui;

TEST(HorizFitFrameCircle, Lower) {

    phase::ui::Frame circle = {
            .origin = {0, 0},
            .width = 100,
            .height = 100,
    };

    phase::ui::Frame frame = {
            .origin = {0, 60},
            .width = 100,
            .height = 20,
    };

    auto out = phase::ui::watch::clip_frame_to_fit_circle(frame, circle);

    EXPECT_EQ(out.origin.x, 10);
    EXPECT_EQ(out.width, 80);

}

TEST(HorizFitFrameCircle, Upper) {

    phase::ui::Frame circle = {
            .origin = {0, 0},
            .width = 100,
            .height = 100,
    };

    phase::ui::Frame frame = {
            .origin = {0, 20},
            .width = 100,
            .height = 20,
    };

    auto out = phase::ui::watch::clip_frame_to_fit_circle(frame, circle);

    EXPECT_EQ(out.origin.x, 10);
    EXPECT_EQ(out.width, 80);

}

TEST(HorizFitFrameCircle, Both) {

    phase::ui::Frame circle = {
            .origin = {0, 0},
            .width = 100,
            .height = 100,
    };

    phase::ui::Frame frame = {
            .origin = {0, 20},
            .width = 100,
            .height = 50,
    };

    auto out = phase::ui::watch::clip_frame_to_fit_circle(frame, circle);

    EXPECT_EQ(out.origin.x, 10);
    EXPECT_EQ(out.width, 80);

}

TEST(HorizFitFrameCircle, PartialRight) {

    phase::ui::Frame circle = {
            .origin = {0, 0},
            .width = 100,
            .height = 100,
    };

    phase::ui::Frame frame = {
            .origin = {15, 60},
            .width = 85,
            .height = 20,
    };

    auto out = phase::ui::watch::clip_frame_to_fit_circle(frame, circle);

    EXPECT_EQ(out.origin.x, 15);
    EXPECT_EQ(out.width, 75);

}

TEST(HorizFitFrameCircle, PartiaLeft) {

    phase::ui::Frame circle = {
            .origin = {0, 0},
            .width = 100,
            .height = 100,
    };

    phase::ui::Frame frame = {
            .origin = {0, 60},
            .width = 85,
            .height = 20,
    };

    auto out = phase::ui::watch::clip_frame_to_fit_circle(frame, circle);

    EXPECT_EQ(out.origin.x, 10);
    EXPECT_EQ(out.width, 75);

}