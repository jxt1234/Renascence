#!/bin/bash
rm -rf temp
find src/ -name "*.h" >> temp
find src/ -name "*.c" >> temp
find src/ -name "*.cpp" >> temp
find include/ -name "*.h" >> temp

while read x
do
	sed -i 's/\t/    /g' $x
done < temp
rm -rf temp
