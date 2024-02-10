add_compile_options(-lstdc++fs -lstdc++ -lrt)

add_executable(tests_sql test_sql.cpp)

target_link_libraries(tests_sql PRIVATE shdb_core stdc++fs stdc++ rt)
