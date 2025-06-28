find_package(libzip CONFIG REQUIRED)
find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)

create_zip(assets
	"sprites/flappy.png"
)

add_executable(FlappyEye
	"main.c"
	"fe_app.h"
	"fe_app.c"
	"fe_img.c"
	${assets})

target_compile_options(FlappyEye PRIVATE
	$<$<C_COMPILER_ID:MSVC>:/W4 /WX>
	$<$<NOT:$<C_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

target_link_libraries(FlappyEye PRIVATE libzip::zip)
target_link_libraries(FlappyEye PRIVATE SDL3::SDL3)
target_link_libraries(FlappyEye PRIVATE
	$<IF:$<TARGET_EXISTS:SDL3_image::SDL3_image-shared>,
		SDL3_image::SDL3_image-shared,
		SDL3_image::SDL3_image-static>
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
	set_property(TARGET FlappyEye PROPERTY C_STANDARD 99)
endif()
