mkdir build -p

global_result=0
CXX=g++-concepts
SRC_FOLDER=src

for test_file in ${SRC_FOLDER}/*.cpp; do
    test=$(basename $test_file)
    comnd="$CXX -std=c++1z -lstdc++ -Wall -Werror -pedantic -O0 $test_file -o build/${test}.out -L/usr/local/lib/gcc/x86_64-pc-linux-gnu/lib64/"
    eval $comnd
    exit_code=$?

    if [[ "$test" =~ "STD_CORRECT" ]]; then
        if [[ "$test" =~ "EXPECTED_CONFORMANT" ]]; then
            if [[ $exit_code != 0 ]]; then
                echo "Test $test failed:"
                echo " - With command: $comnd"
                echo " - Expected: Compilation Success (The example is correct by the Standard and the compiler was expected to be conformant. This may be a compiler bug)"

                global_result=1
            else
                echo "Test $test passed (Correct by the Standard and the compiler is conformant)"
            fi;
        elif [[ "$test" =~ "EXPECTED_NON_CONFORMANT" ]]; then
            if [[ $exit_code = 0 ]]; then
                echo "Test $test failed:"
                echo " - With command: $comnd"
                echo " - Expected: Compilation Failure (The example is correct by the Standard but the compiler was expected to be non-conformant. This may be an error in the example specification)"

                global_result=1
            else
                echo "Test $test passed (Correct by the Standard but the compiler is non-conformant as expected)"
            fi;
        else
            echo "ERROR: Test files should have a EXPECTED_CONFORMANT or EXPECTED_NON_CONFORMANT mark as part of their filename"
            echo "With test file \"$test\""
            echo "Aborting"
            exit 1
        fi;
    elif [[ "$test" =~ "STD_INCORRECT" ]]; then
        if [[ "$test" =~ "EXPECTED_CONFORMANT" ]]; then
            if [[ $exit_code = 0 ]]; then
                echo "Test $test failed:"
                echo " - With command: $comnd"
                echo " - Expected: Compilation Failure (The example is incorrect by the Standard and the compiler was expected to be conformant. This may be a compiler bug)"

                global_result=1
            else
                echo "Test $test passed (Incorrect by the Standard and the compiler is conformant)"
            fi;
        elif [[ "$test" =~ "EXPECTED_NON_CONFORMANT" ]]; then
            if [[ $exit_code != 0 ]]; then
                echo "Test $test failed:"
                echo " - With command: $comnd"
                echo " - Expected: Compilation Failure (The example is incorrect by the Standard but the compiler was expected to be non-conformant. This may be an error in the example specification)"

                global_result=1
            else
                echo "Test $test passed (Incorrect by the Standard but the compiler is non-conformant as expected)"
            fi;
        else
            echo "ERROR: Test files should have a EXPECTED_CONFORMANT or EXPECTED_NON_CONFORMANT mark as part of their filename"
            echo "With test file \"$test\""
            echo "Aborting"
            exit 1
        fi;
    else
        echo "ERROR: Test files should have a STD_CORRECT or STD_INCORRECT mark as part of their filename"
        echo "With test file \"$test\""
        echo "Aborting"
        exit 1
    fi;
done

exit $global_result
