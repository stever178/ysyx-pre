TEST_LOG=${1:-"build/tests.log"}

echo "\nLogging to $TEST_LOG\n"

echo "===== Running unit tests =====" | tee $TEST_LOG
echo "valgrind commands : ${VALGRIND}" | tee -a $TEST_LOG

for i in build/*_tests; do
	echo "---- running $i "
    if test -f $i; then
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
