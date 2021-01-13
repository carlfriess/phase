//
// Created by Carl Friess on 02/01/2021.
//

#include "phase-ui.h"

#include <cstring>

extern "C" {
#include "fontem.h"
}

#include "utf8.h"

#include "util.h"

namespace phase {
namespace ui {

void TextView::render(uint8_t *buffer, Frame region) const {

    Frame glyph_frame = frame;
    glyph_t prev = 0;

    // Fill the background if this view is opaque
    if (opaque) {
        fill_frame_color(buffer, region, background_color, frame);
    }

    // Move initial frame according to text alignment offset
    glyph_frame.origin.x += alignment_offset;

    // Iterate the string
    auto it = text.begin();
    while (it < text.end()) {

        // Get next UTF-8 character
        glyph_t cur = utf8::next(it, text.end());

        // Fetch the next glyph
        const struct glyph *g = font_get_glyph(font, cur);
        if (g == nullptr) {
            continue;
        }

        // Setup draw frame for next glyph
        glyph_frame.origin.x += g->left;
        glyph_frame.origin.y = frame.origin.y + (font->ascender - g->top);
        glyph_frame.width = g->cols;
        glyph_frame.height = g->rows;

        // Check if the glyph overlaps with the buffer region
        if (region.overlaps(glyph_frame)) {

            // Draw the glyph
            mask_frame_color(buffer, region, color, g->bitmap, glyph_frame);

        }

        // Advance to the next character
        glyph_frame.origin.x +=
                g->advance - g->left + font_get_kerning(font, prev, cur);
        prev = cur;

    }

}

size_t TextView::setText(const std::string str) {

    Coord width = 0;
    Coord last_ws_width = 0;
    glyph_t prev = 0;

    // Iterate string and calculate width until frame width is exceeded
    auto it = str.begin();
    auto last_ws = it;
    while (it < str.end()) {

        // Get next UTF-8 character
        glyph_t cur = utf8::next(it, str.end());
        if (cur == '\n') {
            break;
        } else if (std::isspace(cur)) {
            last_ws = it;
            last_ws_width = width;
        }

        // Lookup the next glyph
        const struct glyph *g = font_get_glyph(font, cur);
        if (g == nullptr && (g = font_get_glyph(font, '?')) == nullptr) {
            continue;
        }

        // Calculate width
        Coord gWidth = g->advance + font_get_kerning(font, prev, cur);
        prev = cur;
        if (width + gWidth <= frame.width) {
            width += gWidth;
        } else {
            // If we encountered whitespace, jump back to word boundary
            if (last_ws > str.begin()) {
                it = last_ws;
                width = last_ws_width;
            } else {
                utf8::prior(it, str.begin());
            }
            break;
        }

    }

    // Calculate text alignment offset
    switch (align) {
        case LEFT:
            alignment_offset = 0;
            break;
        case CENTER:
            alignment_offset = (frame.width - width) / 2;
            break;
        case RIGHT:
            alignment_offset = frame.width - width;
            break;
    }

    // Get accepted substring and set dirty flag
    auto sub_str = std::string(str.begin(), it);
    dirty = text != sub_str;
    text = sub_str;

    // Return byte length of accepted substring
    return it - str.begin();
}

void TextView::setColor(const Color &new_color) {
    if (memcmp(color, new_color, sizeof(Color)) != 0) {
        dirty = true;
    }
    memcpy(color, new_color, sizeof(Color));
}

void TextView::setTextAlignment(TextView::TextAlignment setting) {
    if (align != setting) {
        align = setting;
        if (text.length()) {
            setText(text);
            dirty = true;
        }
    }
}

}
}
