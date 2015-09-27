include(cmake/return.cmake)

macro(_map_collect MAP)
	# Garbage collection of keys
	foreach(KEY ${__${MAP}_DEADKEYS})
		# If key-value pair variable is not defined in current and parent scopes, or it is
		# but marked to be collected, unset it. Since we have acess to the direct parent scope only,
		# the variable is not marked as collected (To be collected while climbing the call stack)
		if(${MAP}_${KEY} MATCHES "__${MAP}_${KEY}_DEAD_ENTRY")
			unset(${MAP}_${KEY} PARENT_SCOPE)
			message("${MAP}_${KEY} cleared")
		else()
			# Else the key-value pair variable has a value in the parent scope isolated from map usage, so it can be marked
			# as collected
			list(REMOVE_ITEM __${MAP}_DEADKEYS ${KEY})
			message("${MAP}_${KEY} collected (Value=\"${${MAP}_${KEY}}\")")
		endif()
	endforeach()

	return(__${MAP}_DEADKEYS)
endmacro()

macro(return_map MAP)
	return(__${MAP}_KEYLIST)
	return(__${MAP}_DEADKEYS)

	foreach(KEY ${__${MAP}_KEYLIST})
		return(${MAP}_${KEY})
	endforeach()

	_map_collect(${MAP})
endmacro()

function(map_find_key MAP KEY RESULT)
	list(FIND __${MAP}_KEYLIST "${KEY}" ${RESULT})
	return(${RESULT})
endfunction()

function(_map_set_key MAP KEY)
	map_find_key(${MAP} ${KEY} KEY_INDEX)

	if(KEY_INDEX LESS 0)
		list(APPEND __${MAP}_KEYLIST ${KEY})
	else()
		list(INSERT __${MAP}_KEYLIST ${KEY_INDEX} ${KEY})
	endif()

	return(__${MAP}_KEYLIST)
endfunction()

function(map_set MAP KEY VALUE)
	_map_set_key(${MAP} ${KEY})
	set(${MAP}_${KEY} "${VALUE}")
	return_map(${MAP})
endfunction()

function(map_get MAP KEY RETURN)
	map_find_key(${MAP} ${KEY} KEY_INDEX)

	if(KEY_INDEX LESS 0)
		message(FATAL_ERROR "No \"${KEY}\" key in \"${MAP}\" map.")
	else()
		assign_and_return(${RETURN} "${${MAP}_${KEY}}")
	endif()
endfunction()

function(map_remove MAP KEY)
	map_find_key(${MAP} ${KEY} KEY_INDEX)

	if(KEY_INDEX LESS 0)
		message(FATAL_ERROR "No \"${KEY}\" key in \"${MAP}\" map.")
	else()
		list(REMOVE_AT __${MAP}_KEYLIST ${KEY_INDEX})

		# Mark for garbage collection
		list(APPEND __${MAP}_DEADKEYS ${KEY})
		set(${MAP}_${KEY} "__${MAP}_${KEY}_DEAD_ENTRY")
	endif()

	return_map(${MAP})
	message("Exiting map_remove()")
endfunction()

function(map_keys MAP RESULT)
	assign_and_return(${RESULT} "${__${MAP}_KEYLIST}")
endfunction()

function(map_elems MAP RESULT)
	map_keys(${MAP} KEYS)

	foreach(KEY ${KEYS})
		map_get(${MAP} ${KEY} ELEM)
		list(APPEND elems ${ELEM})
	endforeach()

	assign_and_return(${RESULT} "${elems}")
endfunction()