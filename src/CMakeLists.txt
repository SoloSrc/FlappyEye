find_package(SDL3 CONFIG REQUIRED)

add_executable(FlappyEye
  "main.c"
)
target_compile_options(FlappyEye PRIVATE
  $<$<C_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<C_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)
target_link_libraries(FlappyEye PRIVATE SDL3::SDL3)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET FlappyEye PROPERTY C_STANDARD 99)
endif()
