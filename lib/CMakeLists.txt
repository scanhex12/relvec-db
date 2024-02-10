include(cmake/glob_sources.cmake)
include(cmake/ragel.cmake)

add_library(Lib serialize.cpp)
target_include_directories(Lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(sql)
add_subdirectory(tests)
