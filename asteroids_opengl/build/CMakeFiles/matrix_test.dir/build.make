# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/leon/Computergrafik/asteroids_opengl

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/leon/Computergrafik/asteroids_opengl/build

# Include any dependencies generated for this target.
include CMakeFiles/matrix_test.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/matrix_test.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/matrix_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/matrix_test.dir/flags.make

CMakeFiles/matrix_test.dir/matrix_test.cc.o: CMakeFiles/matrix_test.dir/flags.make
CMakeFiles/matrix_test.dir/matrix_test.cc.o: ../matrix_test.cc
CMakeFiles/matrix_test.dir/matrix_test.cc.o: CMakeFiles/matrix_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leon/Computergrafik/asteroids_opengl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/matrix_test.dir/matrix_test.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/matrix_test.dir/matrix_test.cc.o -MF CMakeFiles/matrix_test.dir/matrix_test.cc.o.d -o CMakeFiles/matrix_test.dir/matrix_test.cc.o -c /home/leon/Computergrafik/asteroids_opengl/matrix_test.cc

CMakeFiles/matrix_test.dir/matrix_test.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/matrix_test.dir/matrix_test.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/leon/Computergrafik/asteroids_opengl/matrix_test.cc > CMakeFiles/matrix_test.dir/matrix_test.cc.i

CMakeFiles/matrix_test.dir/matrix_test.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/matrix_test.dir/matrix_test.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/leon/Computergrafik/asteroids_opengl/matrix_test.cc -o CMakeFiles/matrix_test.dir/matrix_test.cc.s

CMakeFiles/matrix_test.dir/matrix.cc.o: CMakeFiles/matrix_test.dir/flags.make
CMakeFiles/matrix_test.dir/matrix.cc.o: ../matrix.cc
CMakeFiles/matrix_test.dir/matrix.cc.o: CMakeFiles/matrix_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leon/Computergrafik/asteroids_opengl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/matrix_test.dir/matrix.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/matrix_test.dir/matrix.cc.o -MF CMakeFiles/matrix_test.dir/matrix.cc.o.d -o CMakeFiles/matrix_test.dir/matrix.cc.o -c /home/leon/Computergrafik/asteroids_opengl/matrix.cc

CMakeFiles/matrix_test.dir/matrix.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/matrix_test.dir/matrix.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/leon/Computergrafik/asteroids_opengl/matrix.cc > CMakeFiles/matrix_test.dir/matrix.cc.i

CMakeFiles/matrix_test.dir/matrix.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/matrix_test.dir/matrix.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/leon/Computergrafik/asteroids_opengl/matrix.cc -o CMakeFiles/matrix_test.dir/matrix.cc.s

CMakeFiles/matrix_test.dir/math.cc.o: CMakeFiles/matrix_test.dir/flags.make
CMakeFiles/matrix_test.dir/math.cc.o: ../math.cc
CMakeFiles/matrix_test.dir/math.cc.o: CMakeFiles/matrix_test.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/leon/Computergrafik/asteroids_opengl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/matrix_test.dir/math.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/matrix_test.dir/math.cc.o -MF CMakeFiles/matrix_test.dir/math.cc.o.d -o CMakeFiles/matrix_test.dir/math.cc.o -c /home/leon/Computergrafik/asteroids_opengl/math.cc

CMakeFiles/matrix_test.dir/math.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/matrix_test.dir/math.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/leon/Computergrafik/asteroids_opengl/math.cc > CMakeFiles/matrix_test.dir/math.cc.i

CMakeFiles/matrix_test.dir/math.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/matrix_test.dir/math.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/leon/Computergrafik/asteroids_opengl/math.cc -o CMakeFiles/matrix_test.dir/math.cc.s

# Object files for target matrix_test
matrix_test_OBJECTS = \
"CMakeFiles/matrix_test.dir/matrix_test.cc.o" \
"CMakeFiles/matrix_test.dir/matrix.cc.o" \
"CMakeFiles/matrix_test.dir/math.cc.o"

# External object files for target matrix_test
matrix_test_EXTERNAL_OBJECTS =

matrix_test: CMakeFiles/matrix_test.dir/matrix_test.cc.o
matrix_test: CMakeFiles/matrix_test.dir/matrix.cc.o
matrix_test: CMakeFiles/matrix_test.dir/math.cc.o
matrix_test: CMakeFiles/matrix_test.dir/build.make
matrix_test: CMakeFiles/matrix_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/leon/Computergrafik/asteroids_opengl/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable matrix_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/matrix_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/matrix_test.dir/build: matrix_test
.PHONY : CMakeFiles/matrix_test.dir/build

CMakeFiles/matrix_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/matrix_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/matrix_test.dir/clean

CMakeFiles/matrix_test.dir/depend:
	cd /home/leon/Computergrafik/asteroids_opengl/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/leon/Computergrafik/asteroids_opengl /home/leon/Computergrafik/asteroids_opengl /home/leon/Computergrafik/asteroids_opengl/build /home/leon/Computergrafik/asteroids_opengl/build /home/leon/Computergrafik/asteroids_opengl/build/CMakeFiles/matrix_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/matrix_test.dir/depend

