//
// Created by Carl Friess on 13/01/21.
//

#ifndef PHASE_ICONS_H
#define PHASE_ICONS_H

#include <cstdint>
#include <cstddef>
#include <map>
#include <string>

struct icon_meta {
    const uint8_t *data;
    size_t width, height;
};

extern const std::map<const std::string, struct icon_meta> icons;

#endif //PHASE_ICONS_H
