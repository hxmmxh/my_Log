cmake_minimum_required (VERSION 3.13)

set(CMAKE_CXX_FLAGS "-pthread")

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(../include)

aux_source_directory(. Log_srcs)

add_library(Log STATIC ${Log_srcs})
