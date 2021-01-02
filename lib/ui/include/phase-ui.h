//
// Created by Carl Friess on 01/01/2021.
//

#ifndef PHASE_PHASE_UI_H
#define PHASE_PHASE_UI_H

#include <cstdint>
#include <vector>

#define min(a, b)   ((a) < (b) ? (a) : (b))
#define max(a, b)   ((a) > (b) ? (a) : (b))

namespace phase {
namespace ui {

typedef int16_t Coord;

typedef uint8_t Color[3];

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
};

class View {
public:
    Frame frame;
    std::vector<View> subviews;
    bool opaque = true;
    Color background_color = {0x00, 0x00, 0x00};

    explicit View(Frame frame) : frame(frame) {};

    virtual void render(uint8_t *buffer, Frame frame) const;
};

class ImageView : View {
    uint8_t *img;

public:
    explicit ImageView(uint8_t *img, Frame frame) : View(frame), img(img) {};
};

}
}

#endif //PHASE_PHASE_UI_H
