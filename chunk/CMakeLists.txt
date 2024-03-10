cmake_minimum_required(VERSION 3.0)
project(chunk)

find_package(Poco REQUIRED COMPONENTS Foundation Net Util JSON)
#add_subdirectory(persistent_kv)

file(GLOB LIB_SOURCES ../lib/sql/*.cpp)

add_executable(chunk main.cpp chunk_manager.cpp ${LIB_SOURCES})
target_include_directories(chunk PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(chunk PRIVATE Poco::Foundation Poco::Net Poco::Util Poco::JSON Lib)

#add_subdirectory(tests)
