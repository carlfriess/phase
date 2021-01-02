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

TEST(FrameTest, Overlaps) {

    phase::ui::Frame frame1 = {
            .origin = {0, 0},
            .width = 10,
            .height = 10,
    };

    phase::ui::Frame frame2 = {
            .origin = {15, 10},
            .width = 10,
            .height = 10,
    };

    ASSERT_FALSE(frame1.overlaps(frame2));
    ASSERT_FALSE(frame2.overlaps(frame1));

    frame2.origin.x = 10;

    ASSERT_FALSE(frame1.overlaps(frame2));
    ASSERT_FALSE(frame2.overlaps(frame1));

    frame2.origin.x = 0;

    ASSERT_FALSE(frame1.overlaps(frame2));
    ASSERT_FALSE(frame2.overlaps(frame1));

    frame2.origin.y = 5;

    ASSERT_TRUE(frame1.overlaps(frame2));
    ASSERT_TRUE(frame2.overlaps(frame1));

    frame2.origin.y = 0;

    ASSERT_TRUE(frame1.overlaps(frame2));
    ASSERT_TRUE(frame2.overlaps(frame1));

}

TEST(FrameTest, Contains) {

    phase::ui::Frame frame1 = {
            .origin = {0, 0},
            .width = 10,
            .height = 10,
    };

    phase::ui::Frame frame2 = {
            .origin = {15, 5},
            .width = 10,
            .height = 10,
    };

    ASSERT_FALSE(frame1.contains(frame2));
    ASSERT_FALSE(frame2.contains(frame1));

    frame2.origin.x = 5;

    ASSERT_FALSE(frame1.contains(frame2));
    ASSERT_FALSE(frame2.contains(frame1));

    frame2.origin.x = 0;

    ASSERT_FALSE(frame1.contains(frame2));
    ASSERT_FALSE(frame2.contains(frame1));

    frame2.origin.y = 0;

    ASSERT_TRUE(frame1.contains(frame2));
    ASSERT_TRUE(frame2.contains(frame1));

    frame2.width = 5;
    frame2.height = 5;

    ASSERT_TRUE(frame1.contains(frame2));
    ASSERT_FALSE(frame2.contains(frame1));

    frame2.origin.x = 1;
    frame2.origin.y = 1;

    ASSERT_TRUE(frame1.contains(frame2));
    ASSERT_FALSE(frame2.contains(frame1));

}

TEST(FrameTest, Overlap) {

    phase::ui::Frame frame1 = {
            .origin = {0, 0},
            .width = 10,
            .height = 10,
    };

    phase::ui::Frame frame2 = {
            .origin = {5, 5},
            .width = 10,
            .height = 10,
    };

    phase::ui::Frame overlap = frame1.overlap(frame2);
    ASSERT_EQ(overlap.origin.x, 5);
    ASSERT_EQ(overlap.origin.y, 5);
    ASSERT_EQ(overlap.width, 5);
    ASSERT_EQ(overlap.height, 5);
    overlap = frame2.overlap(frame1);
    ASSERT_EQ(overlap.origin.x, 5);
    ASSERT_EQ(overlap.origin.y, 5);
    ASSERT_EQ(overlap.width, 5);
    ASSERT_EQ(overlap.height, 5);

    frame2.origin.x = -5;

    overlap = frame1.overlap(frame2);
    ASSERT_EQ(overlap.origin.x, 0);
    ASSERT_EQ(overlap.origin.y, 5);
    ASSERT_EQ(overlap.width, 5);
    ASSERT_EQ(overlap.height, 5);
    overlap = frame2.overlap(frame1);
    ASSERT_EQ(overlap.origin.x, 0);
    ASSERT_EQ(overlap.origin.y, 5);
    ASSERT_EQ(overlap.width, 5);
    ASSERT_EQ(overlap.height, 5);

    frame2.origin.x = 1;
    frame2.origin.y = 1;
    frame2.width = 6;
    frame2.height = 6;

    overlap = frame1.overlap(frame2);
    ASSERT_EQ(overlap.origin.x, 1);
    ASSERT_EQ(overlap.origin.y, 1);
    ASSERT_EQ(overlap.width, 6);
    ASSERT_EQ(overlap.height, 6);
    overlap = frame2.overlap(frame1);
    ASSERT_EQ(overlap.origin.x, 1);
    ASSERT_EQ(overlap.origin.y, 1);
    ASSERT_EQ(overlap.width, 6);
    ASSERT_EQ(overlap.height, 6);

    overlap = frame1.overlap(frame1);
    ASSERT_EQ(overlap.origin.x, 0);
    ASSERT_EQ(overlap.origin.y, 0);
    ASSERT_EQ(overlap.width, 10);
    ASSERT_EQ(overlap.height, 10);

}
