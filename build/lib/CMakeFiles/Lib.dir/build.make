# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/robertj/coursework3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/robertj/coursework3/build

# Include any dependencies generated for this target.
include lib/CMakeFiles/Lib.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include lib/CMakeFiles/Lib.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/CMakeFiles/Lib.dir/progress.make

# Include the compile flags for this target's objects.
include lib/CMakeFiles/Lib.dir/flags.make

lib/CMakeFiles/Lib.dir/serialize.cpp.o: lib/CMakeFiles/Lib.dir/flags.make
lib/CMakeFiles/Lib.dir/serialize.cpp.o: ../lib/serialize.cpp
lib/CMakeFiles/Lib.dir/serialize.cpp.o: lib/CMakeFiles/Lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/robertj/coursework3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/CMakeFiles/Lib.dir/serialize.cpp.o"
	cd /home/robertj/coursework3/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lib/CMakeFiles/Lib.dir/serialize.cpp.o -MF CMakeFiles/Lib.dir/serialize.cpp.o.d -o CMakeFiles/Lib.dir/serialize.cpp.o -c /home/robertj/coursework3/lib/serialize.cpp

lib/CMakeFiles/Lib.dir/serialize.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Lib.dir/serialize.cpp.i"
	cd /home/robertj/coursework3/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/robertj/coursework3/lib/serialize.cpp > CMakeFiles/Lib.dir/serialize.cpp.i

lib/CMakeFiles/Lib.dir/serialize.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Lib.dir/serialize.cpp.s"
	cd /home/robertj/coursework3/build/lib && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/robertj/coursework3/lib/serialize.cpp -o CMakeFiles/Lib.dir/serialize.cpp.s

# Object files for target Lib
Lib_OBJECTS = \
"CMakeFiles/Lib.dir/serialize.cpp.o"

# External object files for target Lib
Lib_EXTERNAL_OBJECTS =

lib/libLib.a: lib/CMakeFiles/Lib.dir/serialize.cpp.o
lib/libLib.a: lib/CMakeFiles/Lib.dir/build.make
lib/libLib.a: lib/CMakeFiles/Lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/robertj/coursework3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libLib.a"
	cd /home/robertj/coursework3/build/lib && $(CMAKE_COMMAND) -P CMakeFiles/Lib.dir/cmake_clean_target.cmake
	cd /home/robertj/coursework3/build/lib && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/CMakeFiles/Lib.dir/build: lib/libLib.a
.PHONY : lib/CMakeFiles/Lib.dir/build

lib/CMakeFiles/Lib.dir/clean:
	cd /home/robertj/coursework3/build/lib && $(CMAKE_COMMAND) -P CMakeFiles/Lib.dir/cmake_clean.cmake
.PHONY : lib/CMakeFiles/Lib.dir/clean

lib/CMakeFiles/Lib.dir/depend:
	cd /home/robertj/coursework3/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/robertj/coursework3 /home/robertj/coursework3/lib /home/robertj/coursework3/build /home/robertj/coursework3/build/lib /home/robertj/coursework3/build/lib/CMakeFiles/Lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/CMakeFiles/Lib.dir/depend
