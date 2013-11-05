#!/usr/bin/python
#Generate MakeFile

#Const Values
CFLAGS=""
CFLAGS="-rdynamic "
#CFLAGS=" -O3 "
CPP="g++ "
C="gcc "
MIDPATH='build/'
MAIN_PROGRAM='GP_MAIN'
CLIBS=" -ldl "

import os


cmd = "find include/ -name \"*.h\""
include_h = os.popen(cmd).read().split('\n');

import sys
sourceDir = 'src/'

cmd = "find "+sourceDir+" -name \"*.cpp\""
sources_cpp = os.popen(cmd).read().split('\n');
cmd = "find " + sourceDir+" -name \"*.c\""
sources_c = os.popen(cmd).read().split('\n');

sources_cpp.append('./main.cpp')

#Head Files
include_Flag = '-Iinclude'
include = "ALL_INCLUES"
fileContents = include+'='
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

#Main Program
fileContents += ('all: ' + MAIN_PROGRAM + '\n')
#objs
fileContents += (MAIN_PROGRAM + ': ')
for obj in objs:
	fileContents+=' '+MIDPATH+obj
fileContents+='\n'
#Program
fileContents+=('\t' + CPP + CFLAGS + ' -o ' + MAIN_PROGRAM+' ')
for obj in objs:
	fileContents+=(' '+MIDPATH+obj)
fileContents+=CLIBS+'\n'

#All objs' make 
for sequence in sequences:
	fileContents+=sequence

#clean
fileContents+='\nclean:\n\trm '+MIDPATH+'*.o'
fileContents+='\n\trm '+MAIN_PROGRAM

makeFile = open('Makefile','w')
makeFile.write(fileContents)

makeFile.close();
