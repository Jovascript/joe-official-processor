function(GenParser
        LexerFile ParserFile ParserNamespace
        )
    project(ANTLRGenParser)
    if (NOT EXISTS "${ANTLR_JAR}")
        message(FATAL_ERROR "Unable to find antlr tool. ANTLR_JAR:${ANTLR_JAR}")
    endif ()

    FIND_PACKAGE(Java COMPONENTS Runtime REQUIRED)
    FIND_PACKAGE(ANTLR REQUIRED)

    set(GEN_SRC_DIR "${CMAKE_CURRENT_BINARY_DIR}/${ParserNamespace}")
    message(STATUS "ANTLR Generated Sources Directory: ${GEN_SRC_DIR}")


    # NAME SOLVING AND PRE-BUILD PATH MAKING
    # EXECUTES COMMANDS IN SETUP *AND* BUILD STAGES
    file(MAKE_DIRECTORY ${GEN_SRC_DIR})
    execute_process(
            COMMAND
            "${Java_JAVA_EXECUTABLE}" -jar "${ANTLR_JAR}" -Werror -Dlanguage=Cpp -listener -visitor -o "${GEN_SRC_DIR}" -package ${ParserNamespace} "${LexerFile}" "${ParserFile}"
    )
    file(GLOB gen_libs "${GEN_SRC_DIR}/*.cpp")
    file(GLOB gen_files "${GEN_SRC_DIR}/*.cpp" "${GEN_SRC_DIR}/*.h")


    # This will generate Lexer, parser, Visitor and Listener at build time
    add_custom_command(
            OUTPUT ${gen_files}
            COMMAND
            "${Java_JAVA_EXECUTABLE}" -jar "${ANTLR_JAR}" -Werror -Dlanguage=Cpp -listener -visitor -o "${GEN_SRC_DIR}" -package ${ParserNamespace} "${LexerFile}" "${ParserFile}"
            DEPENDS ${LexerFile} ${ParserFile}
    )


    add_library(ANTLR-generated)
    target_include_directories(ANTLR-generated INTERFACE "${GEN_SRC_DIR}")
    target_sources(ANTLR-generated PRIVATE "${gen_libs}")

    target_link_libraries(ANTLR-generated PUBLIC ANTLR::runtime)

    # Referenced by main buildfile.
    add_library(ANTLR::generated ALIAS ANTLR-generated)


endfunction()