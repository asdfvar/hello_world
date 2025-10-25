cmake_minimum_required (VERSION 3.28) # Specify the minimum required CMake version
project (mutex LANGUAGES CXX)    # Define the project name and languages used

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

# Add an executable target named "HelloWorld" from the source file "main.cpp"
add_executable (mutex main.cpp)
install (TARGETS mutex DESTINATION ${CMAKE_SOURCE_DIR}/exe)
