#!/usr/bin/env bash

SCRIPT_DIR=$(dirname -- "$0")

echo "BUILD_TYPE=${BUILD_TYPE}"

cmake --build "${SCRIPT_DIR}/../build" --config "${BUILD_TYPE}"
