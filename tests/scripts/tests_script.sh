#!/bin/bash

count_tests=0
count_passed_tests=0

test_script()
{
    count_tests=$(( $count_tests + 1 ))
    ret_val=0
    (exec "./42sh" "$1" > "actual")
    actual_err=$?
    (exec "bash" "--posix" "$1" > "expected")
    expected_err=$?

    if [ ${actual_err} -ne ${expected_err} ]
    then
        echo -e '\e[1;31m ERROR \e[0m' "$1" "return values do not match"
        ret_val=1
    fi

    diff -u "actual" "expected" > "$1"
    if [ $? -ne 0 ]
    then
        echo -e '\e[1;31m ERROR \e[0m' "$1" "wrong output"
        ret_val=1
    fi

    if [ "${ret_val}" -eq 0 ]
    then
        echo -e '\e[1;32m PASSED \e[0m' "$1"
        count_passed_tests=$(( $count_passed_tests + 1 ))
    else
        exit_val=1
    fi


}

exit_val=0

for f in $(find scripts/test_scripts/* -name '*.sh');
do
    test_script $f
done

echo -e "\e[1;33m $count_passed_tests / $count_tests tests passed\e[0;"
exit ${exit_val}