# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/jason/jason2/project/ellipse_detection-master

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jason/jason2/project/ellipse_detection-master

# Include any dependencies generated for this target.
include CMakeFiles/Ellipse_detection.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Ellipse_detection.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Ellipse_detection.dir/flags.make

CMakeFiles/Ellipse_detection.dir/main.cpp.o: CMakeFiles/Ellipse_detection.dir/flags.make
CMakeFiles/Ellipse_detection.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jason/jason2/project/ellipse_detection-master/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Ellipse_detection.dir/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Ellipse_detection.dir/main.cpp.o -c /home/jason/jason2/project/ellipse_detection-master/main.cpp

CMakeFiles/Ellipse_detection.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Ellipse_detection.dir/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jason/jason2/project/ellipse_detection-master/main.cpp > CMakeFiles/Ellipse_detection.dir/main.cpp.i

CMakeFiles/Ellipse_detection.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Ellipse_detection.dir/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jason/jason2/project/ellipse_detection-master/main.cpp -o CMakeFiles/Ellipse_detection.dir/main.cpp.s

CMakeFiles/Ellipse_detection.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/Ellipse_detection.dir/main.cpp.o.requires

CMakeFiles/Ellipse_detection.dir/main.cpp.o.provides: CMakeFiles/Ellipse_detection.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/Ellipse_detection.dir/build.make CMakeFiles/Ellipse_detection.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/Ellipse_detection.dir/main.cpp.o.provides

CMakeFiles/Ellipse_detection.dir/main.cpp.o.provides.build: CMakeFiles/Ellipse_detection.dir/main.cpp.o


# Object files for target Ellipse_detection
Ellipse_detection_OBJECTS = \
"CMakeFiles/Ellipse_detection.dir/main.cpp.o"

# External object files for target Ellipse_detection
Ellipse_detection_EXTERNAL_OBJECTS =

Ellipse_detection: CMakeFiles/Ellipse_detection.dir/main.cpp.o
Ellipse_detection: CMakeFiles/Ellipse_detection.dir/build.make
Ellipse_detection: /usr/local/lib/libopencv_highgui.so.2.4.13
Ellipse_detection: /usr/local/lib/libopencv_imgproc.so.2.4.13
Ellipse_detection: /usr/local/lib/libopencv_core.so.2.4.13
Ellipse_detection: CMakeFiles/Ellipse_detection.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jason/jason2/project/ellipse_detection-master/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Ellipse_detection"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Ellipse_detection.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Ellipse_detection.dir/build: Ellipse_detection

.PHONY : CMakeFiles/Ellipse_detection.dir/build

CMakeFiles/Ellipse_detection.dir/requires: CMakeFiles/Ellipse_detection.dir/main.cpp.o.requires

.PHONY : CMakeFiles/Ellipse_detection.dir/requires

CMakeFiles/Ellipse_detection.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Ellipse_detection.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Ellipse_detection.dir/clean

CMakeFiles/Ellipse_detection.dir/depend:
	cd /home/jason/jason2/project/ellipse_detection-master && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jason/jason2/project/ellipse_detection-master /home/jason/jason2/project/ellipse_detection-master /home/jason/jason2/project/ellipse_detection-master /home/jason/jason2/project/ellipse_detection-master /home/jason/jason2/project/ellipse_detection-master/CMakeFiles/Ellipse_detection.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Ellipse_detection.dir/depend
