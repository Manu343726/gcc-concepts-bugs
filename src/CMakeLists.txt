# Explicitly declared tests (Override the in-source ones):

# gcb_target(TARGET Trivial SOURCES Trivial.cpp COMPILE_OPTIONS -std=c++1z EXPECTED_SUCCESS)

# Add in-source declared tests within this directory (Call after explicit tests, so those have preference)
add_directory_tests()
