cmake_minimum_required(VERSION 3.0)
project(client)

find_package(Poco REQUIRED COMPONENTS Foundation Net Util JSON)
#add_subdirectory(persistent_kv)
file(GLOB LIB_SOURCES ../lib/sql/*.cpp)

add_executable(client main.cpp client_config.cpp client_getter.cpp ${LIB_SOURCES})
target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(client PRIVATE Poco::Foundation Poco::Net Poco::Util Poco::JSON Lib)

#add_subdirectory(tests)
