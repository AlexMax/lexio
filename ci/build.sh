#!/usr/bin/env bash

SCRIPT_DIR=$(dirname -- "$0")

echo "CMAKE_BUILD_TYPE=${BUILD_TYPE}" \

cmake --build "${SCRIPT_DIR}/../build" --config "${CMAKE_BUILD_TYPE}"
