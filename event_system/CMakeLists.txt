project(event_system)

file(GLOB event_system_filepath "${CMAKE_CURRENT_SOURCE_DIR}/*/*")

add_library(event_system SHARED ${event_system_filepath})

target_include_directories (event_system PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/inc)

if(MSVC)
  target_compile_options(event_system PRIVATE /W4 /WX)
else()
  target_compile_options(event_system PRIVATE -Werror)
endif()


