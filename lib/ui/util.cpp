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

}
}
