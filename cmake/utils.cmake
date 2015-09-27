function(to_cmake_list VAR)
	string(REGEX REPLACE " " ";" ${VAR} "${${VAR}}")
	string(REGEX REPLACE "," ";" ${VAR} "${${VAR}}")

	set(${VAR} "${${VAR}}" PARENT_SCOPE)
endfunction()

function(to_space_list VAR)
	string(REGEX REPLACE ";" " " ${VAR} "${${VAR}}")
	string(REGEX REPLACE "," " " ${VAR} "${${VAR}}")

	set(${VAR} "${${VAR}}" PARENT_SCOPE)
endfunction()

function(to_comma_list VAR)
	string(REGEX REPLACE " " "," ${VAR} "${${VAR}}")
	string(REGEX REPLACE ";" "," ${VAR} "${${VAR}}")

	set(${VAR} "${${VAR}}" PARENT_SCOPE)
endfunction()

function(to_fullpath LIST)
	foreach(path ${${LIST}})
		get_filename_component(fullpath ${path} ABSOLUTE)
		list(APPEND PATHS ${fullpath})
	endforeach()

	set(${LIST} ${PATHS} PARENT_SCOPE)
endfunction()