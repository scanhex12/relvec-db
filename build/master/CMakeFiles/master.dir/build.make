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
include master/CMakeFiles/master.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include master/CMakeFiles/master.dir/compiler_depend.make

# Include the progress variables for this target.
include master/CMakeFiles/master.dir/progress.make

# Include the compile flags for this target's objects.
include master/CMakeFiles/master.dir/flags.make

master/CMakeFiles/master.dir/main.cpp.o: master/CMakeFiles/master.dir/flags.make
master/CMakeFiles/master.dir/main.cpp.o: ../master/main.cpp
master/CMakeFiles/master.dir/main.cpp.o: master/CMakeFiles/master.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/robertj/coursework3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object master/CMakeFiles/master.dir/main.cpp.o"
	cd /home/robertj/coursework3/build/master && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT master/CMakeFiles/master.dir/main.cpp.o -MF CMakeFiles/master.dir/main.cpp.o.d -o CMakeFiles/master.dir/main.cpp.o -c /home/robertj/coursework3/master/main.cpp

master/CMakeFiles/master.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/master.dir/main.cpp.i"
	cd /home/robertj/coursework3/build/master && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/robertj/coursework3/master/main.cpp > CMakeFiles/master.dir/main.cpp.i

master/CMakeFiles/master.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/master.dir/main.cpp.s"
	cd /home/robertj/coursework3/build/master && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/robertj/coursework3/master/main.cpp -o CMakeFiles/master.dir/main.cpp.s

master/CMakeFiles/master.dir/persistent_kv/kv_storage.cpp.o: master/CMakeFiles/master.dir/flags.make
master/CMakeFiles/master.dir/persistent_kv/kv_storage.cpp.o: ../master/persistent_kv/kv_storage.cpp
master/CMakeFiles/master.dir/persistent_kv/kv_storage.cpp.o: master/CMakeFiles/master.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/robertj/coursework3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object master/CMakeFiles/master.dir/persistent_kv/kv_storage.cpp.o"
	cd /home/robertj/coursework3/build/master && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT master/CMakeFiles/master.dir/persistent_kv/kv_storage.cpp.o -MF CMakeFiles/master.dir/persistent_kv/kv_storage.cpp.o.d -o CMakeFiles/master.dir/persistent_kv/kv_storage.cpp.o -c /home/robertj/coursework3/master/persistent_kv/kv_storage.cpp

master/CMakeFiles/master.dir/persistent_kv/kv_storage.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/master.dir/persistent_kv/kv_storage.cpp.i"
	cd /home/robertj/coursework3/build/master && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/robertj/coursework3/master/persistent_kv/kv_storage.cpp > CMakeFiles/master.dir/persistent_kv/kv_storage.cpp.i

master/CMakeFiles/master.dir/persistent_kv/kv_storage.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/master.dir/persistent_kv/kv_storage.cpp.s"
	cd /home/robertj/coursework3/build/master && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/robertj/coursework3/master/persistent_kv/kv_storage.cpp -o CMakeFiles/master.dir/persistent_kv/kv_storage.cpp.s

master/CMakeFiles/master.dir/persistent_kv/flexible_page.cpp.o: master/CMakeFiles/master.dir/flags.make
master/CMakeFiles/master.dir/persistent_kv/flexible_page.cpp.o: ../master/persistent_kv/flexible_page.cpp
master/CMakeFiles/master.dir/persistent_kv/flexible_page.cpp.o: master/CMakeFiles/master.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/robertj/coursework3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object master/CMakeFiles/master.dir/persistent_kv/flexible_page.cpp.o"
	cd /home/robertj/coursework3/build/master && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT master/CMakeFiles/master.dir/persistent_kv/flexible_page.cpp.o -MF CMakeFiles/master.dir/persistent_kv/flexible_page.cpp.o.d -o CMakeFiles/master.dir/persistent_kv/flexible_page.cpp.o -c /home/robertj/coursework3/master/persistent_kv/flexible_page.cpp

master/CMakeFiles/master.dir/persistent_kv/flexible_page.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/master.dir/persistent_kv/flexible_page.cpp.i"
	cd /home/robertj/coursework3/build/master && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/robertj/coursework3/master/persistent_kv/flexible_page.cpp > CMakeFiles/master.dir/persistent_kv/flexible_page.cpp.i

master/CMakeFiles/master.dir/persistent_kv/flexible_page.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/master.dir/persistent_kv/flexible_page.cpp.s"
	cd /home/robertj/coursework3/build/master && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/robertj/coursework3/master/persistent_kv/flexible_page.cpp -o CMakeFiles/master.dir/persistent_kv/flexible_page.cpp.s

