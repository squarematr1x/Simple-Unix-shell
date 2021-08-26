#!/bin/bash

BUILD_BIN="shell"
BUILD_DIR="build"

if [ -d "${BUILD_DIR}" ]; then
  make clean
fi

make

if [ -e "${BUILD_BIN}" ]; then
  echo Starting shell...
  echo
  "./${BUILD_BIN}"
fi

echo
