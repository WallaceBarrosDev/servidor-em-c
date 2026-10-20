cmake_minimum_required(VERSION 3.20)
project(lib_api C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB PROGRAM_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.c)

add_executable(program ${PROGRAM_SOURCES})

target_compile_options(program PRIVATE -Wall -Wextra -Wpedantic)

set_target_properties(program PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
    target_link_libraries(program PRIVATE ${MATH_LIBRARY})
endif()
