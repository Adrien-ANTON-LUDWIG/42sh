while echo foo;
do
    break KABOOM;
done && echo SHOULD NOT BE PRINTED

echo The previous ast was broken
