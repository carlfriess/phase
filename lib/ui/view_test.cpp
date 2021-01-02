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

TEST(View, Visible) {

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

TEST(View, DirtyView) {

    phase::ui::View view({{0, 0}, 1, 1});
    EXPECT_TRUE(view.isDirty());

    view.clean();
    EXPECT_FALSE(view.isDirty());

    view.setOpaque(true);
    EXPECT_FALSE(view.isDirty());
    view.setOpaque(false);
    EXPECT_TRUE(view.isDirty());
    view.clean();
    EXPECT_FALSE(view.isDirty());

    view.setVisible(true);
    EXPECT_FALSE(view.isDirty());
    view.setVisible(false);
    EXPECT_TRUE(view.isDirty());
    view.clean();
    EXPECT_FALSE(view.isDirty());

    view.setBackgroundColor(phase::ui::black);
    EXPECT_FALSE(view.isDirty());
    view.setBackgroundColor(phase::ui::white);
    EXPECT_TRUE(view.isDirty());
    view.clean();
    EXPECT_FALSE(view.isDirty());

}

TEST(View, DirtyChild) {

    phase::ui::View view({{0, 0}, 1, 1});
    phase::ui::View child({{0, 0}, 1, 1});
    EXPECT_TRUE(view.isDirty());

    view.clean();
    EXPECT_FALSE(view.isDirty());
    view.addChildView(&child);
    EXPECT_TRUE(view.isDirty());
    child.clean();
    EXPECT_TRUE(view.isDirty());
    view.clean();
    EXPECT_FALSE(view.isDirty());

    child.setBackgroundColor(phase::ui::red);
    EXPECT_TRUE(view.isDirty());
    view.clean();
    EXPECT_FALSE(view.isDirty());

}

TEST(View, DirtyFrame) {

    phase::ui::View view({{0, 0}, 3, 3});
    phase::ui::View child1({{0, 0}, 1, 1});
    phase::ui::View child2({{2, 2}, 1, 1});
    view.addChildView(&child1);
    view.addChildView(&child2);

    phase::ui::Frame dirty_frame = view.getDirtyFrame();
    EXPECT_TRUE(dirty_frame.contains(view.getFrame()));
    EXPECT_TRUE(view.getFrame().contains(dirty_frame));

    view.clean();
    dirty_frame = view.getDirtyFrame();
    EXPECT_EQ(dirty_frame.width, 0);
    EXPECT_EQ(dirty_frame.height, 0);

    child2.setBackgroundColor(phase::ui::red);
    dirty_frame = view.getDirtyFrame();
    EXPECT_TRUE(dirty_frame.contains(child2.getFrame()));
    EXPECT_TRUE(child2.getFrame().contains(dirty_frame));

    child1.setBackgroundColor(phase::ui::blue);
    child2.setBackgroundColor(phase::ui::blue);
    dirty_frame = view.getDirtyFrame();
    EXPECT_TRUE(dirty_frame.contains(child1.getFrame()));
    EXPECT_TRUE(dirty_frame.contains(child2.getFrame()));

}
