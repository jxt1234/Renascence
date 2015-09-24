#!/usr/bin/python
import os
import sys

if len(sys.argv)<2:
	print "Usage: ./GenerateTest.py XXXXX"

model = 'model'
name = 'GP'+sys.argv[1]+'Test'
default = 'XXXXXXXXXXXXXX'

f = open(model,'r')
content = f.read();
f.close()
content = content.replace(default, name);
out = open(name+".cpp", 'w')
out.write(content);
out.close()
