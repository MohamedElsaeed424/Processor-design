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
include CMakeFiles/Processor-design.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Processor-design.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Processor-design.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Processor-design.dir/flags.make

CMakeFiles/Processor-design.dir/playground.c.obj: CMakeFiles/Processor-design.dir/flags.make
CMakeFiles/Processor-design.dir/playground.c.obj: CMakeFiles/Processor-design.dir/includes_C.rsp
CMakeFiles/Processor-design.dir/playground.c.obj: C:/Users/Mahmoud/CLionProjects/Processor-design/playground.c
CMakeFiles/Processor-design.dir/playground.c.obj: CMakeFiles/Processor-design.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=C:\Users\Mahmoud\CLionProjects\Processor-design\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Processor-design.dir/playground.c.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.4\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Processor-design.dir/playground.c.obj -MF CMakeFiles\Processor-design.dir\playground.c.obj.d -o CMakeFiles\Processor-design.dir\playground.c.obj -c C:\Users\Mahmoud\CLionProjects\Processor-design\playground.c

CMakeFiles/Processor-design.dir/playground.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/Processor-design.dir/playground.c.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.4\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Mahmoud\CLionProjects\Processor-design\playground.c > CMakeFiles\Processor-design.dir\playground.c.i

CMakeFiles/Processor-design.dir/playground.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/Processor-design.dir/playground.c.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.4\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Mahmoud\CLionProjects\Processor-design\playground.c -o CMakeFiles\Processor-design.dir\playground.c.s

# Object files for target Processor-design
Processor__design_OBJECTS = \
"CMakeFiles/Processor-design.dir/playground.c.obj"

# External object files for target Processor-design
Processor__design_EXTERNAL_OBJECTS =

Processor-design.exe: CMakeFiles/Processor-design.dir/playground.c.obj
Processor-design.exe: CMakeFiles/Processor-design.dir/build.make
Processor-design.exe: CMakeFiles/Processor-design.dir/linkLibs.rsp
Processor-design.exe: CMakeFiles/Processor-design.dir/objects1.rsp
Processor-design.exe: CMakeFiles/Processor-design.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=C:\Users\Mahmoud\CLionProjects\Processor-design\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Processor-design.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Processor-design.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Processor-design.dir/build: Processor-design.exe
.PHONY : CMakeFiles/Processor-design.dir/build

CMakeFiles/Processor-design.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Processor-design.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Processor-design.dir/clean

CMakeFiles/Processor-design.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Mahmoud\CLionProjects\Processor-design C:\Users\Mahmoud\CLionProjects\Processor-design C:\Users\Mahmoud\CLionProjects\Processor-design\cmake-build-debug C:\Users\Mahmoud\CLionProjects\Processor-design\cmake-build-debug C:\Users\Mahmoud\CLionProjects\Processor-design\cmake-build-debug\CMakeFiles\Processor-design.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Processor-design.dir/depend

