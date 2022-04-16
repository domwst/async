cmake_minimum_required(VERSION 3.5)

get_filename_component(EXAMPLES_PATH "." ABSOLUTE)

file(GLOB EXAMPLE_SOURCES RELATIVE ${EXAMPLES_PATH} "./*.cpp")

foreach(example_source_file ${EXAMPLE_SOURCES})
    string(REPLACE ".cpp" "" example_name ${example_source_file})
    add_executable(${example_name} ${example_source_file})
    target_link_libraries(${example_name} PUBLIC async)
endforeach()