master/CMakeFiles/master.dir/persistent_kv/file_editor.cpp.o: master/CMakeFiles/master.dir/flags.make
master/CMakeFiles/master.dir/persistent_kv/file_editor.cpp.o: ../master/persistent_kv/file_editor.cpp
master/CMakeFiles/master.dir/persistent_kv/file_editor.cpp.o: master/CMakeFiles/master.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/robertj/coursework3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object master/CMakeFiles/master.dir/persistent_kv/file_editor.cpp.o"
	cd /home/robertj/coursework3/build/master && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT master/CMakeFiles/master.dir/persistent_kv/file_editor.cpp.o -MF CMakeFiles/master.dir/persistent_kv/file_editor.cpp.o.d -o CMakeFiles/master.dir/persistent_kv/file_editor.cpp.o -c /home/robertj/coursework3/master/persistent_kv/file_editor.cpp

master/CMakeFiles/master.dir/persistent_kv/file_editor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/master.dir/persistent_kv/file_editor.cpp.i"
	cd /home/robertj/coursework3/build/master && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/robertj/coursework3/master/persistent_kv/file_editor.cpp > CMakeFiles/master.dir/persistent_kv/file_editor.cpp.i

master/CMakeFiles/master.dir/persistent_kv/file_editor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/master.dir/persistent_kv/file_editor.cpp.s"
	cd /home/robertj/coursework3/build/master && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/robertj/coursework3/master/persistent_kv/file_editor.cpp -o CMakeFiles/master.dir/persistent_kv/file_editor.cpp.s

master/CMakeFiles/master.dir/balancer.cpp.o: master/CMakeFiles/master.dir/flags.make
master/CMakeFiles/master.dir/balancer.cpp.o: ../master/balancer.cpp
master/CMakeFiles/master.dir/balancer.cpp.o: master/CMakeFiles/master.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/robertj/coursework3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object master/CMakeFiles/master.dir/balancer.cpp.o"
	cd /home/robertj/coursework3/build/master && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT master/CMakeFiles/master.dir/balancer.cpp.o -MF CMakeFiles/master.dir/balancer.cpp.o.d -o CMakeFiles/master.dir/balancer.cpp.o -c /home/robertj/coursework3/master/balancer.cpp

master/CMakeFiles/master.dir/balancer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/master.dir/balancer.cpp.i"
	cd /home/robertj/coursework3/build/master && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/robertj/coursework3/master/balancer.cpp > CMakeFiles/master.dir/balancer.cpp.i

master/CMakeFiles/master.dir/balancer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/master.dir/balancer.cpp.s"
	cd /home/robertj/coursework3/build/master && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/robertj/coursework3/master/balancer.cpp -o CMakeFiles/master.dir/balancer.cpp.s

# Object files for target master
master_OBJECTS = \
"CMakeFiles/master.dir/main.cpp.o" \
"CMakeFiles/master.dir/persistent_kv/kv_storage.cpp.o" \
"CMakeFiles/master.dir/persistent_kv/flexible_page.cpp.o" \
"CMakeFiles/master.dir/persistent_kv/file_editor.cpp.o" \
"CMakeFiles/master.dir/balancer.cpp.o"

# External object files for target master
master_EXTERNAL_OBJECTS =

master/master: master/CMakeFiles/master.dir/main.cpp.o
master/master: master/CMakeFiles/master.dir/persistent_kv/kv_storage.cpp.o
master/master: master/CMakeFiles/master.dir/persistent_kv/flexible_page.cpp.o
master/master: master/CMakeFiles/master.dir/persistent_kv/file_editor.cpp.o
master/master: master/CMakeFiles/master.dir/balancer.cpp.o
master/master: master/CMakeFiles/master.dir/build.make
master/master: /usr/lib/x86_64-linux-gnu/libPocoNet.so.62
master/master: /usr/lib/x86_64-linux-gnu/libPocoUtil.so.62
master/master: /usr/lib/x86_64-linux-gnu/libPocoJSON.so.62
master/master: lib/libLib.a
master/master: /usr/lib/x86_64-linux-gnu/libPocoXML.so.62
master/master: /usr/lib/x86_64-linux-gnu/libPocoFoundation.so.62
master/master: /usr/lib/x86_64-linux-gnu/libpcre.so
master/master: /usr/lib/x86_64-linux-gnu/libz.so
master/master: /usr/lib/x86_64-linux-gnu/libexpat.so
master/master: master/CMakeFiles/master.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/robertj/coursework3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable master"
	cd /home/robertj/coursework3/build/master && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/master.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
master/CMakeFiles/master.dir/build: master/master
.PHONY : master/CMakeFiles/master.dir/build

master/CMakeFiles/master.dir/clean:
	cd /home/robertj/coursework3/build/master && $(CMAKE_COMMAND) -P CMakeFiles/master.dir/cmake_clean.cmake
.PHONY : master/CMakeFiles/master.dir/clean

master/CMakeFiles/master.dir/depend:
	cd /home/robertj/coursework3/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/robertj/coursework3 /home/robertj/coursework3/master /home/robertj/coursework3/build /home/robertj/coursework3/build/master /home/robertj/coursework3/build/master/CMakeFiles/master.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : master/CMakeFiles/master.dir/depend
