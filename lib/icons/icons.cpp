#include "icons.h"

const std::map<const std::string, struct icon_meta> icons = {
{{ range $key, $value := . }}
        {
            "{{ $key }}",
            {
                .data = reinterpret_cast<const uint8_t *>({{ $value.Address }}),
                .width = {{ $value.Width }},
                .height = {{ $value.Height }},
            },
        },
{{ end }}
};