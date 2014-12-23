#!/usr/bin/python
GPSIGN = "/*GP FUNCTION*/"
import os
def collectFiles(path):
    filelist = []
    cmd = "find " + path + " -name \"*.h\""
    for line in os.popen(cmd).read().split('\n'):
        if len(line)>0:
            filelist.append(line)
    return filelist
import re
def analysisHeadFile(filename, functionresult):
    class function:
        def __init__(self):
            self.name = ""
            self.output = ""
            self.inputs = []
        def prints(self):
            print "Function Begin"
            print self.name
            print self.output
            print self.inputs
            print "Function End"
        def valid(self):
            assert(self.output != 'void')
            for v in self.inputs:
                assert(v.find("*")!=-1)
    with open(filename) as f:
        lines = f.read().split("\n")
        for [i, line] in enumerate(lines):
            if line.find(GPSIGN)!=-1:
                line = re.sub('/\*.*\*/', "", line)
                variables = re.findall('\w+[*| ]', line)
                func = function();
                func.name = re.findall('\w+(?= *\()', line)[0]
                print "name:", func.name
                func.output = variables[0]
                for i in range(1, len(variables)):
                    func.inputs.append(variables[i])
                functionresult.append(func)
                func.prints()
def producelist():
    filelist = collectFiles("")
    gpfunctions = []
    for headfile in filelist:
        analysisHeadFile(headfile, gpfunctions)
    return gpfunctions
def constructFunction(function, typeTable=[]):
    name = function.name + "_GPpackage"
    functionline = "GP_Output " + name + "(GP_Input inputs)"
    outputname = function.output.replace("*", '')
    hline = functionline + ";"
    cppline = functionline + "\n{\n"
    inputnumbers = len(function.inputs)
    cppline+="assert("+ '%d' %inputnumbers + " == inputs.size());\n"
    cppline+="GPOutput out;\n"
    for [i, var] in enumerate(function.inputs):
        num = '%d' %i
        cppline+= var + " X" + num + " = (" + var + ")inputs[" + num + '];\n';
    cppline+=outputname + "* result"
    if (function.output.find("*")==-1):
        cppline+=" = new " + outputname + ";\n*"
    cppline+="result = " + function.name + "("
    for i in range(0, inputnumbers-1):
        num = '%d' %i
        cppline+= 'X' + num + ', '
    if (inputnumbers>0):
        num = '%d' %(inputnumbers-1)
        cppline+= 'X'+num
    cppline+=");\n"
#cppline+="out.push(result," + typeTable[outputname].free + ");\n"
    cppline+="out.push(result);\n"
    cppline+="return out;\n"
    cppline += '}\n'
    return [name, hline, cppline]
def determineDependents(allfunctions):
    return
def findAllType(allfunctions):
    return
if __name__=='__main__':
    allfunctions = producelist()
    for f in allfunctions:
        [name, hline, cppline]  = constructFunction(f)
        print name
        print hline
        print cppline
#for f in allfunctions:
