cmake_minimum_required(VERSION 3.10)
project(jopemulation VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)


add_library(jopemulation src/constants.cpp src/InputOutputHandler.cpp src/MemoryHandler.cpp src/Processor.cpp src/proc_types.cpp src/helpers.cpp)


#Set target properties
target_include_directories(jopemulation
        PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        )


target_link_libraries(jopemulation
        PRIVATE Boost::boost)

