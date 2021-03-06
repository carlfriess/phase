//
// Created by Carl Friess on 01/01/2021.
//

#ifndef PHASE_UTIL_H
#define PHASE_UTIL_H

#include "phase-ui.h"

namespace phase {
namespace ui {

void fill_color(uint8_t *dest, const Color color, size_t num);

void blend_color(uint8_t *dst, const Color color, uint8_t alpha);

void fill_frame_color(uint8_t *dst, const Frame &dst_frame, const Color color,
                      const Frame &fill_frame);

void mask_frame_color(uint8_t *dst, const Frame &dst_frame, const Color color,
                      const uint8_t *mask, const Frame &mask_frame);

void imgcpy(uint8_t *dst, const Frame &dst_frame, const uint8_t *img,
            const Frame &img_frame);

void imgcpy_mask(uint8_t *dst, const Frame &dst_frame, const uint8_t *mask,
                 const uint8_t *img, const Frame &img_frame);

}
}

#endif //PHASE_UTIL_H
