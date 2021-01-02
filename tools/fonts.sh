#!/bin/bash

# Generates embedded font files using the fontem font decoder
# https://github.com/chrisy/fontem
# TODO: Build fontem using Bazel and use a genrule to do this work

# Exit script if any step fails
set -e

# Create temporary storage directory
mkdir -p /tmp/fontem

# Download and verify Quicksand Light
wget -O /tmp/fontem/Quicksand-Light.ttf https://github.com/andrew-paglinawan/QuicksandFamily/raw/c0f7d6b2d2cb4e5477745d3eed096d7c1c4b05ba/fonts/statics/Quicksand-Light.ttf
echo "67b51bf5c5c2bb090c7bb3ec1866742d659a4623 */tmp/fontem/Quicksand-Light.ttf" | shasum -c -

# Generate embedded font files
fontem --name=quicksand --size=64 --chars=0123456789: --font=/tmp/fontem/Quicksand-Light.ttf --dir=lib/fonts
