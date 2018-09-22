cmake_minimum_required(VERSION 3.12)
project(jopassembler)

find_package(Boost REQUIRED program_options filesystem)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(jopassembler main.cpp compiler.cpp compiler.h)

# Link the executable to the Hello library. Since the Hello library has
# public include directories we will use those link directories when building
# helloDemo
target_link_libraries (jopassembler LINK_PUBLIC jopemulation)
target_link_libraries(jopassembler ${BOOST_LIBRARIES})