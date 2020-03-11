cmake_minimum_required(VERSION 3.10)
project(freq)

set(ALL_CXX_SOURCE_FILES
    freq/freq.cpp
    freq/freq.hpp
    freq/main.cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(FREQ_OPTIMIZE_FLAGS "-O3 -mmmx -msse -msse2 -msse3 -mssse3 -msse4.1 -msse4.2 -maes -mavx")
set(FREQ_RELEASE_FLAGS "${FREQ_OPTIMIZE_FLAGS} -Wall -Wextra -Werror")
set(FREQ_DEBUG_FLAGS "-Wall -Wextra -Werror -g")

set(CMAKE_CXX_FLAGS_RELEASE ${FREQ_OPTIMIZE_FLAGS})
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO ${FREQ_OPTIMIZE_FLAGS})
set(CMAKE_CXX_FLAGS_DEBUG ${FREQ_DEBUG_FLAGS})

include_directories(.)
include_directories(freq)

add_executable(freq ${ALL_CXX_SOURCE_FILES})

#if(UNIX)
#    find_program(CLANG_FORMAT "clang-format")
#    if(CLANG_FORMAT)
#        add_custom_command(TARGET freq
#            PRE_BUILD
#            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#            COMMAND /usr/bin/clang-format -i -verbose -style=file ${ALL_CXX_SOURCE_FILES})
#    endif(CLANG_FORMAT)
#endif(UNIX)
