# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.27

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2023.3.4\bin\cmake\win\x64\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2023.3.4\bin\cmake\win\x64\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Mahmoud\CLionProjects\Processor-design

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Mahmoud\CLionProjects\Processor-design\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/MyExecutable.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/MyExecutable.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/MyExecutable.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MyExecutable.dir/flags.make

CMakeFiles/MyExecutable.dir/main.c.obj: CMakeFiles/MyExecutable.dir/flags.make
CMakeFiles/MyExecutable.dir/main.c.obj: CMakeFiles/MyExecutable.dir/includes_C.rsp
CMakeFiles/MyExecutable.dir/main.c.obj: C:/Users/Mahmoud/CLionProjects/Processor-design/main.c
CMakeFiles/MyExecutable.dir/main.c.obj: CMakeFiles/MyExecutable.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\Mahmoud\CLionProjects\Processor-design\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/MyExecutable.dir/main.c.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.4\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/MyExecutable.dir/main.c.obj -MF CMakeFiles\MyExecutable.dir\main.c.obj.d -o CMakeFiles\MyExecutable.dir\main.c.obj -c C:\Users\Mahmoud\CLionProjects\Processor-design\main.c

CMakeFiles/MyExecutable.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/MyExecutable.dir/main.c.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.4\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Mahmoud\CLionProjects\Processor-design\main.c > CMakeFiles\MyExecutable.dir\main.c.i

CMakeFiles/MyExecutable.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/MyExecutable.dir/main.c.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.4\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Mahmoud\CLionProjects\Processor-design\main.c -o CMakeFiles\MyExecutable.dir\main.c.s

# Object files for target MyExecutable
MyExecutable_OBJECTS = \
"CMakeFiles/MyExecutable.dir/main.c.obj"

# External object files for target MyExecutable
MyExecutable_EXTERNAL_OBJECTS =

Tests/MyExecutable.exe: CMakeFiles/MyExecutable.dir/main.c.obj
Tests/MyExecutable.exe: CMakeFiles/MyExecutable.dir/build.make
Tests/MyExecutable.exe: CMakeFiles/MyExecutable.dir/linkLibs.rsp
Tests/MyExecutable.exe: CMakeFiles/MyExecutable.dir/objects1.rsp
Tests/MyExecutable.exe: CMakeFiles/MyExecutable.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\Users\Mahmoud\CLionProjects\Processor-design\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Tests\MyExecutable.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\MyExecutable.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MyExecutable.dir/build: Tests/MyExecutable.exe
.PHONY : CMakeFiles/MyExecutable.dir/build

CMakeFiles/MyExecutable.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\MyExecutable.dir\cmake_clean.cmake
.PHONY : CMakeFiles/MyExecutable.dir/clean

CMakeFiles/MyExecutable.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Mahmoud\CLionProjects\Processor-design C:\Users\Mahmoud\CLionProjects\Processor-design C:\Users\Mahmoud\CLionProjects\Processor-design\cmake-build-debug C:\Users\Mahmoud\CLionProjects\Processor-design\cmake-build-debug C:\Users\Mahmoud\CLionProjects\Processor-design\cmake-build-debug\CMakeFiles\MyExecutable.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/MyExecutable.dir/depend

