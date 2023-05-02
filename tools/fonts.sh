#!/bin/bash

# Generates embedded font files using the fontem font decoder
# https://github.com/chrisy/fontem
# TODO: Build fontem using Bazel and use a genrule to do this work

# Exit script if any step fails
set -e

# Create temporary storage directory
mkdir -p /tmp/fontem

# Download Quicksand Light
wget -O /tmp/fontem/Quicksand-Light.ttf https://github.com/andrew-paglinawan/QuicksandFamily/raw/c0f7d6b2d2cb4e5477745d3eed096d7c1c4b05ba/fonts/statics/Quicksand-Light.ttf

# Download OpenSans Regular and Bold
wget -O /tmp/fontem/OpenSans-Regular.ttf https://github.com/google/fonts/raw/4e24bf1805f1526cd1ce3e39447d5f8e6f1f6b67/apache/opensans/OpenSans-Regular.ttf
wget -O /tmp/fontem/OpenSans-Bold.ttf https://github.com/google/fonts/raw/4e24bf1805f1526cd1ce3e39447d5f8e6f1f6b67/apache/opensans/OpenSans-Bold.ttf

# Verify downloaded font files
cat <<EOF | shasum -c -
67b51bf5c5c2bb090c7bb3ec1866742d659a4623 */tmp/fontem/Quicksand-Light.ttf
513966e260bb7610d47b2329dba194143831893e */tmp/fontem/OpenSans-Regular.ttf
8387d4f8e061c264dc3aebebe6068b66e45d7c6f */tmp/fontem/OpenSans-Bold.ttf
EOF

# Configure locale for fontem
export LC_CTYPE=C
export LC_ALL=en_US.UTF-8

# Generate embedded font files
fontem --name=quicksand --size=64 --chars=0123456789: --font=/tmp/fontem/Quicksand-Light.ttf --dir=lib/fonts
fontem --name=opensans --size=12 --chars="!@#$%^&*()_+-={}|[]\\:\";'<>?,./\`~° ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789ÄÖÜßäöü" --font=/tmp/fontem/OpenSans-Regular.ttf --dir=lib/fonts
fontem --name=opensans-bold --size=12 --chars="!@#$%^&*()_+-={}|[]\\:\";'<>?,./\`~° ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789ÄÖÜßäöü" --font=/tmp/fontem/OpenSans-Bold.ttf --dir=lib/fonts
