cmake_minimum_required(VERSION 3.10)
project(jopemulator)

add_executable(jopemulator main.cpp)

# Link the executable to the Hello library. Since the Hello library has
# public include directories we will use those link directories when building
# helloDemo)
target_link_libraries(jopemulator
        PRIVATE Boost::boost jopemulation Boost::program_options Boost::filesystem spdlog::spdlog)