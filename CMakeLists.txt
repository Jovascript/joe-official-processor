cmake_minimum_required(VERSION 3.10)
project(JOP)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost REQUIRED COMPONENTS program_options filesystem)
add_subdirectory(includes/spdlog)
add_subdirectory(jop-emulation)
add_subdirectory(jop-emulator)
add_subdirectory(jop-assembler)
