project(jopcode)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

set(ANTLR_JAR ${CMAKE_CURRENT_SOURCE_DIR}/antlr/antlr-4.7.1-complete.jar)

include(GenerateParser)
GenParser(
        ${CMAKE_CURRENT_SOURCE_DIR}/TLexer.g4 ${CMAKE_CURRENT_SOURCE_DIR}/TParser.g4 T
)

find_package(ANTLR)

add_executable(jopcode src/main.cpp)
target_link_libraries(jopcode
        PRIVATE ANTLR::runtime ANTLR::generated
        )

