cmake_minimum_required (VERSION 3.13)
aux_source_directory(. Time_srcs)
add_library(Time STATIC ${Time_srcs})