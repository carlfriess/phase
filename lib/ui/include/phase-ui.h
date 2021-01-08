//
// Created by Carl Friess on 01/01/2021.
//

#ifndef PHASE_PHASE_UI_H
#define PHASE_PHASE_UI_H

#include <cstdint>
#include <vector>
#include <string>

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
        Coord nX = std::max(origin.x, other.origin.x);
        Coord nY = std::max(origin.y, other.origin.y);
        Coord nW =
                std::min(origin.x + width, other.origin.x + other.width) - nX;
        Coord nH =
                std::min(origin.y + height, other.origin.y + other.height) - nY;
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
        Coord nX = std::min(origin.x, other.origin.x);
        Coord nY = std::min(origin.y, other.origin.y);
        Coord nW =
                std::max(origin.x + width, other.origin.x + other.width) - nX;
        Coord nH =
                std::max(origin.y + height, other.origin.y + other.height) - nY;
        return {
                .origin = {nX, nY},
                .width = nW,
                .height = nH,
        };
    }
};

class View {
public:
    explicit View(Frame frame) : frame(frame) {};

    virtual ~View() = default;

    virtual void render(uint8_t *buffer, Frame frame) const;

    Frame getFrame() const;

    void setVisible(bool on);

    void setOpaque(bool on);

    void setBackgroundColor(const Color &color);

    void addChildView(View *view);

    void removeChildView(View *view);

    bool isDirty() const;

    Frame getDirtyFrame() const;

    void clean();

protected:
    Frame frame;
    std::vector<View *> subviews;
    bool visible = true;
    bool opaque = true;
    Color background_color = {0x00, 0x00, 0x00};
    bool dirty = true;
};

class ImageView final : public View {
public:
    ImageView(const uint8_t *img, Frame frame) : View(frame), img(img) {};

    void render(uint8_t *buffer, Frame frame) const override;

    void setMask(const uint8_t *buffer);

private:
    const uint8_t *img;
    const uint8_t *mask = nullptr;
};

class TextView : public View {
public:
    enum TextAlignment {
        LEFT, CENTER, RIGHT,
    };

    TextView(const struct font *font, Frame frame) : View(frame), font(font) {};

    void render(uint8_t *buffer, Frame frame) const override;

    virtual size_t setText(const std::string str);

    virtual void setColor(const Color &color);

    virtual void setTextAlignment(enum TextAlignment setting);

protected:
    std::string text = "";
    const struct font *font;
    Color color = {0xFF, 0xFF, 0xFF};
    enum TextAlignment align = CENTER;
    Coord alignment_offset;
};

}
}

#endif //PHASE_PHASE_UI_H
