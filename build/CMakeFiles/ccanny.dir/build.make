# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/nobug/anaconda3/lib/python3.8/site-packages/cmake/data/bin/cmake

# The command to remove a file.
RM = /home/nobug/anaconda3/lib/python3.8/site-packages/cmake/data/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/nobug/Documents/oblivion/ccanny

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/nobug/Documents/oblivion/ccanny/build

# Include any dependencies generated for this target.
include CMakeFiles/ccanny.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ccanny.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ccanny.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ccanny.dir/flags.make

CMakeFiles/ccanny.dir/main.cc.o: CMakeFiles/ccanny.dir/flags.make
CMakeFiles/ccanny.dir/main.cc.o: ../main.cc
CMakeFiles/ccanny.dir/main.cc.o: CMakeFiles/ccanny.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nobug/Documents/oblivion/ccanny/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ccanny.dir/main.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ccanny.dir/main.cc.o -MF CMakeFiles/ccanny.dir/main.cc.o.d -o CMakeFiles/ccanny.dir/main.cc.o -c /home/nobug/Documents/oblivion/ccanny/main.cc

CMakeFiles/ccanny.dir/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ccanny.dir/main.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nobug/Documents/oblivion/ccanny/main.cc > CMakeFiles/ccanny.dir/main.cc.i

CMakeFiles/ccanny.dir/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ccanny.dir/main.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nobug/Documents/oblivion/ccanny/main.cc -o CMakeFiles/ccanny.dir/main.cc.s

CMakeFiles/ccanny.dir/src/blur.cc.o: CMakeFiles/ccanny.dir/flags.make
CMakeFiles/ccanny.dir/src/blur.cc.o: ../src/blur.cc
CMakeFiles/ccanny.dir/src/blur.cc.o: CMakeFiles/ccanny.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nobug/Documents/oblivion/ccanny/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ccanny.dir/src/blur.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ccanny.dir/src/blur.cc.o -MF CMakeFiles/ccanny.dir/src/blur.cc.o.d -o CMakeFiles/ccanny.dir/src/blur.cc.o -c /home/nobug/Documents/oblivion/ccanny/src/blur.cc

CMakeFiles/ccanny.dir/src/blur.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ccanny.dir/src/blur.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nobug/Documents/oblivion/ccanny/src/blur.cc > CMakeFiles/ccanny.dir/src/blur.cc.i

CMakeFiles/ccanny.dir/src/blur.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ccanny.dir/src/blur.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nobug/Documents/oblivion/ccanny/src/blur.cc -o CMakeFiles/ccanny.dir/src/blur.cc.s

CMakeFiles/ccanny.dir/src/image/basic.cc.o: CMakeFiles/ccanny.dir/flags.make
CMakeFiles/ccanny.dir/src/image/basic.cc.o: ../src/image/basic.cc
CMakeFiles/ccanny.dir/src/image/basic.cc.o: CMakeFiles/ccanny.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nobug/Documents/oblivion/ccanny/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/ccanny.dir/src/image/basic.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ccanny.dir/src/image/basic.cc.o -MF CMakeFiles/ccanny.dir/src/image/basic.cc.o.d -o CMakeFiles/ccanny.dir/src/image/basic.cc.o -c /home/nobug/Documents/oblivion/ccanny/src/image/basic.cc

CMakeFiles/ccanny.dir/src/image/basic.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ccanny.dir/src/image/basic.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nobug/Documents/oblivion/ccanny/src/image/basic.cc > CMakeFiles/ccanny.dir/src/image/basic.cc.i

CMakeFiles/ccanny.dir/src/image/basic.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ccanny.dir/src/image/basic.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nobug/Documents/oblivion/ccanny/src/image/basic.cc -o CMakeFiles/ccanny.dir/src/image/basic.cc.s

CMakeFiles/ccanny.dir/src/image/io.cc.o: CMakeFiles/ccanny.dir/flags.make
CMakeFiles/ccanny.dir/src/image/io.cc.o: ../src/image/io.cc
CMakeFiles/ccanny.dir/src/image/io.cc.o: CMakeFiles/ccanny.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/nobug/Documents/oblivion/ccanny/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/ccanny.dir/src/image/io.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ccanny.dir/src/image/io.cc.o -MF CMakeFiles/ccanny.dir/src/image/io.cc.o.d -o CMakeFiles/ccanny.dir/src/image/io.cc.o -c /home/nobug/Documents/oblivion/ccanny/src/image/io.cc

CMakeFiles/ccanny.dir/src/image/io.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ccanny.dir/src/image/io.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/nobug/Documents/oblivion/ccanny/src/image/io.cc > CMakeFiles/ccanny.dir/src/image/io.cc.i

CMakeFiles/ccanny.dir/src/image/io.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ccanny.dir/src/image/io.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/nobug/Documents/oblivion/ccanny/src/image/io.cc -o CMakeFiles/ccanny.dir/src/image/io.cc.s

# Object files for target ccanny
ccanny_OBJECTS = \
"CMakeFiles/ccanny.dir/main.cc.o" \
"CMakeFiles/ccanny.dir/src/blur.cc.o" \
"CMakeFiles/ccanny.dir/src/image/basic.cc.o" \
"CMakeFiles/ccanny.dir/src/image/io.cc.o"

# External object files for target ccanny
ccanny_EXTERNAL_OBJECTS =

ccanny: CMakeFiles/ccanny.dir/main.cc.o
ccanny: CMakeFiles/ccanny.dir/src/blur.cc.o
ccanny: CMakeFiles/ccanny.dir/src/image/basic.cc.o
ccanny: CMakeFiles/ccanny.dir/src/image/io.cc.o
ccanny: CMakeFiles/ccanny.dir/build.make
ccanny: CMakeFiles/ccanny.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/nobug/Documents/oblivion/ccanny/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable ccanny"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ccanny.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ccanny.dir/build: ccanny
.PHONY : CMakeFiles/ccanny.dir/build

CMakeFiles/ccanny.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ccanny.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ccanny.dir/clean

CMakeFiles/ccanny.dir/depend:
	cd /home/nobug/Documents/oblivion/ccanny/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/nobug/Documents/oblivion/ccanny /home/nobug/Documents/oblivion/ccanny /home/nobug/Documents/oblivion/ccanny/build /home/nobug/Documents/oblivion/ccanny/build /home/nobug/Documents/oblivion/ccanny/build/CMakeFiles/ccanny.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/ccanny.dir/depend
