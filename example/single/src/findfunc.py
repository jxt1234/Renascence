#!/usr/bin/python
import os
import sys
if len(sys.argv)<3:
	print "Usage: ./findfunc.py ${input1} ${input2} ... ${output}"

l = len(sys.argv) - 1

outFile = sys.argv[len(sys.argv)-1];

func = []
for i in range(1, l):
	inpFile = sys.argv[i];
	f = open(inpFile)
	lines = f.read().split("\n")
	f.close()
	
	for line in lines:
		fin = line.find("(")
		if fin==-1:
			continue
		sta = fin -1
		while line[sta]!=' ' and line[sta]!='\t':
			sta-=1
		func.append(line[sta+1:fin])

result =''
for func_ in func:
	result+="\""+func_+'\",\n'
for func_ in func:
	result+=func_+',\n'
f=open(outFile, 'w')
f.write(result)
f.close()
