cmake_minimum_required(VERSION 3.10)
project(jopassembler)


find_package(Boost REQUIRED COMPONENTS program_options filesystem)


add_executable(jopassembler main.cpp compiler.cpp lexer.cpp compilation_helpers.cpp)

# Link the executable to the Hello library. Since the Hello library has
# public include directories we will use those link directories when building
# helloDemo)
target_link_libraries(jopassembler
        PRIVATE Boost::boost jopemulation Boost::program_options Boost::filesystem)