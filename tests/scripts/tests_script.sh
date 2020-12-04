#!/bin/bash

count_tests=0
count_passed_tests=0

test_script()
{
    count_tests=$(( $count_tests + 1 ))
    ret_val=0
    (exec "./42sh" "${args}" > "actual")
    actual_err=$?
    (exec "bash" "--posix" "${args}" > "expected")
    expected_err=$?

    if [ ${actual_err} -ne ${expected_err} ]
    then
        echo -e '\e[1;31m ERROR \e[0m' "${args}" "return values do not match"
        ret_val=1
    fi

    diff -u "actual" "expected" > "${args}"
    if [ $? -ne 0 ]
    then
        echo -e '\e[1;31m ERROR \e[0m' "${args}" "wrong output"
        ret_val=1
    fi

    if [ "${ret_val}" -eq 0 ]
    then
        echo -e '\e[1;32m PASSED \e[0m' "${args}"
        count_passed_tests=$(( $count_passed_tests + 1 ))
    else
        exit_val=1
    fi


}

exit_val=0

for args in "scripts/test_scripts/"*;
do
    test_script
done

echo -e "\e[1;33m $count_passed_tests / $count_tests tests passed\e[0;"
exit ${exit_val}