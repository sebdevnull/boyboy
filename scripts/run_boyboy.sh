#!/bin/sh
# Wrapper to run BoyBoy with ASan suppressions

SCRIPT_DIR=$(cd "$(dirname "$0")" && pwd)
ROOT_DIR=$(cd "${SCRIPT_DIR}/.." && pwd)

ASAN_SUPPRESS_FILE="${ROOT_DIR}/config/asan_suppress.txt"

if [ ! -f "${ASAN_SUPPRESS_FILE}" ]; then
    echo "Error: ASan suppression file not found at ${ASAN_SUPPRESS_FILE}"
    exit 1
fi

export ASAN_OPTIONS="detect_leaks=1:print_suppressions=1"
export LSAN_OPTIONS="suppressions=${ASAN_SUPPRESS_FILE}"

BOYBOY_BINARY="${ROOT_DIR}/bin/debug/boyboy"

echo "Running with ASAN_OPTIONS: $ASAN_OPTIONS"
echo "Running with LSAN_OPTIONS: $LSAN_OPTIONS"

exec "${BOYBOY_BINARY}" "$@"
