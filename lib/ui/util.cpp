//
// Created by Carl Friess on 01/01/2021.
//

#include "util.h"

#include <cstring>

namespace phase {
namespace ui {

void fill_color(uint8_t *dest, const Color color, size_t num) {
    uint8_t *limit = dest + (num * sizeof(Color));
    for (uint8_t *ptr = dest; ptr < limit; ptr += sizeof(Color)) {
        memcpy(ptr, color, sizeof(Color));
    }
}

void fill_frame_color(uint8_t *dst, const Frame &dst_frame, const Color color,
                      const Frame &fill_frame) {

    if (!dst_frame.overlaps(fill_frame)) {
        return;
    }

    // Calculate steps for buffer
    size_t dst_step = dst_frame.width * sizeof(Color);

    // Adjust buffer pointer for vertical offset
    Coord y_offset = fill_frame.origin.y - dst_frame.origin.y;
    if (y_offset > 0) {
        dst += y_offset * dst_step;
    }

    // Adjust buffer pointer for horizontal offset
    Coord x_offset = fill_frame.origin.x - dst_frame.origin.x;
    if (x_offset > 0) {
        dst += x_offset * sizeof(Color);
    }

    // Calculate overlap
    Frame overlap = dst_frame.overlap(fill_frame);

    for (Coord row = 0; row < overlap.height; row++) {
        fill_color(dst, color, overlap.width);
        dst += dst_step;
    }

}

void imgcpy(uint8_t *dst, const Frame &dst_frame, const uint8_t *img,
            const Frame &img_frame) {

    if (!dst_frame.overlaps(img_frame)) {
        return;
    }

    // Calculate steps for each buffer
    size_t dst_step = dst_frame.width * sizeof(Color);
    size_t img_step = img_frame.width * sizeof(Color);

    // Adjust buffer pointers for vertical offset
    Coord y_offset = img_frame.origin.y - dst_frame.origin.y;
    if (y_offset > 0) {
        dst += y_offset * dst_step;
    } else {
        img += (-y_offset) * img_step;
    }

    // Adjust buffer pointers for horizontal offset
    Coord x_offset = img_frame.origin.x - dst_frame.origin.x;
    if (x_offset > 0) {
        dst += x_offset * sizeof(Color);
    } else {
        img += (-x_offset) * sizeof(Color);
    }

    // Calculate number of bytes that need to be copied per row
    Frame overlap = dst_frame.overlap(img_frame);
    size_t n = overlap.width * sizeof(Color);

    for (Coord row = 0; row < overlap.height; row++) {
        memcpy(dst, img, n);
        dst += dst_step;
        img += img_step;
    }

}

}
}
