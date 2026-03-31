cmake_minimum_required(VERSION 3.5)

get_filename_component(LIB_INCLUDE_PATH ".." ABSOLUTE)

file(GLOB_RECURSE LIB_CXX_SOURCES CONFIGURE_DEPENDS *.cpp)

add_library(async STATIC ${LIB_CXX_SOURCES} ${LIB_ASM_SOURCES})
target_include_directories(async PUBLIC ${LIB_INCLUDE_PATH})
target_compile_options(async PUBLIC -masm=intel)
