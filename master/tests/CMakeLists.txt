add_compile_options(-lstdc++fs -lstdc++ -lrt)

add_executable(tests_kv test_persistent_kv.cpp)

target_link_libraries(tests_kv PRIVATE PersistentKV stdc++fs stdc++ rt)
