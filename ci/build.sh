#!/usr/bin/env bash

SCRIPT_DIR=$(dirname -- "$0")

cmake --build "${SCRIPT_DIR}/../build"
