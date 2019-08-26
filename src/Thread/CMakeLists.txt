cmake_minimum_required (VERSION 3.13)
aux_source_directory(. Thread_srcs)
add_library(Thread STATIC ${Thread_srcs})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
aux_source_directory(./Test Thread_Tests)
foreach(testfile ${Thread_Tests})
    get_filename_component(testname ${testfile} NAME_WE)
    target_link_libraries(${testname} Thread)
    add_executable(${testname} ${testfile})
endforeach(testfile)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)



