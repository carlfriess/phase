#include <stdint.h>
#include <stddef.h>

const size_t {{ .Name }}_width = {{ .Width }};
const size_t {{ .Name }}_height = {{ .Height }};

const uint8_t {{ .Name }}[] = {
{{ range .Data }}
        {{ range . }}{{ . }}, {{- end -}}
{{ end }}
};
