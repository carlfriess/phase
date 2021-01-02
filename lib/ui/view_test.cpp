//
// Created by Carl Friess on 02/01/2021.
//

#include "gtest/gtest.h"
#include "phase-ui.h"

TEST(View, BackgroundColor) {

    uint8_t buf[sizeof(phase::ui::Color)];
    phase::ui::View root({{0, 0}, 1, 1});

    root.setBackgroundColor(phase::ui::red);
    root.render(buf, root.getFrame());
    for (size_t i = 0; i < sizeof(phase::ui::Color); ++i) {
        EXPECT_EQ(buf[i], phase::ui::red[i]);
    }

    root.setBackgroundColor(phase::ui::blue);
    root.render(buf, root.getFrame());
    for (size_t i = 0; i < sizeof(phase::ui::Color); ++i) {
        EXPECT_EQ(buf[i], phase::ui::blue[i]);
    }

}

TEST(View, Visibile) {

    uint8_t buf[sizeof(phase::ui::Color)];
    phase::ui::View root({{0, 0}, 1, 1});

    phase::ui::View child(root.getFrame());
    child.setBackgroundColor(phase::ui::white);
    root.addChildView(&child);

    root.render(buf, root.getFrame());
    for (size_t i = 0; i < sizeof(phase::ui::Color); ++i) {
        EXPECT_EQ(buf[i], phase::ui::white[i]);
    }

    child.setVisible(false);

    root.render(buf, root.getFrame());
    for (size_t i = 0; i < sizeof(phase::ui::Color); ++i) {
        EXPECT_EQ(buf[i], phase::ui::black[i]);
    }

}

TEST(View, Opaque) {

    uint8_t buf[sizeof(phase::ui::Color)];
    phase::ui::View root({{0, 0}, 1, 1});

    phase::ui::View child(root.getFrame());
    child.setBackgroundColor(phase::ui::white);
    root.addChildView(&child);

    root.render(buf, root.getFrame());
    for (size_t i = 0; i < sizeof(phase::ui::Color); ++i) {
        EXPECT_EQ(buf[i], phase::ui::white[i]);
    }

    child.setOpaque(false);

    root.render(buf, root.getFrame());
    for (size_t i = 0; i < sizeof(phase::ui::Color); ++i) {
        EXPECT_EQ(buf[i], phase::ui::black[i]);
    }

}

TEST(View, Children) {

    uint8_t buf[sizeof(phase::ui::Color)];
    phase::ui::View root({{0, 0}, 1, 1});

    phase::ui::View child1(root.getFrame());
    child1.setBackgroundColor(phase::ui::red);
    root.addChildView(&child1);

    phase::ui::View child2(root.getFrame());
    child2.setBackgroundColor(phase::ui::green);
    child1.addChildView(&child2);

    root.render(buf, root.getFrame());
    for (size_t i = 0; i < sizeof(phase::ui::Color); ++i) {
        EXPECT_EQ(buf[i], phase::ui::green[i]);
    }

    phase::ui::View child3(root.getFrame());
    child3.setBackgroundColor(phase::ui::blue);
    root.addChildView(&child3);

    root.render(buf, root.getFrame());
    for (size_t i = 0; i < sizeof(phase::ui::Color); ++i) {
        EXPECT_EQ(buf[i], phase::ui::blue[i]);
    }

}
