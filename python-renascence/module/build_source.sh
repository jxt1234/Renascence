#!/bin/bash
cp ../../swig/renascence_basic.i .
swig -c++ -python  -outdir . renascence_basic.i
rm ./renascence_basic.i
