add_compile_options(-lstdc++fs -lstdc++ -lrt)

add_executable(test_knn test_knn.cpp)

target_link_libraries(test_knn PRIVATE Vectors stdc++fs stdc++ rt)
