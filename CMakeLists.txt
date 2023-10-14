cmake_minimum_required(VERSION 3.5)
project(keylogger VERSION 0.1.0)

# source files
file(GLOB_RECURSE SRC_FILES ./src/*.c)
add_executable(keylogger ${SRC_FILES})

link_libraries(keylogger wininet)