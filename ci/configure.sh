#!/usr/bin/env bash

SCRIPT_DIR=$(dirname -- "$0")

if ! hash ninja; then
    sudo apt-get install ninja-build
fi

rm -rfv "${SCRIPT_DIR}/../build"

if [[ -z $GENERATOR ]]; then
    GENERATOR="Ninja Multi-Config"
fi

if [[ -z $BUILD_TYPE ]]; then
    BUILD_TYPE="Debug"
fi

if [[ -z $CXX_STANDARD ]]; then
    CXX_STANDARD="20"
fi

if [[ -z $LEXIO_TEST_NONSTD ]]; then
    LEXIO_TEST_NONSTD="OFF"
fi

cmake \
    -S "${SCRIPT_DIR}/.." \
    -B "${SCRIPT_DIR}/../build" \
    -G "${GENERATOR}" \
    -D "CMAKE_CXX_STANDARD=${CXX_STANDARD}" \
    -D "LEXIO_TEST_NONSTD=${LEXIO_TEST_NONSTD}"
