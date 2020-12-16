while echo Start
do
    while echo foo
    do
        while echo bar;
        do
            break 2;
            echo No
        done
        echo Niet
    done
    echo End
    break
done
