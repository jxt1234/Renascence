#!/usr/bin/python
import os

def getMacro(fileName):
    #Delete '/' '.'
    sta =0
    while sta <len(fileName):
        if (fileName[sta]=='/'):
            break
        sta+=1
    fin = sta+1
    while fin < len(fileName):
        if (fileName[fin]=='.'):
            break
        fin+=1
    fileName = fileName[sta+1 : fin]

    dirs = fileName.split('/')
    macro = ''
    for d in dirs:
        macro += d.upper() + '_'
    macro+= 'H'
    return macro

gHeadS = '/******************************************************************\n   Copyright 2016, Jiang Xiao-tang\n\n   Licensed under the Apache License, Version 2.0 (the "License");\n   you may not use this file except in compliance with the License.\n   You may obtain a copy of the License at\n\n     http://www.apache.org/licenses/LICENSE-2.0\n\n   Unless required by applicable law or agreed to in writing, software\n   distributed under the License is distributed on an "AS IS" BASIS,\n   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.\n   See the License for the specific language governing permissions and\n   limitations under the License.\n******************************************************************/'

def replaceMacro(fileName, macro):
    f = open(fileName)
    lines = f.read().split('\n')
    f.close()
    #Find if the file has macro
    hasmacro = False
    for line in lines:
        if (line.find('_H')!=-1 or line.find('.h')!=-1) and line.find('#ifndef')!=-1:
            hasmacro = True
        elif line.find('Licensed under the Apache License,')!=-1:
            hasmacro = True
        if hasmacro:
            break
    if hasmacro == False:
        print fileName
        fc = gHeadS + '\n'
        fc += '#ifndef ' + macro + '\n'
        fc += '#define ' + macro + '\n'
        for line in lines:
            if len(line) <=0:
                continue
            fc += line + '\n'
        fc += '#endif'+'\n'
        f = open(fileName, 'w')
        f.write(fc);
        f.close()
    return

if __name__=='__main__':
    cmd = 'find include src -name \"*.h\"'
    files = os.popen(cmd).read().split('\n')
    for file in files:
        if len(file) <=0:
            continue
        macro = getMacro(file);
        replaceMacro(file, macro);

