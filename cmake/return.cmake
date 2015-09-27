macro(return VARIABLE)
	set(${VARIABLE} "${${VARIABLE}}" PARENT_SCOPE)
endmacro()

macro(assign_and_return VARIABLE NEW_VALUE)
	set(${VARIABLE} "${NEW_VALUE}" PARENT_SCOPE)
endmacro()