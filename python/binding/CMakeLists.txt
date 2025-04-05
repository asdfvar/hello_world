cmake_minimum_required (VERSION 3.28.3)
project (pyBind)

find_package (Python3 COMPONENTS Interpreter Development)
find_package (pybind11 REQUIRED)

add_library (example SHARED example.cpp)
target_link_libraries (example PRIVATE Python3::Python)

#include_directories (/usr/include/python3.12)
#add_library (example SHARED example.cpp)
#install (TARGETS example LIBRARY DESTINATION lib PERMISSIONS WORLD_EXECUTE OWNER_WRITE)
