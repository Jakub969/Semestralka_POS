# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_SOURCE_DIR = /tmp/tmp.1kRmHROS2c

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/tmp.1kRmHROS2c/cmake-build-frios

# Include any dependencies generated for this target.
include CMakeFiles/server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/server.dir/flags.make

CMakeFiles/server.dir/pos_sockets/active_socket.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/pos_sockets/active_socket.c.o: ../pos_sockets/active_socket.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.1kRmHROS2c/cmake-build-frios/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/server.dir/pos_sockets/active_socket.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/pos_sockets/active_socket.c.o -c /tmp/tmp.1kRmHROS2c/pos_sockets/active_socket.c

CMakeFiles/server.dir/pos_sockets/active_socket.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/pos_sockets/active_socket.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /tmp/tmp.1kRmHROS2c/pos_sockets/active_socket.c > CMakeFiles/server.dir/pos_sockets/active_socket.c.i

CMakeFiles/server.dir/pos_sockets/active_socket.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/pos_sockets/active_socket.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /tmp/tmp.1kRmHROS2c/pos_sockets/active_socket.c -o CMakeFiles/server.dir/pos_sockets/active_socket.c.s

CMakeFiles/server.dir/pos_sockets/char_buffer.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/pos_sockets/char_buffer.c.o: ../pos_sockets/char_buffer.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.1kRmHROS2c/cmake-build-frios/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/server.dir/pos_sockets/char_buffer.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/pos_sockets/char_buffer.c.o -c /tmp/tmp.1kRmHROS2c/pos_sockets/char_buffer.c

CMakeFiles/server.dir/pos_sockets/char_buffer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/pos_sockets/char_buffer.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /tmp/tmp.1kRmHROS2c/pos_sockets/char_buffer.c > CMakeFiles/server.dir/pos_sockets/char_buffer.c.i

CMakeFiles/server.dir/pos_sockets/char_buffer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/pos_sockets/char_buffer.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /tmp/tmp.1kRmHROS2c/pos_sockets/char_buffer.c -o CMakeFiles/server.dir/pos_sockets/char_buffer.c.s

CMakeFiles/server.dir/pos_sockets/passive_socket.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/pos_sockets/passive_socket.c.o: ../pos_sockets/passive_socket.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.1kRmHROS2c/cmake-build-frios/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/server.dir/pos_sockets/passive_socket.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/pos_sockets/passive_socket.c.o -c /tmp/tmp.1kRmHROS2c/pos_sockets/passive_socket.c

CMakeFiles/server.dir/pos_sockets/passive_socket.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/pos_sockets/passive_socket.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /tmp/tmp.1kRmHROS2c/pos_sockets/passive_socket.c > CMakeFiles/server.dir/pos_sockets/passive_socket.c.i

CMakeFiles/server.dir/pos_sockets/passive_socket.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/pos_sockets/passive_socket.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /tmp/tmp.1kRmHROS2c/pos_sockets/passive_socket.c -o CMakeFiles/server.dir/pos_sockets/passive_socket.c.s

CMakeFiles/server.dir/main.c.o: CMakeFiles/server.dir/flags.make
CMakeFiles/server.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.1kRmHROS2c/cmake-build-frios/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/server.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server.dir/main.c.o -c /tmp/tmp.1kRmHROS2c/main.c

CMakeFiles/server.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /tmp/tmp.1kRmHROS2c/main.c > CMakeFiles/server.dir/main.c.i

CMakeFiles/server.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /tmp/tmp.1kRmHROS2c/main.c -o CMakeFiles/server.dir/main.c.s

# Object files for target server
server_OBJECTS = \
"CMakeFiles/server.dir/pos_sockets/active_socket.c.o" \
"CMakeFiles/server.dir/pos_sockets/char_buffer.c.o" \
"CMakeFiles/server.dir/pos_sockets/passive_socket.c.o" \
"CMakeFiles/server.dir/main.c.o"

# External object files for target server
server_EXTERNAL_OBJECTS =

server: CMakeFiles/server.dir/pos_sockets/active_socket.c.o
server: CMakeFiles/server.dir/pos_sockets/char_buffer.c.o
server: CMakeFiles/server.dir/pos_sockets/passive_socket.c.o
server: CMakeFiles/server.dir/main.c.o
server: CMakeFiles/server.dir/build.make
server: CMakeFiles/server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/tmp.1kRmHROS2c/cmake-build-frios/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/server.dir/build: server

.PHONY : CMakeFiles/server.dir/build

CMakeFiles/server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server.dir/clean

CMakeFiles/server.dir/depend:
	cd /tmp/tmp.1kRmHROS2c/cmake-build-frios && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/tmp.1kRmHROS2c /tmp/tmp.1kRmHROS2c /tmp/tmp.1kRmHROS2c/cmake-build-frios /tmp/tmp.1kRmHROS2c/cmake-build-frios /tmp/tmp.1kRmHROS2c/cmake-build-frios/CMakeFiles/server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/server.dir/depend

