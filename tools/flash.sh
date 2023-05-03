#!/bin/bash

# Exit script if any step fails
set -e

if [ $# -eq 0 ]; then
    echo "Usage: $0 <hex-file>[ <qspi-hex-file>]"
    exit 1
fi

# This script relies on the external nRF command line tools repository downloaded by Bazel.
# If the tools repository is missing, build an nRF5_binary target first.
BAZEL_OUTPUT_BASE=$(bazel info output_base)

# Select tools repository based on host OS
case "$(uname -s)" in
    Linux*)     TOOLS="$BAZEL_OUTPUT_BASE/external/nRF_tools_linux";;
    Darwin*)    TOOLS="$BAZEL_OUTPUT_BASE/external/nRF_tools_macOS";;
    *)          echo "Error: unsupported host platform"; exit 1
esac

NRFJPROG="$TOOLS/nrfjprog/nrfjprog"

if [ ! -f "$NRFJPROG" ]; then
    echo "Error: nrfjprog is missing"
    exit 1
fi

$NRFJPROG --program $1 --chiperase --reset

if [ "$#" -ge 2 ]; then
  $NRFJPROG --qspiini tools/QspiConfig.ini --program $2 --qspisectorerase --reset
fi
