cmake_minimum_required (VERSION 3.28.3)
project (dynamicLib)

add_library (foo SHARED foo.c)
add_executable (test main.c)
target_link_libraries (test foo)
install (TARGETS test RUNTIME DESTINATION bin)
install (TARGETS foo LIBRARY DESTINATION lib PERMISSIONS WORLD_EXECUTE OWNER_WRITE)
