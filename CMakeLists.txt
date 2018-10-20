cmake_minimum_required(VERSION 3.10)
project(JOP)

set(CMAKE_CXX_STANDARD 14)

add_subdirectory(includes/spdlog)
add_subdirectory(jop-emulation)
add_subdirectory(jop-emulator)
add_subdirectory(jop-assembler)
add_subdirectory(jop-code)
