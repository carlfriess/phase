//
// Created by Carl Friess on 02/01/2021.
//

#include "gtest/gtest.h"
#include "phase-ui.h"

TEST(Frame, Area) {

    phase::ui::Frame frame = {
            .origin = {0, 0},
            .width = 64,
            .height = 64,
    };

    EXPECT_EQ(frame.area(), 64 * 64);

}

TEST(Frame, Overlaps) {

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

    EXPECT_FALSE(frame1.overlaps(frame2));
    EXPECT_FALSE(frame2.overlaps(frame1));

    frame2.origin.x = 10;

    EXPECT_FALSE(frame1.overlaps(frame2));
    EXPECT_FALSE(frame2.overlaps(frame1));

    frame2.origin.x = 0;

    EXPECT_FALSE(frame1.overlaps(frame2));
    EXPECT_FALSE(frame2.overlaps(frame1));

    frame2.origin.y = 5;

    EXPECT_TRUE(frame1.overlaps(frame2));
    EXPECT_TRUE(frame2.overlaps(frame1));

    frame2.origin.y = 0;

    EXPECT_TRUE(frame1.overlaps(frame2));
    EXPECT_TRUE(frame2.overlaps(frame1));

}

TEST(Frame, Contains) {

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

    EXPECT_FALSE(frame1.contains(frame2));
    EXPECT_FALSE(frame2.contains(frame1));

    frame2.origin.x = 5;

    EXPECT_FALSE(frame1.contains(frame2));
    EXPECT_FALSE(frame2.contains(frame1));

    frame2.origin.x = 0;

    EXPECT_FALSE(frame1.contains(frame2));
    EXPECT_FALSE(frame2.contains(frame1));

    frame2.origin.y = 0;

    EXPECT_TRUE(frame1.contains(frame2));
    EXPECT_TRUE(frame2.contains(frame1));

    frame2.width = 5;
    frame2.height = 5;

    EXPECT_TRUE(frame1.contains(frame2));
    EXPECT_FALSE(frame2.contains(frame1));

    frame2.origin.x = 1;
    frame2.origin.y = 1;

    EXPECT_TRUE(frame1.contains(frame2));
    EXPECT_FALSE(frame2.contains(frame1));

}

TEST(Frame, Overlap) {

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
    EXPECT_EQ(overlap.origin.x, 5);
    EXPECT_EQ(overlap.origin.y, 5);
    EXPECT_EQ(overlap.width, 5);
    EXPECT_EQ(overlap.height, 5);
    overlap = frame2.overlap(frame1);
    EXPECT_EQ(overlap.origin.x, 5);
    EXPECT_EQ(overlap.origin.y, 5);
    EXPECT_EQ(overlap.width, 5);
    EXPECT_EQ(overlap.height, 5);

    frame2.origin.x = -5;

    overlap = frame1.overlap(frame2);
    EXPECT_EQ(overlap.origin.x, 0);
    EXPECT_EQ(overlap.origin.y, 5);
    EXPECT_EQ(overlap.width, 5);
    EXPECT_EQ(overlap.height, 5);
    overlap = frame2.overlap(frame1);
    EXPECT_EQ(overlap.origin.x, 0);
    EXPECT_EQ(overlap.origin.y, 5);
    EXPECT_EQ(overlap.width, 5);
    EXPECT_EQ(overlap.height, 5);

    frame2.origin.x = 1;
    frame2.origin.y = 1;
    frame2.width = 6;
    frame2.height = 6;

    overlap = frame1.overlap(frame2);
    EXPECT_EQ(overlap.origin.x, 1);
    EXPECT_EQ(overlap.origin.y, 1);
    EXPECT_EQ(overlap.width, 6);
    EXPECT_EQ(overlap.height, 6);
    overlap = frame2.overlap(frame1);
    EXPECT_EQ(overlap.origin.x, 1);
    EXPECT_EQ(overlap.origin.y, 1);
    EXPECT_EQ(overlap.width, 6);
    EXPECT_EQ(overlap.height, 6);

    overlap = frame1.overlap(frame1);
    EXPECT_EQ(overlap.origin.x, 0);
    EXPECT_EQ(overlap.origin.y, 0);
    EXPECT_EQ(overlap.width, 10);
    EXPECT_EQ(overlap.height, 10);

}

TEST(Frame, Expand) {

    phase::ui::Frame frame1 = {
            .origin = {10, 10},
            .width = 5,
            .height = 5,
    };

    phase::ui::Frame frame2 = {
            .origin = {20, 20},
            .width = 10,
            .height = 5,
    };

    phase::ui::Frame expanded = {
            .origin = {10, 10},
            .width = 20,
            .height = 15,
    };

    ASSERT_TRUE(frame1.expand(frame2).contains(expanded));
    ASSERT_TRUE(expanded.contains(frame1.expand(frame2)));
    ASSERT_TRUE(frame2.expand(frame1).contains(expanded));
    ASSERT_TRUE(expanded.contains(frame2.expand(frame1)));

}
