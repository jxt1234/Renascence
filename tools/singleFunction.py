#!/usr/bin/python
import sys
gInput = './res/function_source.txt'
gOutput = './src/math/GPSingleTreeFunction.cpp'
if len(sys.argv)>=3:
    gInput = sys.argv[1]
    gOutput = sys.argv[2]
functions = filter(lambda x:len(x)>0, open(gInput).read().split('\n'))
functions = map(lambda f:filter(lambda x:len(x)>0, f.split(' ')), functions)
print functions

headContents = '#include \"math/GPSingleTreeFunction.h\"\n'
headContents += '#include <math.h>\n'

funcContents = 'int GPSingleTreeFunction::func(const std::string& name)\n{\n'
for i in range(0, len(functions)):
    funcContents += 'if (name == \"' + functions[i][0]+'\") '
    funcContents += 'return %d' %i+';\n'
funcContents += 'return -1;\n}\n'


queryContents = 'std::string GPSingleTreeFunction::query(int func)\n{\n'
for i in range(0, len(functions)):
    queryContents += 'if (func == %d)' %i + ' '
    queryContents += 'return \"' + functions[i][0] + '\";\n'
queryContents += 'return \"\";\n}\n'


inputNumberContents = "int GPSingleTreeFunction::inputNumber(int func)\n{\n"
def getNumber(style):
    maxpos = -1
    for i in range(0, 10):
        si = '%d' %i
        if style.find(si)!=-1:
            maxpos = i
        else:
            break
    return maxpos+1
for i in range(0, len(functions)):
    inputNumberContents += 'if (func == %d)' %i + ' '
    inputNumberContents += 'return %d' %(getNumber(functions[i][1])) + ';\n'
inputNumberContents += 'return -1;\n}\n'

computeContents = 'GPFLOAT GPSingleTreeFunction::compute(int func, GPFLOAT* x)\n{\n'
for i in range(0, len(functions)):
    computeContents += 'if (func == %d)' %i + ' '
    computeContents += 'return ' + functions[i][1] + ';\n'

computeContents += 'return 0;\n}\n'

priorityContents = "int GPSingleTreeFunction::priority(int func)\n{\n"
for i in range(0, len(functions)):
    priorityContents += 'if (func == %d)' %i + ' '
    priorityContents += 'return ' + functions[i][2] + ';\n'
priorityContents += 'return 0;\n}\n'

with open(gOutput, 'w') as fw:
    fw.write(headContents)
    fw.write(funcContents)
    fw.write(queryContents)
    fw.write(inputNumberContents)
    fw.write(computeContents)
    fw.write(priorityContents)
