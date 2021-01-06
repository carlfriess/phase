//
// Created by Carl Friess on 05/01/2021.
//

#include "phase-ui-watch.h"

#include "fontem.h"

namespace phase {
namespace ui {
namespace watch {

NotificationView::NotificationView(const char *title,
                                   const struct font *title_font,
                                   const char *body,
                                   const struct font *body_font, Frame frame)
        : View(frame) {

    setOpaque(false);

    Frame icon_frame{};
    icon_frame.width = 60;
    icon_frame.height = 60;
    icon_frame.origin.x =
            frame.origin.x + (frame.width / 2) - (icon_frame.width / 2);
    icon_frame.origin.y = frame.origin.y;
    auto icon_view = new View(icon_frame);

    Frame content_frame{};
    content_frame.origin.x = frame.origin.x;
    content_frame.origin.y = frame.origin.y + (icon_frame.height / 2);
    content_frame.width = frame.width;
    content_frame.height = frame.height - (icon_frame.height / 2);
    auto content_view = new View(content_frame);
    content_view->setBackgroundColor(white);
    addChildView(content_view);

    Frame text_frame{};
    text_frame.origin.x = content_frame.origin.x;
    text_frame.origin.y = content_frame.origin.y + (icon_frame.height / 2) + 4;
    text_frame.width = content_frame.width;
    text_frame.height = title_font->height;
    auto title_view = new TextView(title_font, text_frame);
    title_view->setText(title);
    title_view->setColor(black);
    title_view->setOpaque(false);
    content_view->addChildView(title_view);

    text_frame.origin.y += text_frame.height;
    text_frame.height = frame.origin.y + frame.height - text_frame.origin.y;
    auto body_view = new MultilineTextView(body_font, text_frame);
    body_view->setText(body);
    body_view->setColor(black);
    body_view->setOpaque(false);
    content_view->addChildView(body_view);

    addChildView(icon_view);

}

NotificationView::~NotificationView() {
    for (View *view : subviews) {
        delete view;
    }
}

}
}
}