cmake_minimum_required (VERSION 3.13)
aux_source_directory(. Thread_srcs)
add_library(Thread STATIC ${Thread_srcs})


