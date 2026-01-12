TEST_LOG=${1:-"build/tests.log"}

echo "\nLogging to $TEST_LOG\n"

echo "===== Running unit tests =====" | tee $TEST_LOG

for i in build/*_tests; do
    LOG_FILE=$(basename ${i})
    echo "---- running ${LOG_FILE}"
    VALGRIND_CMD=$(echo "${VALGRIND}" | sed "s/todo/${LOG_FILE}/g")
    echo "Command: ${VALGRIND_CMD} ..." | tee -a $TEST_LOG

    if test -f $i; then
        if ${VALGRIND_CMD} ./$i 2>>$TEST_LOG; then
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
