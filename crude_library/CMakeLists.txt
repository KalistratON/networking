project(select_base_proccesing)

file(GLOB select_base_proccesing_filepath "${CMAKE_CURRENT_SOURCE_DIR}/*/*")

add_library(select_base_proccesing SHARED ${select_base_proccesing_filepath})

target_include_directories (select_base_proccesing PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

if(MSVC)
  target_compile_options(select_base_proccesing PRIVATE /W4 /WX)
else()
  target_compile_options(select_base_proccesing PRIVATE -Werror)
endif()


