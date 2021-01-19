//
// Created by Carl Friess on 19/01/21.
//

#ifndef PHASE_UTF8_H
#define PHASE_UTF8_H

#include <string>

namespace phase {
namespace utf8 {

enum error {
    UTF8_OK,
    UTF8_ILLEGAL,
    UTF8_INCOMPLETE,
};

error next(std::string::const_iterator &str, std::string::const_iterator end,
           uint32_t &code_point);

error strlen(const std::string &str, size_t &len);

}
}

#endif //PHASE_UTF8_H
