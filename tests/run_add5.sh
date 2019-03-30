# !/bin/bash

## Queries the adder instance generator with all possible inputs
## solves the instances and read the addition result and compares
## with the golden result

BW=8 # BitWidth
ntests=500
adderType=TWO_OPERAND # COUNTER_CHAIN ESPRESSO DOT_MATRIX

g++ -std=c++11 add5.cpp ../core/formula.cpp -I ../core -DN=$BW -DATYPE=$adderType -o add5

M=$(echo "2 ^ $BW" | bc -ql)

failed=0

for t in $(seq $ntests); do
    a=$((RANDOM % M))
    b=$((RANDOM % M))
    c=$((RANDOM % M))
    d=$((RANDOM % M))
    e=$((RANDOM % M))
    ./add5 $a $b $c $d $e > tmp.cnf
    ~/maplesat tmp.cnf tmp.sol > tmp.out
    z=$(tail -n 1 tmp.sol | awk -vx=$BW 'BEGIN{s=0;}{for(i=x;i>=1;i--) s=s*2+($i<0?0:1);}END{print s}')
    tz=$(( (a+b+c+d+e) % M ))
    if [ $z -ne $tz ]; then
        echo "$a + $b + $c + $d + $e: $z (should be $tz)"
        failed=$((failed+1))
    fi
done

if [ $failed -eq 0 ]; then
    echo "All tests passed."
fi

rm -f tmp.sol tmp.cnf tmp.out
