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
            print self.name
            print self.output
            for inp in self.inputs:
                print inp
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

producelist()
