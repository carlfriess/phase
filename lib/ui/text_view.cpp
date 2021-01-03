//
// Created by Carl Friess on 02/01/2021.
//

#include "phase-ui.h"

#include <stdlib.h>
#include <cstring>

extern "C" {
#include "fontem.h"
}

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
    for (const char *p = text; *p; p++) {

        // Fetch the next glyph
        const struct glyph *g = font_get_glyph(font, *p);
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
                g->advance - g->left + font_get_kerning(font, prev, *p);
        prev = *p;

    }

}

size_t TextView::setText(const char *str) {

    size_t numChars = 0;
    Coord width = 0;
    glyph_t prev = 0;

    // Iterate string and calculate width until frame width is exceeded
    for (const char *p = str; *p; p++, numChars++) {
        const struct glyph *g = font_get_glyph(font, *p);
        if (g == nullptr) {
            continue;
        }
        Coord gWidth = g->advance + font_get_kerning(font, prev, *p);
        prev = *p;
        if (width + gWidth <= frame.width) {
            width += gWidth;
        } else {
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

    // Check if the new string is different
    if (numChars != strlen(text) || strncmp(text, str, numChars)) {

        // Set dirty flag
        dirty = true;

        // Free previous string information and copy the accepted substring
        if (text) {
            free(text);
        }
        text = strndup(str, numChars);

    }

    return numChars;
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
        if (text) {
            setText(text);
            dirty = true;
        }
    }
}

}
}
