#!/usr/bin/bash

cmake -S . -B build

cd build

cmake --build .

cmake --install . --prefix $PWD
