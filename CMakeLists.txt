cmake_minimum_required(VERSION 3.0)
project(MyProjects)

find_package(Poco REQUIRED COMPONENTS Foundation Net Util JSON)

add_subdirectory(lib)
#add_subdirectory(tests)
add_subdirectory(master)
add_subdirectory(chunk)
add_subdirectory(client)

# add_executable(master master/main.cpp)
#add_executable(client client/main.cpp)

# target_link_libraries(master PRIVATE Poco::Foundation Poco::Net Poco::Util Poco::JSON Lib)
#target_link_libraries(client PRIVATE Poco::Foundation Poco::Net Poco::Util Poco::JSON Lib)
