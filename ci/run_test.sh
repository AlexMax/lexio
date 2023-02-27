#!/usr/bin/env bash

SCRIPT_DIR=$(dirname -- "$0")

echo "CMAKE_BUILD_TYPE=${BUILD_TYPE}" \

"${SCRIPT_DIR}/../build/${BUILD_TYPE}/test"
