cmake_minimum_required (VERSION 3.10)
project (templates LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable (main main.cpp)

install (TARGETS main DESTINATION ${PROJECT_SOURCE_DIR}/exe)
