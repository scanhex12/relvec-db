add_compile_options(-lstdc++fs -lstdc++ -lrt)

add_executable(tests_vectors test_vectors.cpp)

target_link_libraries(tests_vectors PRIVATE Vectors stdc++fs stdc++ rt)
