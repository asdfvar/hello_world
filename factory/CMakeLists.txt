cmake_minimum_required (VERSION 3.28.3)
project (factory)

add_executable (factory factory.cpp)

install (TARGETS factory RUNTIME DESTINATION bin)
