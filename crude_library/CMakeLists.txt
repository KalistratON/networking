project(crude_library)

file(GLOB crude_library_filepath "${CMAKE_CURRENT_SOURCE_DIR}/*/*")

add_library(crude_library SHARED ${crude_library_filepath})

target_include_directories (crude_library PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

if(MSVC)
  target_compile_options(crude_library PRIVATE /W4 /WX)
else()
  target_compile_options(crude_library PRIVATE -Werror)
endif()


