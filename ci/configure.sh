#!/usr/bin/env bash

SCRIPT_DIR=$(dirname -- "$0")

rm -rfv "${SCRIPT_DIR}/../build"

if [[ -z $GENERATOR ]]; then
    GENERATOR="Ninja Multi-Config"
fi

cmake \
    -S "${SCRIPT_DIR}/.." \
    -B "${SCRIPT_DIR}/../build" \
    -G "${GENERATOR}" \
    -D "CMAKE_CXX_STANDARD=${CXX_STANDARD}" \
    -D "LEXIO_TEST_NONSTD=${LEXIO_TEST_NONSTD}"
