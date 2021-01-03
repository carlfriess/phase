//
// Created by Carl Friess on 01/01/2021.
//

#ifndef PHASE_PHASE_UI_H
#define PHASE_PHASE_UI_H

#include <cstdint>
#include <vector>

#define min(a, b)   ((a) < (b) ? (a) : (b))
#define max(a, b)   ((a) > (b) ? (a) : (b))

// Forward declaration of fontem type
struct font;

namespace phase {
namespace ui {

typedef int16_t Coord;

typedef uint8_t Color[3];

constexpr Color white = {0xFF, 0xFF, 0xFF};
constexpr Color red = {0xFF, 0x00, 0x00};
constexpr Color green = {0x00, 0xFF, 0x00};
constexpr Color blue = {0x00, 0x00, 0xFF};
constexpr Color black = {0x00, 0x00, 0x00};

struct Point {
    Coord x, y;
};

struct Frame {
    Point origin;
    Coord width, height;

    inline size_t area() const {
        return width * height;
    }

    inline bool overlaps(const Frame &other) const {
        return !(origin.x + width <= other.origin.x ||
                 origin.y + height <= other.origin.y ||
                 other.origin.x + other.width <= origin.x ||
                 other.origin.y + other.height <= origin.y);
    }

    inline Frame overlap(const Frame &other) const {
        Coord nX = max(origin.x, other.origin.x);
        Coord nY = max(origin.y, other.origin.y);
        Coord nW = min(origin.x + width, other.origin.x + other.width) - nX;
        Coord nH = min(origin.y + height, other.origin.y + other.height) - nY;
        return {
                .origin = {nX, nY},
                .width = nW,
                .height = nH,
        };
    }

    inline bool contains(const Frame &other) const {
        return origin.x <= other.origin.x &&
               origin.y <= other.origin.y &&
               origin.x + width >= other.origin.x + other.width &&
               origin.y + height >= other.origin.y + other.height;
    }

    inline Frame expand(const Frame &other) const {
        Coord nX = min(origin.x, other.origin.x);
        Coord nY = min(origin.y, other.origin.y);
        Coord nW = max(origin.x + width, other.origin.x + other.width) - nX;
        Coord nH = max(origin.y + height, other.origin.y + other.height) - nY;
        return {
                .origin = {nX, nY},
                .width = nW,
                .height = nH,
        };
    }
};

class View {
protected:
    Frame frame;
    std::vector<View *> subviews;
    bool visible = true;
    bool opaque = true;
    Color background_color = {0x00, 0x00, 0x00};
    bool dirty = true;

public:

    explicit View(Frame frame) : frame(frame) {};

    virtual void render(uint8_t *buffer, Frame frame) const;

    Frame getFrame() const;

    void setVisible(bool on);

    void setOpaque(bool on);

    void setBackgroundColor(const Color &color);

    void addChildView(View *view);

    bool isDirty() const;

    Frame getDirtyFrame() const;

    void clean();
};

class ImageView final : public View {
    const uint8_t *img;

public:
    explicit ImageView(const uint8_t *img, Frame frame) : View(frame),
                                                          img(img) {};

    void render(uint8_t *buffer, Frame frame) const override;
};

class TextView final : public View {

public:
    enum TextAlignment {
        LEFT, CENTER, RIGHT,
    };

private:
    char *text = nullptr;
    const struct font *font;
    Color color = {0xFF, 0xFF, 0xFF};
    enum TextAlignment align = CENTER;
    Coord alignment_offset;

public:
    explicit TextView(const struct font *font, Frame frame) : View(frame),
                                                              font(font) {};

    void render(uint8_t *buffer, Frame frame) const override;

    size_t setText(const char *str);

    void setColor(const Color &color);

    void setTextAlignment(enum TextAlignment setting);
};

}
}

#endif //PHASE_PHASE_UI_H
