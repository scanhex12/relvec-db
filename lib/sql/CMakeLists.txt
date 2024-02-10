find_package(BISON REQUIRED)
BISON_TARGET(SHDBParser parser.y ${CMAKE_CURRENT_SOURCE_DIR}/parser.cpp DEFINES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/parser.hpp)
RAGEL_TARGET(SHDBLexer lexer.l ${CMAKE_CURRENT_SOURCE_DIR}/lexer.cpp)

add_headers_and_sources(shdb_core ${CMAKE_CURRENT_SOURCE_DIR})
add_library(shdb_core ${shdb_core_headers} ${shdb_core_sources} ${BISON_SHDBParser_OUTPUTS} ${RAGEL_SHDBLexer_OUTPUTS})
