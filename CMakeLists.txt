cmake_minimum_required (VERSION 3.8)

project ("networking")

add_subdirectory ("crude_library")
add_subdirectory ("test")

target_include_directories (networking_exe PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/crude_library/inc)

