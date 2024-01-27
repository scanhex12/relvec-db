cmake_minimum_required(VERSION 3.0)
project(client)

find_package(Poco REQUIRED COMPONENTS Foundation Net Util JSON)
include_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(client main.cpp)
target_link_libraries(client Poco::Foundation Poco::Net Poco::Util Poco::JSON)
