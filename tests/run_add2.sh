# !/bin/bash

## Queries the adder instance generator with all possible inputs
## solves the instances and read the addition result and compares
## with the golden result

BW=4 # BitWidth

g++ -std=c++11 add2.cpp ../core/formula.cpp -I ../core -DN=$BW -o add2

M=$(echo "2 ^ $BW - 1" | bc -ql)

failed=0

for x in $(seq 0 $M); do
    for y in $(seq 0 $M); do
        ./add2 $x $y > tmp.cnf
        ~/maplesat tmp.cnf tmp.sol > tmp.out
        z=$(tail -n 1 tmp.sol | awk -vx=$BW 'BEGIN{s=0;}{for(i=x;i>=1;i--) s=s*2+($i<0?0:1);}END{print s}')
        tz=$(( (x+y) % (M+1) ))
        if [ $z -ne $tz ]; then
            echo "$x + $y : $z (should be $tz)"
            failed=$((failed+1))
        fi
    done
done

if [ $failed -eq 0 ]; then
    echo "All tests passed."
fi

rm -f tmp.sol tmp.cnf tmp.out
