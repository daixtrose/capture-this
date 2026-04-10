cmake_minimum_required(VERSION 3.29)

project(capture-this-examples LANGUAGES CXX)

# Each example is built separately per C++ standard.
# Use: cmake -DCMAKE_CXX_STANDARD=17 ..
# or the helper script: ./build.sh
if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Treat warnings as errors to make standard differences visible
add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(01_this_pointer src/01_this_pointer.cpp)
add_executable(02_star_this src/02_star_this.cpp)
add_executable(03_init_capture src/03_init_capture.cpp)
add_executable(04_implicit_this src/04_implicit_this.cpp)
add_executable(05_deprecated_eq src/05_deprecated_eq.cpp)
add_executable(06_dangling src/06_dangling.cpp)
