var=foo

echo $var

for var in a b c
do
    echo $var
    var=bar
    echo $var
done

echo $var

for var in yetanothervar
do
    echo $var
done

echo $var

for newvar in stillthere?
do
    echo $newvar
done

echo $newvar
