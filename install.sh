#!/bin/bash

cd $(dirname $0)
if [ -d "build" ]; then
	rm -rf build
fi
mkdir build
cmake -S . -B build
make -C build
