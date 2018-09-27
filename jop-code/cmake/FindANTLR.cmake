# Input Variables:
#  ANTLR_JAR - Path to ANTLR JAR File
# Output Variables:
#  ANTLR_FOUND - Set only if the library was found
# Exported Targets:
#  ANTLR::runtime

find_path(ANTLR_INCLUDE_DIR_RAW
        NAMES antlr4-runtime.h
        PATH_SUFFIXES antlr4-runtime
        )

# Add all subdirectories
list(APPEND ANTLR_INCLUDE_DIR ${ANTLR_INCLUDE_DIR_RAW})
foreach (dirext misc atn dfa tree support)
    string(CONCAT temp ${ANTLR_INCLUDE_DIR_RAW} "/" ${dirext})
    list(APPEND ANTLR_INCLUDE_DIR ${temp})
endforeach (dirext misc atn dfa tree support)

find_library(ANTLR_LIBRARY
        NAMES antlr4-runtime)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ANTLR
        FOUND_VAR ANTLR_FOUND
        REQUIRED_VARS
        ANTLR_LIBRARY
        ANTLR_INCLUDE_DIR
        )

if (ANTLR_FOUND)
    set(ANTLR_LIBRARIES ${ANTLR_LIBRARY})
    set(ANTLR_INCLUDE_DIRS ${ANTLR_INCLUDE_DIR})
endif ()

if (ANTLR_FOUND AND NOT TARGET ANTLR::runtime)
    add_library(ANTLR::runtime UNKNOWN IMPORTED)
    set_target_properties(ANTLR::runtime PROPERTIES
            IMPORTED_LOCATION "${ANTLR_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${ANTLR_INCLUDE_DIR}"
            )

endif ()

