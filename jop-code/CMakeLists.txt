project(jopcode)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(ANTLR)

add_executable(jopcode src/main.cpp)

target_link_libraries(jopcode
        PRIVATE ANTLR::runtime)

