cmake_minimum_required(VERSION 3.10)
project(gmp_example)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GMP REQUIRED gmp)

add_executable(example example.c)
target_include_directories(example PRIVATE ${GMP_INCLUDE_DIRS})
target_link_directories(example PRIVATE ${GMP_LIBRARY_DIRS})
target_link_libraries(example PRIVATE ${GMP_LIBRARIES})
