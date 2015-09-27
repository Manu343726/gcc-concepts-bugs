
project(gcc-concepts-bugs)
cmake_minimum_required(VERSION 3.3)

include(CMakeParseArguments)
include(cmake/utils.cmake)

option(GCB_TESTS_EXPLICIT_ONLY "Include only tests explicitly declared in CMakeLists.txt with gcb_target() calls, ignoring in-source declared tests" OFF)

function(gcb_target)
	cmake_parse_arguments(ARG 
		                  "EXPECTED_SUCCESS;EXPECTED_FAIL"
		                  "TARGET;DESC" 
		                  "SOURCES;COMPILE_OPTIONS;INCLUDE_DIRECTORIES"
		                  ${ARGN})
	if(TARGET __build_${ARG_TARGET})
		message("\"${ARG_TARGET}\" test already defined, this will be ignored")
	else()
		to_fullpath(ARG_SOURCES)
		to_fullpath(ARG_INCLUDE_DIRECTORIES)

		add_executable(__build_${ARG_TARGET} EXCLUDE_FROM_ALL ${ARG_SOURCES})
		target_include_directories(__build_${ARG_TARGET} PRIVATE ${ARG_INCLUDE_DIRECTORIES})
		target_compile_options(__build_${ARG_TARGET} PRIVATE ${ARG_COMPILE_OPTIONS})


		set(command "${CMAKE_COMMAND} --build \".\" --target __build_${ARG_TARGET} --config ${CMAKE_BUILD_TYPE}")

		if(ARG_EXPECTED_FAIL)
			set(command "! ${command}")
		endif()

		# Just to pretty print
		to_space_list(ARG_SOURCES)
		to_space_list(ARG_INCLUDE_DIRECTORIES)
		to_space_list(ARG_COMPILE_OPTIONS)

		message("Adding \"${ARG_TARGET}\" test:")
		message("- Sources: ${ARG_SOURCES}")
		message("- Compile options: ${ARG_COMPILE_OPTIONS}")
		message("- Include directories: ${ARG_INCLUDE_DIRECTORIES}")
		message("- Compiler: ${CMAKE_CXX_COMPILER}")


		add_test(NAME ${ARG_TARGET} COMMAND bash -c ${command} WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
	endif()
endfunction()

function(read_var TEXT RE GROUP VAR)
	if(NOT ${VAR})
		string(REGEX MATCH "${RE}" res "${TEXT}")

		if(res)
			set(${VAR} "${CMAKE_MATCH_${GROUP}}" PARENT_SCOPE)
		else()
			set(${VAR} "" PARENT_SCOPE)
		endif()
	endif()
endfunction()

function(gcb_target_from_file FILE)
	if(EXISTS ${FILE})
		file(STRINGS ${FILE} text)

		foreach(line ${text})
			read_var("${line}" ".*DESCRIPTION: (.*)" 1 DESCRIPTION)
			read_var("${line}" ".*SOURCES: (.*)" 1 SOURCES)
			read_var("${line}" ".*INCLUDE_DIRECTORIES:(.*)" 1 INCLUDE_DIRECTORIES)
			read_var("${line}" ".*COMPILE_OPTIONS: (.*)" 1 COMPILE_OPTIONS)
			read_var("${line}" ".*RESULT: (.*)" 1 RESULT)
		endforeach()
		
		to_cmake_list(SOURCES)
		to_cmake_list(INCLUDE_DIRECTORIES)
		to_cmake_list(COMPILE_OPTIONS)
		
		get_filename_component(FILENAME "${FILE}" NAME)
		get_filename_component(TARGET_NAME "${FILE}" NAME_WE)

		list(FIND SOURCES ${FILENAME} INDEX)
		if(INDEX LESS 0)
			list(INSERT SOURCES 0 ${FILENAME})
		endif()

		gcb_target(TARGET ${TARGET_NAME}
			       DESC "${DESCRIPTION}"
			       SOURCES "${SOURCES}" 
			       INCLUDE_DIRECTORIES "${INCLUDE_DIRECTORIES}"
			       COMPILE_OPTIONS "${COMPILE_OPTIONS}"
			       ${RESULT})
	else()
		message(FATAL_ERROR "File \"${FILE}\" does not exist")
	endif()
endfunction()

function(add_directory_tests)
	if(NOT GCB_TESTS_EXPLICIT_ONLY)
		file(GLOB files "*.cpp")
		foreach(file ${files})
			message(STATUS "Scanning ${file} for in-source test declaration...")
			gcb_target_from_file("${file}")
		endforeach()
	else()
		message(WARNING "in-source declared tests disabled by GCB_TESTS_EXPLICIT_ONLY option")
	endif()
endfunction()

enable_testing()

add_subdirectory(src)

