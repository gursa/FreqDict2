cmake_minimum_required(VERSION 3.15)
project(freq)

set(ALL_CXX_SOURCE_FILES
        freq/freq.cpp
        freq/freq.hpp
        freq/main.cpp)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(freq)

add_executable(freq ${ALL_CXX_SOURCE_FILES})

find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
    add_custom_command(TARGET freq
            PRE_BUILD
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            COMMAND /usr/bin/clang-format -i -verbose -style=file ${ALL_CXX_SOURCE_FILES}
            )
endif()
