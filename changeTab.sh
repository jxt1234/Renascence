#!/bin/bash
rm -rf temp
find -name "*.h" >> temp
find -name "*.c" >> temp
find -name "*.cpp" >> temp

while read x
do
	sed -i 's/\t/    /g' $x
done < temp
rm -rf temp
