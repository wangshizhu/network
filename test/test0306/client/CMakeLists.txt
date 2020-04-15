cmake_minimum_required(VERSION 3.16)

set(EXECUTABLE_OUTPUT_PATH ../bin)
set (CMAKE_CXX_FLAGS "-g -pthread")
project(client VERSION 1.0)

if(CMAKE_COMPILER_IS_GNUCC)
    message("COMPILER IS GNUCC")
    ADD_DEFINITIONS ( -std=c++1y)
endif(CMAKE_COMPILER_IS_GNUCC)

file(GLOB_RECURSE CURRENT_HEADERS  *.h *.hpp ../../../common/*.h ../../../common/*.cpp ../../../lib/*.h ../../../lib/*.cpp ../src/*.h ../src/*.cpp)
source_group("Header" FILES ${CURRENT_HEADERS}) 

include_directories(../../../include/msgpack)
aux_source_directory(. DIR_SRCS)
add_executable(client ${DIR_SRCS} ${CURRENT_HEADERS})

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
