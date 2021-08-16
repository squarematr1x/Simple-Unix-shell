#!/bin/bash

BUILD_BIN="shell"

make

if [ -e "${BUILD_BIN}" ]; then
  echo Starting shell...
  echo
  "./${BUILD_BIN}"
fi

echo
