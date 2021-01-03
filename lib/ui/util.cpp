//
// Created by Carl Friess on 01/01/2021.
//

#include "util.h"

#include <cstring>
#include <unistd.h>

namespace phase {
namespace ui {

void fill_color(uint8_t *dest, const Color color, size_t num) {
    uint8_t *limit = dest + (num * sizeof(Color));
    for (uint8_t *ptr = dest; ptr < limit; ptr += sizeof(Color)) {
        memcpy(ptr, color, sizeof(Color));
    }
}

void blend_color(uint8_t *dst, const Color color, uint8_t alpha) {
    uint16_t a = alpha;
    uint16_t a_inv = 0xFF - alpha;
    dst[0] = (color[0] * a + dst[0] * a_inv) >> 8;
    dst[1] = (color[1] * a + dst[1] * a_inv) >> 8;
    dst[2] = (color[2] * a + dst[2] * a_inv) >> 8;
}

void fill_frame_color(uint8_t *dst, const Frame &dst_frame, const Color color,
                      const Frame &fill_frame) {

    if (!dst_frame.overlaps(fill_frame)) {
        return;
    }

    // Calculate steps for buffer
    size_t dst_step = static_cast<size_t>(dst_frame.width) * sizeof(Color);

    // Adjust buffer pointer for vertical offset
    ssize_t y_offset = fill_frame.origin.y - dst_frame.origin.y;
    if (y_offset > 0) {
        dst += y_offset * dst_step;
    }

    // Adjust buffer pointer for horizontal offset
    ssize_t x_offset = fill_frame.origin.x - dst_frame.origin.x;
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

void mask_frame_color(uint8_t *dst, const Frame &dst_frame, const Color color,
                      const uint8_t *mask, const Frame &mask_frame) {

    if (!dst_frame.overlaps(mask_frame)) {
        return;
    }

    // Calculate steps for each buffer
    size_t dst_step = static_cast<size_t>(dst_frame.width) * sizeof(Color);
    size_t mask_step = mask_frame.width;

    // Adjust buffer pointers for vertical offset
    ssize_t y_offset = mask_frame.origin.y - dst_frame.origin.y;
    if (y_offset > 0) {
        dst += y_offset * dst_step;
    } else {
        mask += (-y_offset) * mask_step;
    }

    // Adjust buffer pointers for horizontal offset
    ssize_t x_offset = mask_frame.origin.x - dst_frame.origin.x;
    if (x_offset > 0) {
        dst += x_offset * sizeof(Color);
    } else {
        mask += (-x_offset);
    }

    // Calculate number of bytes that need to be copied per row
    Frame overlap = dst_frame.overlap(mask_frame);
    size_t n = static_cast<size_t>(overlap.width) * sizeof(Color);

    for (Coord row = 0; row < overlap.height; row++) {
        uint8_t *limit = dst + n;
        const uint8_t *m = mask;
        for (uint8_t *d = dst; d < limit; d += sizeof(Color), m++) {
            blend_color(d, color, *m);
        }
        dst += dst_step;
        mask += mask_step;
    }

}

void imgcpy(uint8_t *dst, const Frame &dst_frame, const uint8_t *img,
            const Frame &img_frame) {

    if (!dst_frame.overlaps(img_frame)) {
        return;
    }

    // Calculate steps for each buffer
    size_t dst_step = static_cast<size_t>(dst_frame.width) * sizeof(Color);
    size_t img_step = static_cast<size_t>(img_frame.width) * sizeof(Color);

    // Adjust buffer pointers for vertical offset
    ssize_t y_offset = img_frame.origin.y - dst_frame.origin.y;
    if (y_offset > 0) {
        dst += y_offset * dst_step;
    } else {
        img += (-y_offset) * img_step;
    }

    // Adjust buffer pointers for horizontal offset
    ssize_t x_offset = img_frame.origin.x - dst_frame.origin.x;
    if (x_offset > 0) {
        dst += x_offset * sizeof(Color);
    } else {
        img += (-x_offset) * sizeof(Color);
    }

    // Calculate number of bytes that need to be copied per row
    Frame overlap = dst_frame.overlap(img_frame);
    size_t n = static_cast<size_t>(overlap.width) * sizeof(Color);

    for (Coord row = 0; row < overlap.height; row++) {
        memcpy(dst, img, n);
        dst += dst_step;
        img += img_step;
    }

}

}
}
