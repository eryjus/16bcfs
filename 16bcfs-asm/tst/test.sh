CWD=`pwd`
STS=PASSED

echo "=================================================="
echo "               Beginning tests"
echo "=================================================="


for top in */;  do
    for dir in $top*/; do
        echo -n "Executing test ${dir%/}:"
        file="${dir%/}".s
        cd $dir

        if [ -f .expected ]; then
            # Execite the test
            RES=`$CWD/../bin/16bcfs-asm ${file##*/} 2>&1 | diff -wB .expected -`
            RC=$?

            if [ $RC -eq 1 ]; then
            echo ""
            echo $RES
            echo " FAILED!"
            echo ""
            STS=FAILED
            else
            echo " PASSED!"
            fi
        else
            echo " SKIPPED"
        fi

        rm -f msb.bin lsb.bin

        cd $CWD
    done
done


echo "=================================================="
echo "   Testing complete (Overall Status: $STS)"
echo "=================================================="

if [ $STS = "FAILED" ]; then
    false;
fi


