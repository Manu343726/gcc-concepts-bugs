
def FlagsForFile( filename, **kwargs ):
	return {
		'flags': ['-std=c++14', '-Wall', '-pedantic'],
		'do_cache': true
	}