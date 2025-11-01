cmake_minimum_required (VERSION 3.28)
project (exec_worker)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable (main main.cpp)
install (TARGETS main RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/exe)
