cmake_minimum_required(VERSION 3.12)
project(jopemulation VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost REQUIRED program_options filesystem)
include_directories(${Boost_INCLUDE_DIRS})

add_library(jopemulation constants.cpp InputOutputHandler.cpp MemoryHandler.cpp Processor.cpp proc_types.h proc_types.cpp)

target_link_libraries(jopemulation ${Boost_LIBRARIES})
# Make sure the compiler can find include files for our Hello library
# when other libraries or executables link to Hello
target_include_directories (jopemulation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

