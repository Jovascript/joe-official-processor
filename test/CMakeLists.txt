cmake_minimum_required(VERSION 3.12)
project(test)


set(CMAKE_CXX_STANDARD 14)

find_package(Boost REQUIRED program_options filesystem)
include_directories(${Boost_INCLUDE_DIRS})


add_executable(test main.cpp)
# Link the executable to the Hello library. Since the Hello library has
# public include directories we will use those link directories when building
# helloDemo
target_link_libraries (test LINK_PUBLIC jopemulation)
target_link_libraries(jopassembler ${BOOST_LIBRARIES})