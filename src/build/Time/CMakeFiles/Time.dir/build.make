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
CMAKE_SOURCE_DIR = /home/hxm/Project/my_Log/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hxm/Project/my_Log/src/build

# Include any dependencies generated for this target.
include Time/CMakeFiles/Time.dir/depend.make

# Include the progress variables for this target.
include Time/CMakeFiles/Time.dir/progress.make

# Include the compile flags for this target's objects.
include Time/CMakeFiles/Time.dir/flags.make

Time/CMakeFiles/Time.dir/Date.cpp.o: Time/CMakeFiles/Time.dir/flags.make
Time/CMakeFiles/Time.dir/Date.cpp.o: ../Time/Date.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hxm/Project/my_Log/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Time/CMakeFiles/Time.dir/Date.cpp.o"
	cd /home/hxm/Project/my_Log/src/build/Time && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Time.dir/Date.cpp.o -c /home/hxm/Project/my_Log/src/Time/Date.cpp

Time/CMakeFiles/Time.dir/Date.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Time.dir/Date.cpp.i"
	cd /home/hxm/Project/my_Log/src/build/Time && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hxm/Project/my_Log/src/Time/Date.cpp > CMakeFiles/Time.dir/Date.cpp.i

Time/CMakeFiles/Time.dir/Date.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Time.dir/Date.cpp.s"
	cd /home/hxm/Project/my_Log/src/build/Time && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hxm/Project/my_Log/src/Time/Date.cpp -o CMakeFiles/Time.dir/Date.cpp.s

Time/CMakeFiles/Time.dir/Timestamp.cpp.o: Time/CMakeFiles/Time.dir/flags.make
Time/CMakeFiles/Time.dir/Timestamp.cpp.o: ../Time/Timestamp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hxm/Project/my_Log/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object Time/CMakeFiles/Time.dir/Timestamp.cpp.o"
	cd /home/hxm/Project/my_Log/src/build/Time && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Time.dir/Timestamp.cpp.o -c /home/hxm/Project/my_Log/src/Time/Timestamp.cpp

Time/CMakeFiles/Time.dir/Timestamp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Time.dir/Timestamp.cpp.i"
	cd /home/hxm/Project/my_Log/src/build/Time && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hxm/Project/my_Log/src/Time/Timestamp.cpp > CMakeFiles/Time.dir/Timestamp.cpp.i

Time/CMakeFiles/Time.dir/Timestamp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Time.dir/Timestamp.cpp.s"
	cd /home/hxm/Project/my_Log/src/build/Time && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hxm/Project/my_Log/src/Time/Timestamp.cpp -o CMakeFiles/Time.dir/Timestamp.cpp.s

# Object files for target Time
Time_OBJECTS = \
"CMakeFiles/Time.dir/Date.cpp.o" \
"CMakeFiles/Time.dir/Timestamp.cpp.o"

# External object files for target Time
Time_EXTERNAL_OBJECTS =

lib/libTime.a: Time/CMakeFiles/Time.dir/Date.cpp.o
lib/libTime.a: Time/CMakeFiles/Time.dir/Timestamp.cpp.o
lib/libTime.a: Time/CMakeFiles/Time.dir/build.make
lib/libTime.a: Time/CMakeFiles/Time.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hxm/Project/my_Log/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library ../lib/libTime.a"
	cd /home/hxm/Project/my_Log/src/build/Time && $(CMAKE_COMMAND) -P CMakeFiles/Time.dir/cmake_clean_target.cmake
	cd /home/hxm/Project/my_Log/src/build/Time && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Time.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Time/CMakeFiles/Time.dir/build: lib/libTime.a

.PHONY : Time/CMakeFiles/Time.dir/build

Time/CMakeFiles/Time.dir/clean:
	cd /home/hxm/Project/my_Log/src/build/Time && $(CMAKE_COMMAND) -P CMakeFiles/Time.dir/cmake_clean.cmake
.PHONY : Time/CMakeFiles/Time.dir/clean

Time/CMakeFiles/Time.dir/depend:
	cd /home/hxm/Project/my_Log/src/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hxm/Project/my_Log/src /home/hxm/Project/my_Log/src/Time /home/hxm/Project/my_Log/src/build /home/hxm/Project/my_Log/src/build/Time /home/hxm/Project/my_Log/src/build/Time/CMakeFiles/Time.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Time/CMakeFiles/Time.dir/depend
