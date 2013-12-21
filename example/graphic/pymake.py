#!/usr/bin/python
#Generate MakeFile

import os
import sys
CFLAGS="-O3"
CLINK=""
MAIN_PROGRAM=['GP_MAIN']
if len(sys.argv)>=1:
	for i in range(1, len(sys.argv)):
		MAIN_PROGRAM.append(sys.argv[i])
print MAIN_PROGRAM

CPP="g++ "
C="gcc "
MIDPATH='build/'

cmd = "find include -name \"*.h\""
include_h = os.popen(cmd).read().split('\n');
cmd = "find src -name \"*.cpp\""
sources_cpp = os.popen(cmd).read().split('\n');
cmd = "find src -name \"*.c\""
sources_c = os.popen(cmd).read().split('\n');

sources_c.append('./main.c')

fileContents = 'include make.self\n'

#Head Files
include_Flag = '-Iinclude'
include = "ALL_INCLUES"
fileContents += include+'='
for h in include_h:
	if len(h)>0:
		fileContents+=(' '+h)
fileContents+='\n'

def getNameWithofO(fileName):
	pos = 0
	while fileName[pos]!='/':
		pos+=1
	sta = pos+1
	pos+=1
	while fileName[pos]!='.':
		pos+=1
	result = fileName[sta:pos].replace('/','_')
	return result+'.o'

#All cpp c and .o
objs = []
sequences = []
def getSequencesAndAppend(src, program):
	obj = getNameWithofO(src)
	objs.append(obj)
	sequence = MIDPATH + obj + ' : ' + src + ' ${'+include + '}\n'
	sequence += '\t' + program + CFLAGS + ' -o ' + MIDPATH + obj + ' -c ' + src + ' '+include_Flag+' \n'
	sequences.append(sequence)

for cpp in sources_cpp:
	if len(cpp)>0:
		getSequencesAndAppend(cpp, CPP)
for c in sources_c:
	if len(c)>0:
		getSequencesAndAppend(c, C)

fileContents+='all: '
for main in MAIN_PROGRAM:
	#Main Program
	fileContents += (main + ' ')
fileContents+='\n'
for main in MAIN_PROGRAM:
	#objs
	fileContents += (main + ': ')
	for obj in objs:
		fileContents+=' '+MIDPATH+obj
	fileContents+='\n'
	#Program
	target = ''
	if (main.find('.so')>-1):
		target = ' --share '
	fileContents+=('\t' + CPP + " " + CFLAGS +target+' -o ' + main+' ')
	for obj in objs:
		fileContents+=(' '+MIDPATH+obj)
	fileContents+=(' '+ CLINK  + ' ${SELF_VARIABLES}' + '\n')

#All objs' make 
for sequence in sequences:
	fileContents+=sequence

#clean
fileContents+='\nclean:\n\trm '+MIDPATH+'*.o'
for main in MAIN_PROGRAM:
	fileContents+='\n\trm '+main

makeFile = open('Makefile','w')
makeFile.write(fileContents)

makeFile.close();
