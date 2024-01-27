add_library(PersistentKV file_editor.cpp flexible_page.cpp kv_storage.cpp)
target_include_directories(PersistentKV PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
