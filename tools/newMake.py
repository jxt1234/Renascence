#!/usr/bin/python
#Generate MakeFile

import os
import sys
CFLAGS="-O3 -g -fPIC"
if len(sys.argv)>1:
    CFLAGS="-g -fPIC"

#CPP="g++-4.8 -std=c++11 "
#C="gcc-4.8 -std=c11 "
CPP="g++ -std=c++11 "
C="gcc -std=c11 "
MIDPATH='build/'

MAIN_PROGRAM=[]
gDepends = []
gDirs = []
gSrcFiles = []
gLinks = []
include_Flag = ''

gHistoryFile = []

with open('config.txt') as f:
    filc_tt = f.read().split('**********')
    for s in filc_tt[0].split(':')[1].split(' '):
        include_Flag += '-I' + s + ' '

    print include_Flag
    contents = filc_tt[1].split('##########')
    for c in contents:
        lines = c.split('\n')
        lines = [l for l in lines if len(l)>2]
        MAIN_PROGRAM.append(lines[0].split(':')[1])
        gDepends.append(lines[1].split(':')[1])
        gDirs.append([c for c in lines[2].split(':')[1].split(' ') if len(c)>1])
        gSrcFiles.append([c for c in lines[3].split(':')[1].split(' ') if len(c)>1])
        gLinks.append(lines[4].split(':')[1])
    print MAIN_PROGRAM
    print gDirs
    print gSrcFiles

def Generate_Output(outName, srcDirs, srcFiles, CLINK, depend):
    #Find all files
    include_h = []
    sources_cpp = []
    sources_c = []
    for dir in srcDirs:
        cmd = "find include/" + " -name \"*.h\""
        include_h_temp = os.popen(cmd).read().split('\n');
        for f in include_h_temp:
            if len(f) > 0:
                include_h.append(f)
        cmd = "find " + dir + " -name \"*.cpp\""
        temp_cpp = os.popen(cmd).read().split('\n');
        for f in temp_cpp:
            if len(f) > 0:
                sources_cpp.append(f)
        cmd = "find " + dir + " -name \"*.c\""
        temp_c = os.popen(cmd).read().split('\n');
        for f in temp_c:
            if len(f) > 0:
                sources_c.append(f)
    for f in srcFiles:
        sources_cpp.append(f)
    fileContents = ""
    #Head Files
    include = "ALL_INCLUES"+outName
    fileContents += include+'='
    for h in include_h:
        if len(h)>0:
            fileContents+=(' '+h)
    fileContents+='\n'
    def getNameWithofO(fileName):
        return fileName.replace('/', '_').replace('.', '_') + '.o'
    #All cpp c and .o
    objs = []
    sequences = []
    def getSequencesAndAppend(src, program):
        obj = getNameWithofO(src)
        objs.append(obj)
        if src in gHistoryFile:
            return
        gHistoryFile.append(src)
        sequence = MIDPATH + obj + ' : ' + src + " "+depend+" "+' ${'+include + '}\n'
        sequence += '\t' + program + CFLAGS + ' -o ' + MIDPATH + obj + ' -c ' + src + ' '+include_Flag+' \n'
        sequences.append(sequence)
    for cpp in sources_cpp:
        if len(cpp)>0:
            getSequencesAndAppend(cpp, CPP)
    for c in sources_c:
        if len(c)>0:
            getSequencesAndAppend(c, C)
    fileContents+='\n'
    main = outName
    #objs
    fileContents += (main + ': ')
    for obj in objs:
        fileContents+=' '+MIDPATH+obj
    fileContents+= ' ' + depend
    fileContents+='\n'
    #Program
    target = ''
    main_files = ""
    for obj in objs:
        main_files+=(' '+MIDPATH+obj)
    if (main.find('.so')>-1):
        target = ' --shared '
    if (main.find('.a')!=-1):
        fileContents+=('\t' + 'rm -f '  + main+'\n')
        fileContents+=('\t' + 'ar rcs '  + main+' '+main_files)
    else:
        fileContents+=('\t' + CPP + " " + main_files + " " + CFLAGS + ' ' + target  + ' ' + CLINK  + ' -o ' + main)
    fileContents+=(' ${SELF_VARIABLES}' + '\n')
    #All objs' make 
    for sequence in sequences:
        fileContents+=sequence
    return fileContents

files = 'all:'

for program in MAIN_PROGRAM:
    files+= (program + ' ')
files+='\n'

for i in range(0, len(gSrcFiles)):
    files+='\n'
    files+= Generate_Output(MAIN_PROGRAM[i], gDirs[i], gSrcFiles[i], gLinks[i], gDepends[i])

#clean
files+='\nclean:\n\trm '+MIDPATH+'*.o'
for main in MAIN_PROGRAM:
    files+='\n\trm '+main

makeFile = open('Makefile','w')
makeFile.write(files)

makeFile.close();

import os
if False == os.path.exists('build'):
    os.mkdir('build')
