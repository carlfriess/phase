//
// Created by Carl Friess on 19/01/21.
//

#include "utf8.h"
#include "decode.h"

namespace phase {
namespace utf8 {

error next(std::string::const_iterator &str, std::string::const_iterator end,
             uint32_t &code_point) {

    uint32_t state = UTF8_ACCEPT;

    code_point = 0;
    for (auto s = str; s < end; s++) {
        unsigned char c = *s;
        if (decode(&state, &code_point, c) == UTF8_ACCEPT) {
            str = ++s;
            return UTF8_OK;
        } else if (state == UTF8_REJECT) {
            return UTF8_ILLEGAL;
        }
    }

    return UTF8_INCOMPLETE;

}

error strlen(const std::string &str, size_t &len) {

    uint32_t codepoint;
    uint32_t state = UTF8_ACCEPT;

    len = 0;
    for (const unsigned char c : str) {
        if (!decode(&state, &codepoint, c)) {
            len++;
        } else if (state == UTF8_REJECT) {
            return UTF8_ILLEGAL;
        }
    }

    return state == UTF8_ACCEPT ? UTF8_OK : UTF8_INCOMPLETE;

}

}
}