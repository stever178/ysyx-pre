echo "\n==== Running unit tests ====\n"

TEST_LOG="tests/tests.log"
echo "" > $TEST_LOG

for i in tests/*_tests; do
	echo "---- running $i "
    if test -f $i; then
        if [ "$i" = "tests/list_algos_tests" ]; then
            # continue;
            echo;
        fi

        if $VALGRIND ./$i 2>>$TEST_LOG; then
            echo $i PASS
        else
            echo "ERROR in test $i: here's $TEST_LOG"
            echo "------"
            tail $TEST_LOG
            exit 1
        fi
    fi
    echo ""
done
