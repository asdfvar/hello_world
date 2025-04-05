#!/usr/bin/bash

cmake -DCMAKE_VERBOSE_MAKEFILE=ON -S . -B build

cd build

cmake --build .
