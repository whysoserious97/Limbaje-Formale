# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.15

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2019.3.5\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2019.3.5\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\wildw\Desktop\Univer\Parser

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\wildw\Desktop\Univer\Parser\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Parser.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Parser.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Parser.dir/flags.make

CMakeFiles/Parser.dir/main.cpp.obj: CMakeFiles/Parser.dir/flags.make
CMakeFiles/Parser.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\wildw\Desktop\Univer\Parser\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Parser.dir/main.cpp.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\Parser.dir\main.cpp.obj -c C:\Users\wildw\Desktop\Univer\Parser\main.cpp

CMakeFiles/Parser.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Parser.dir/main.cpp.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\wildw\Desktop\Univer\Parser\main.cpp > CMakeFiles\Parser.dir\main.cpp.i

CMakeFiles/Parser.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Parser.dir/main.cpp.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\wildw\Desktop\Univer\Parser\main.cpp -o CMakeFiles\Parser.dir\main.cpp.s

# Object files for target Parser
Parser_OBJECTS = \
"CMakeFiles/Parser.dir/main.cpp.obj"

# External object files for target Parser
Parser_EXTERNAL_OBJECTS =

Parser.exe: CMakeFiles/Parser.dir/main.cpp.obj
Parser.exe: CMakeFiles/Parser.dir/build.make
Parser.exe: CMakeFiles/Parser.dir/linklibs.rsp
Parser.exe: CMakeFiles/Parser.dir/objects1.rsp
Parser.exe: CMakeFiles/Parser.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\wildw\Desktop\Univer\Parser\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Parser.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Parser.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Parser.dir/build: Parser.exe

.PHONY : CMakeFiles/Parser.dir/build

CMakeFiles/Parser.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Parser.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Parser.dir/clean

CMakeFiles/Parser.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\wildw\Desktop\Univer\Parser C:\Users\wildw\Desktop\Univer\Parser C:\Users\wildw\Desktop\Univer\Parser\cmake-build-debug C:\Users\wildw\Desktop\Univer\Parser\cmake-build-debug C:\Users\wildw\Desktop\Univer\Parser\cmake-build-debug\CMakeFiles\Parser.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Parser.dir/depend

