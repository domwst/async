cmake_minimum_required(VERSION 3.5)
project(async)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

add_compile_options(-Wall -Wextra -Wpedantic -Werror -fno-omit-frame-pointer)

add_subdirectory(async)
add_subdirectory(examples)
