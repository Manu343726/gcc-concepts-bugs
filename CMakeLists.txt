
project(gcc-concepts-bugs)
cmake_minimum_required(VERSION 3.0)

include(CMakeParseArguments)
include(cmake/utils.cmake)

option(GCB_TESTS_EXPLICIT_ONLY "Include only tests explicitly declared in CMakeLists.txt with gcb_target() calls, ignoring in-source declared tests" OFF)

function(gcb_target)
    cmake_parse_arguments(ARG
                          "" # No options
                          "TARGET;DESC;RESULT;VARIANT_DEFINITION_PREFIX" 
                          "SOURCES;COMPILE_OPTIONS;INCLUDE_DIRECTORIES;VARIANTS"
                          ${ARGN})
    if(TARGET __build_${ARG_TARGET})
        message("\"${ARG_TARGET}\" test already defined, this will be ignored")
    else()
        to_fullpath(ARG_SOURCES)
        to_fullpath(ARG_INCLUDE_DIRECTORIES)

        if(NOT ARG_RESULT)
            set(ARG_RESULT "SUCCESS")
        endif()

        if(NOT ARG_VARIANTS)
            list(APPEND ARG_VARIANTS "DEFAULT")
        else()
            list(FIND ARG_VARIANTS "DEFAULT" HAS_DEFAULT)

            if(HAS_DEFAULT LESS 0)
                list(APPEND ARG_VARIANTS "DEFAULT")
            endif()
        endif()
        list(REMOVE_DUPLICATES ARG_VARIANTS)

        foreach(VARIANT ${ARG_VARIANTS})
            string(REGEX REPLACE "\\." "_" VARIANT "${VARIANT}")
            string(REGEX REPLACE "-" "_" VARIANT "${VARIANT}")

            if(NOT ${VARIANT}_DESCRIPTION)
                set(${VARIANT}_DESCRIPTION "${ARG_DESCRIPTION}")
            endif()
            if(NOT ${VARIANT}_SOURCES)
                set(${VARIANT}_SOURCES "${ARG_SOURCES}")
            endif()
            if(NOT ${VARIANT}_COMPILE_OPTIONS)
                set(${VARIANT}_COMPILE_OPTIONS "${ARG_COMPILE_OPTIONS}")
            endif()
            if(NOT ${VARIANT}_RESULT)
                set(${VARIANT}_RESULT "${ARGS_RESULT}")
            endif()

            add_executable(__build_${ARG_TARGET}_${VARIANT} EXCLUDE_FROM_ALL ${${VARIANT}_SOURCES})
            target_include_directories(__build_${ARG_TARGET}_${VARIANT} PRIVATE ${${VARIANT}_INCLUDE_DIRECTORIES} ${CMAKE_SOURCE_DIR})
            target_compile_options(__build_${ARG_TARGET}_${VARIANT} PRIVATE ${${VARIANT}_COMPILE_OPTIONS} -D${ARG_VARIANT_DEFINITION_PREFIX}${VARIANT} -std=c++1z)
            
            set(command "${CMAKE_COMMAND} --build \".\" --target __build_${ARG_TARGET}_${VARIANT} --config ${CMAKE_BUILD_TYPE}")
            
            if(${VARIANT}_RESULT MATCHES ".*FAIL.*")
                set(command "! ${command}")
            endif()        

            if(NOT TEST_COMMANDS)
                set(TEST_COMMANDS "(${command})")
            else()
                set(TEST_COMMANDS "(${TEST_COMMANDS} && (echo \"\n\n\nRuning variant ${VARIANT}:\nDescription: ${${VARIANT}_DESCRIPTION}\" && ${command}))")
            endif()
        endforeach()
        
        # Just to pretty print
        to_space_list(ARG_SOURCES)
        to_space_list(ARG_INCLUDE_DIRECTORIES)
        to_space_list(ARG_COMPILE_OPTIONS)

        message("Adding \"${ARG_TARGET}\" test:")
        message("- Sources: ${ARG_SOURCES}")
        message("- Compile options: ${ARG_COMPILE_OPTIONS}")
        message("- Include directories: ${ARG_INCLUDE_DIRECTORIES}")
        message("- Compiler: ${CMAKE_CXX_COMPILER}")
        message("- Result: ${RESULT} (${ARG_RESULT})")
        
        add_test(NAME ${ARG_TARGET} COMMAND bash -c "${TEST_COMMANDS}" WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
    endif()
endfunction()

function(read_var TEXT VAR TYPE)
    if(((NOT ${VAR}) OR (TYPE MATCHES "LIST")))
        string(REGEX MATCH ".*${VAR}: (.*)" res "${TEXT}")

        set(value "${CMAKE_MATCH_1}")

        if(res)
            if(TYPE MATCHES LIST)
                list(APPEND ${VAR} "${value}")
                set(${VAR} "${${VAR}}" PARENT_SCOPE)
            else()
                set(${VAR} "${value}" PARENT_SCOPE)
            endif()
        endif()
    endif()
endfunction()

function(parse_variant_decl VARIANT)
    string(REGEX MATCH "(.*):(.*)" res "${VARIANT}")
    set(name "${CMAKE_MATCH_1}")
    set(result "${CMAKE_MATCH_2}")

    if(res)
        set(${VARIANT}_NAME "${name}" PARENT_SCOPE)
        set(${${VARIANT}_NAME}_RESULT "${result}" PARENT_SCOPE)
    else()
        set(${VARIANT}_NAME "${VARIANT}")
        set(${VARIANT}_RESULT "${RESULT}" PARENT_SCOPE)
    endif()

    set(${VARIANT} "${name}" PARENT_SCOPE)
endfunction()

function(parse_vars PREFIX)
    read_var("${line}" "${PREFIX}DESCRIPTION" LIST)
    read_var("${line}" "${PREFIX}SOURCES" LIST)
    read_var("${line}" "${PREFIX}INCLUDE_DIRECTORIES" LIST)
    read_var("${line}" "${PREFIX}COMPILE_OPTIONS" "LIST")
    read_var("${line}" "${PREFIX}RESULT" VARIABLE)

    to_list(${PREFIX}DESCRIPTION \n)
    to_cmake_list(${PREFIX}SOURCES)
    to_cmake_list(${PREFIX}INCLUDE_DIRECTORIES)
    to_cmake_list(${PREFIX}COMPILE_OPTIONS)

    list(FIND ${PREFIX}SOURCES ${FILENAME} INDEX)
    if(INDEX LESS 0)
        list(INSERT ${PREFIX}SOURCES 0 ${FILENAME})
    endif()

    if(NOT PREFIX)
        read_var("${line}" VARIANTS LIST)
        read_var("${line}" VARIANT_DEFINITION_PREFIX VARIABLE)
        read_var("${line}" SKIP VARIABLE)

        to_cmake_list(VARIANTS)
    endif()

    set(${PREFIX}DESCRIPTION "${${PREFIX}DESCRIPTION}" PARENT_SCOPE)
    set(${PREFIX}SOURCES "${${PREFIX}SOURCES}" PARENT_SCOPE)
    set(${PREFIX}INCLUDE_DIRECTORIES "${${PREFIX}INCLUDE_DIRECTORIES}" PARENT_SCOPE)
    set(${PREFIX}COMPILE_OPTIONS "${${PREFIX}COMPILE_OPTIONS}" PARENT_SCOPE)
    set(${PREFIX}RESULT "${${PREFIX}RESULT}" PARENT_SCOPE)
    set(${PREFIX}VARIANTS "${${PREFIX}VARIANTS}" PARENT_SCOPE)
    set(${PREFIX}VARIANT_DEFINITION_PREFIX "${${PREFIX}VARIANT_DEFINITION_PREFIX}" PARENT_SCOPE)
    set(${PREFIX}SKIP "${${PREFIX}SKIP}" PARENT_SCOPE)

    if(VERBOSE_CMAKELISTS)
        message("${PREFIX}DESCRIPTION = ${${PREFIX}DESCRIPTION}")
        message("${PREFIX}SOURCES = ${${PREFIX}SOURCES}")
        message("${PREFIX}INCLUDE_DIRECTORIES = ${${PREFIX}INCLUDE_DIRECTORIES}")
        message("${PREFIX}COMPILE_OPTIONS = ${${PREFIX}COMPILE_OPTIONS}")
        message("${PREFIX}RESULT = ${${PREFIX}RESULT}")
        message("${PREFIX}VARIANTS = ${${PREFIX}VARIANTS}")
        message("${PREFIX}VARIANT_DEFINITION_PREFIX = ${${PREFIX}VARIANT_DEFINITION_PREFIX}")
        message("${PREFIX}SKIP = ${${PREFIX}SKIP}")
    endif()
endfunction()

function(gcb_target_from_file FILE)
    get_filename_component(FILENAME "${FILE}" NAME)
    get_filename_component(TARGET_NAME "${FILE}" NAME_WE)

    if(EXISTS ${FILE})
        file(STRINGS ${FILE} text)

        foreach(line ${text})
            parse_vars("")

            foreach(VARIANT ${VARIANTS})
                parse_variant_decl(${VARIANT})
                parse_vars(${${VARIANT}_NAME}_)

                list(APPEND VARIANT_NAMES "${${VARIANT}_NAME}")
            endforeach()
        endforeach()  

        if(VARIANT_NAMES)
            list(REMOVE_DUPLICATES VARIANT_NAMES)
            set(VARIANTS ${VARIANT_NAMES})
        endif()

        if(NOT SKIP)
            gcb_target(TARGET ${TARGET_NAME}
                       DESC "${DESCRIPTION}"
                       SOURCES "${SOURCES}" 
                       INCLUDE_DIRECTORIES "${INCLUDE_DIRECTORIES}"
                       COMPILE_OPTIONS "${COMPILE_OPTIONS}"
                       RESULT ${RESULT}
                       VARIANTS "${VARIANTS}"
                       VARIANT_DEFINITION_PREFIX "${VARIANT_DEFINITION_PREFIX}")
        endif()
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

