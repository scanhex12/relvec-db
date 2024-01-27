cmake_minimum_required(VERSION 3.0)
project(master)

find_package(Poco REQUIRED COMPONENTS Foundation Net Util JSON)

add_executable(master main.cpp)
target_include_directories(master PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(master PRIVATE Poco::Foundation Poco::Net Poco::Util Poco::JSON Lib)

add_subdirectory(persistent_kv)
add_subdirectory(tests)
