find_program(RAGEL_EXECUTABLE NAMES ragel DOC "Path to ragel executable binary")
mark_as_advanced(RAGEL_EXECUTABLE)

if (RAGEL_EXECUTABLE)
    message(STATUS "Found RAGEL: ${RAGEL_EXECUTABLE}")
else()
    message(FATAL_ERROR "Could not find ragel executable")
endif()

macro(RAGEL_TARGET Name Input Output)
    add_custom_command(OUTPUT ${Output}
        COMMAND ${RAGEL_EXECUTABLE}
        ARGS ${Input} -o ${Output}
        DEPENDS ${Input}
        COMMENT "[Ragel][${Name}] Compile state machine with Ragel"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

    set(RAGEL_${Name}_DEFINED TRUE)
    set(RAGEL_${Name}_OUTPUTS ${Output})
    set(RAGEL_${Name}_INPUT ${Input})
endmacro()
