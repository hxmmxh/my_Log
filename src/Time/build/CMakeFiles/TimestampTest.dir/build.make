# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/hxm/Project/my_Log/src/Time

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hxm/Project/my_Log/src/Time/build

# Include any dependencies generated for this target.
include CMakeFiles/TimestampTest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/TimestampTest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TimestampTest.dir/flags.make

CMakeFiles/TimestampTest.dir/Test/TimestampTest.cpp.o: CMakeFiles/TimestampTest.dir/flags.make
CMakeFiles/TimestampTest.dir/Test/TimestampTest.cpp.o: ../Test/TimestampTest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hxm/Project/my_Log/src/Time/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TimestampTest.dir/Test/TimestampTest.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TimestampTest.dir/Test/TimestampTest.cpp.o -c /home/hxm/Project/my_Log/src/Time/Test/TimestampTest.cpp

CMakeFiles/TimestampTest.dir/Test/TimestampTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TimestampTest.dir/Test/TimestampTest.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hxm/Project/my_Log/src/Time/Test/TimestampTest.cpp > CMakeFiles/TimestampTest.dir/Test/TimestampTest.cpp.i

CMakeFiles/TimestampTest.dir/Test/TimestampTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TimestampTest.dir/Test/TimestampTest.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hxm/Project/my_Log/src/Time/Test/TimestampTest.cpp -o CMakeFiles/TimestampTest.dir/Test/TimestampTest.cpp.s

# Object files for target TimestampTest
TimestampTest_OBJECTS = \
"CMakeFiles/TimestampTest.dir/Test/TimestampTest.cpp.o"

# External object files for target TimestampTest
TimestampTest_EXTERNAL_OBJECTS =

bin/TimestampTest: CMakeFiles/TimestampTest.dir/Test/TimestampTest.cpp.o
bin/TimestampTest: CMakeFiles/TimestampTest.dir/build.make
bin/TimestampTest: lib/libTime.a
bin/TimestampTest: CMakeFiles/TimestampTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hxm/Project/my_Log/src/Time/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/TimestampTest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TimestampTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TimestampTest.dir/build: bin/TimestampTest

.PHONY : CMakeFiles/TimestampTest.dir/build

CMakeFiles/TimestampTest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TimestampTest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TimestampTest.dir/clean

CMakeFiles/TimestampTest.dir/depend:
	cd /home/hxm/Project/my_Log/src/Time/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hxm/Project/my_Log/src/Time /home/hxm/Project/my_Log/src/Time /home/hxm/Project/my_Log/src/Time/build /home/hxm/Project/my_Log/src/Time/build /home/hxm/Project/my_Log/src/Time/build/CMakeFiles/TimestampTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TimestampTest.dir/depend
