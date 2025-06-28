function(create_zip out_var)
	set(input_files)
	foreach(in_f ${ARGN})
		list(APPEND input_files "${PROJECT_SOURCE_DIR}/assets/${in_f}")
	endforeach()
	set(out_file "${CMAKE_CURRENT_BINARY_DIR}/${out_var}.hfd")
	add_custom_command(
		COMMAND ${CMAKE_COMMAND} -E tar "cf" "${out_file}" --format=zip -- ${input_files}
		WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/assets"
		OUTPUT  "${out_file}"
		DEPENDS ${input_files}
		COMMENT "Zipping to ${out_file}."
		VERBATIM
	)
	set(${out_var} "${out_file}" PARENT_SCOPE)
endfunction()
