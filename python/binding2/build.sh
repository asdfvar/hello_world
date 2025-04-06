#!/bin/bash

(

cd "$(dirname "$0")"

g++ -fPIC \
    -I ~/anaconda3/envs/py38/include/python3.8 \
    -I ~/anaconda3/envs/py38/lib/python3.8/site-packages/numpy/core/include/numpy \
    -L"$(echo ~/anaconda3/envs/py38/lib)" \
    -shared \
    -o ../Lib/MyLib.so \
    ./theExtension.cpp \
    -lpython3.8 \
    -O3 \
    -std=c++20 \
    -lz
)
