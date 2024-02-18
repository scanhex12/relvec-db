add_compile_options(-mavx2)

add_library(Vectors vec_operations.cpp knn_searcher.cpp hnsw.cpp lsh.cpp)
target_include_directories(Vectors PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
