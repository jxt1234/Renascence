#!/bin/bash
make -j4
cd ./example/graphic/ && make -j4
cp libpics.so ../../
