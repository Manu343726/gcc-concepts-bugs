if [[ ! "$CXX" =~ "concepts" ]]; then 
    echo "Please use GCC with Concepts TS support."
    $CXX --version
    exit 1
fi

mkdir build -p

global_result=0

for test_file in tests/*.cpp; do
    test=$(basename $test_file)
    comnd="$CXX -std=c++1z -lstdc++ -Wall -pedantic -O0 $test_file -o build/${test}.out"
    eval $comnd
    exit_code=$?

    if [[ "$test" =~ "EXPECTED_SUCCESS" ]]; then
        if [[ $exit_code != 0 ]]; then
            echo "Test $test failed:"
            echo " - With command: $comnd"
            echo " - Expected: Compilation Success"

            global_result=1
        else
            echo "Test $test passed"
        fi;
    elif [[ "$test" =~ "EXPECTED_FAILURE" ]]; then
        if [[ $exit_code = 0 ]]; then
            echo "Test $test failed:"
            echo " - With command: $comnd"
            echo " - Expected: Compilation Failure"

            global_result=1
        else
            echo "Test $test passed"
        fi;
    else
        echo "ERROR: Test files should have a EXPECTED_SUCCESS or EXPECTED_FAILURE mark as part of their filename"
        echo "With test file \"$test\""
        echo "Aborting"
        exit 1
    fi;
done

exit $global_result
