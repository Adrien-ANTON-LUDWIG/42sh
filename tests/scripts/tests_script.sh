#!/bin/bash

# Protection against big mistake (as we override the tests with there output)
if [[ ! `pwd` =~ "build"$ ]]
then
    echo "Usage: please run this script only through the 'make script_tests' \
command in a 'build' directory"
    exit 42
fi

count_tests=0
count_passed_tests=0

# Clean the output
echo -ne "\\033[2J\033[0;1f\e[0m"

# Print the text at the center of the terminal
print_center(){
    local x
    local y
    text="$*"
    x=$(( ($(tput cols) - ${#text}) / 2))
    echo -ne "\E[6n";read -sdR y; y=$(echo -ne "${y#*[}" | cut -d';' -f1)
    echo -ne "\033[${y};${x}f$*\e[0m"
    echo ""
}

test_script()
{
    count_tests=$(( $count_tests + 1 ))
    ret_val=0
    (exec "./42sh" "$1" > "actual" 2> "$1_actual_err")
    actual_err=$?
    (exec "bash" "--posix" "$1" > "expected" 2> "$1_expected_err")
    expected_err=$?

    if [ ${actual_err} -ne ${expected_err} ]
    then
        echo -e '\e[1;31m ERROR \e[0m' "$1" "return values: Expected ${expected_err}, got ${actual_err}"
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

for dir in $(find scripts/test_scripts/* -type d);
do
    echo ""
    print_center "\e[1;33m### ${dir}: Tests in progress... ###\e[0m"
    for f in $(find ${dir}/* -name '*.sh');
    do
        test_script $f
    done
done

echo ""
print_center "\e[5;33m$count_passed_tests / $count_tests tests passed\e[0m"
echo ""

exit ${exit_val}
