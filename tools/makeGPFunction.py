#!/usr/bin/python
GPSIGN = "/*GP FUNCTION*/"
GPSTATUSSIGN="/*S*/"
import os
def collectFiles(path):
    filelist = []
    cmd = "find " + path + " -name \"*.h\""
    for line in os.popen(cmd).read().split('\n'):
        if len(line)>0:
            filelist.append(line)
    return filelist
def renameFunction(name):
    return name+"_GPpackage"
def turnFree(name):
    return name+"_GPFree"
import re
def analysisHeadFile(filename, functionresult):
    findfunc = False
    class function:
        def __init__(self):
            self.name = ""
            self.output = ""
            self.inputs = []
            self.status = []
            self.inputPos = []
            self.statusPos = []
        def prints(self):
            print "Function Begin"
            print 'name: ', self.name
            print 'output: ', self.output
            print 'input: ', self.inputs
            print 'status: ', self.status
            print 'inputPos: ', self.inputPos
            print 'statusPos: ', self.statusPos
            print "Function End"
        def valid(self):
            assert(self.output != 'void')
            for v in self.inputs:
                assert(v.find("*")!=-1)
    with open(filename) as f:
        lines = f.read().split("\n")
        for [i, line] in enumerate(lines):
            if line.find(GPSIGN)!=-1:
                newline = line.replace(GPSTATUSSIGN, '')
                findfunc = True
                newline = re.sub('/\*.*\*/', "", newline)
                variables = re.findall('\w+[*| ]', newline)
                func = function();
                func.name = re.findall('\w+(?= *\()', newline)[0]
                func.name = func.name.replace(' ', '')
                func.output = variables[0].replace(' ', '')
                line = line.split(GPSIGN)[1]
                words = line.split(',')
                for i in range(1, len(variables)):
                    if words[i-1].find(GPSTATUSSIGN)!=-1:
                        func.status.append(variables[i].replace(' ', ''))
                        func.statusPos.append(i-1)
                    else:
                        func.inputs.append(variables[i].replace(' ', ''))
                        func.inputPos.append(i-1)
                functionresult.append(func)
    return findfunc
def producelist():
    filelist = collectFiles("include")
    headlist = []
    gpfunctions = []
    for headfile in filelist:
        if True == analysisHeadFile(headfile, gpfunctions):
            headlist.append(headfile)
    return [headlist, gpfunctions]
def constructFunction(function):
    name = renameFunction(function.name)
    functionline = "GPContents* " + name + "(GPContents* inputs)"
    outputname = function.output.replace("*", '')
    hline = functionline + ";\n"
    cppline = functionline + "\n{\n"
    inputnumbers = len(function.inputs) + len(function.status)
    cppline+="assert("+ '%d' %inputnumbers + " == inputs->size());\n"
    for [i, inp] in enumerate(function.inputs):
        cppline+='assert(inputs->contents[%d].type == g' %(function.inputPos[i]) + inp.replace('*', '') + ');\n'
    for [i, inp] in enumerate(function.status):
        cppline+='assert(inputs->contents[%d].type == g' %(function.statusPos[i]) + inp.replace('*', '') + ');\n'
    cppline+="GPContents* out =  new GPContents;\n"
    for [i, var] in enumerate(function.inputs):
        num = '%d' %(function.inputPos[i])
        cppline+= var + " X" + num + " = (" + var + ")inputs->get(" + num + ');\n';
    for [i, var] in enumerate(function.status):
        num = '%d' %(function.statusPos[i])
        cppline+= var + " X" + num + " = (" + var + ")inputs->get(" + num + ');\n';
    cppline+=outputname + "* result"
    if (function.output.find("*")==-1):
        cppline+=" = new " + outputname + ";\n*result"
    cppline+=" = " + function.name + "("
    for i in range(0, inputnumbers-1):
        num = '%d' %i
        cppline+= 'X' + num + ', '
    if (inputnumbers>0):
        num = '%d' %(inputnumbers-1)
        cppline+= 'X'+num
    cppline+=");\n"
    cppline+="out->push(result,g" + outputname + ");\n"
    cppline+="return out;\n"
    cppline += '}\n'
    return [hline, cppline]
def determineDependents(allfunctions):
    return
def findAllType(allfunctions):
    outputtable = []
    inputtable = []
    for f in allfunctions:
        outputtable.append(f.output.replace('*', ''))
        for v in f.inputs:
            inputtable.append(v.replace('*', ''))
        for v in f.status:
            inputtable.append(v.replace('*', ''))
    return [list(set(outputtable)), list(set(inputtable))]


HEADFILE = "src/package/GPPackage.h"
CPPFILE = "src/package/GPPackage.cpp"
XMLFILE = "AUTOGP.xml"
TYPEHEADFILE = "src/package/GPTypes.h"
TYPECPPFILE = "src/package/GPTypes.cpp"

def generateFunctionfiles(filelist, functions):
    hfile = "#ifndef SRC_PACKAGE_GPFUNCTIONS_H\n"
    hfile += "#define SRC_PACKAGE_GPFUNCTIONS_H\n"
    hfile += "#include \"user/GPContents.h\"\n"
    hfile += "extern \"C\"{\n"
    cppfile = "#include \"GPPackage.h\"\n"
    cppfile += "#include \"GPTypes.h\"\n"
    cppfile += "#include <assert.h>\n"
    for f in filelist:
        cppfile+="#include \"" + f.replace("include/", "") + "\"\n"
    for f in allfunctions:
        [hline, cppline]  = constructFunction(f)
        hfile += hline
        cppfile+=cppline
    hfile+="}\n#endif\n"
    with open(HEADFILE, 'w') as f:
        f.write(hfile);
    with open(CPPFILE, 'w') as f:
        f.write(cppfile)
def generateTypeFiles(filelist, outputt, inputt):
    #Output is to make free callback function, input is to make status class
    hfile = "#ifndef SRC_PACKAGE_GPTYPES_H\n"
    hfile += "#define SRC_PACKAGE_GPTYPES_H\n"
    hfile +="#include \"user/IStatusType.h\"\n"
    totoaltype = set(outputt + inputt)
    for t in totoaltype:
        hfile += "extern IStatusType* g"+t+";\n"
    hfile += "extern \"C\"{\n"
    hfile +="IStatusType* GP_IStatusType_Create(const std::string& name);\n"
    hfile+="}\n#endif\n"
    cppfile = "#include \"GPTypes.h\"\n"
    for f in filelist:
        cppfile+="#include \"" + f.replace("include/", "") + "\"\n"
    def turnType(n):
        return n+"_GPType"
    for t in totoaltype:
        cppfile+="class "+turnType(t) + ":public IStatusType\n{\n"
        cppfile+='public:\n'
        cppfile+=turnType(t)+'():IStatusType(\"' + t +"\"){}\n"
        cppfile+='virtual void* vLoad(GPStream* input) const\n{\nreturn NULL;\n}\n'
        cppfile+='virtual void vSave(void* contents, GPWStream* output) const\n{\n}\n'
        cppfile+='virtual void vFree(void* contents) const\n{\n'
        cppfile+=t+'* c = ('+t+'*)contents;\nc->decRef();\n'
        cppfile+='}\n'
        cppfile+='virtual int vMap(void** content, double* value) const\n{\nint mapnumber=0;\n'
        cppfile+='if (NULL == value || NULL == content)\n{\nreturn mapnumber;\n}\nif (NULL == *content)\n{\n}\n'
        cppfile+='return mapnumber;\n}\n'
        cppfile+='virtual bool vCheckCompleted(void* content) const {return NULL!=content;}\n'
        cppfile+='virtual bool vMerge(void* dst, void* src) const {return false;}\n'
        cppfile+="};\n"
        cppfile+="IStatusType* g" + t + " = new "+turnType(t) + "();\n"
    cppfile+='IStatusType* GP_IStatusType_Create(const std::string& name)\n{\n'
    for t in totoaltype:
        cppfile+='if (name == \"'+t + '\")\n{\n'
        cppfile+='return g' + t + ';\n}\n'
    cppfile+='return NULL;\n}\n'
    with open(TYPEHEADFILE, 'w') as f:
        f.write(hfile);
    with open(TYPECPPFILE, 'w') as f:
        f.write(cppfile)
    return
def generateXML(functions):
    xmlcontents = '<NULL>\n'
    for func in functions:
        xmlcontents += '<'+renameFunction(func.name)+'>\n'
        xmlcontents += '<output>' + func.output.replace('*', '') + '</output>\n'
        xmlcontents += '<status>'
        for t in func.status:
            xmlcontents += t.replace('*', '') + ' '
        xmlcontents += '</status>\n'
        xmlcontents += '<input>'
        for t in func.inputs:
            xmlcontents += t.replace('*', '') + ' '
        xmlcontents += '</input>\n'
        xmlcontents += '</'+renameFunction(func.name)+'>\n\n'
    xmlcontents += '</NULL>\n'
    return xmlcontents


import sys
if __name__=='__main__':
    [filelist, allfunctions] = producelist()
    [outputtype, inputtype] = findAllType(allfunctions)
    if (not os.path.exists("src/package")):
        os.makedirs("src/package")
    generateFunctionfiles(filelist, allfunctions);
    generateTypeFiles(filelist, outputtype, inputtype)
    #Default Function Table
    if (not os.path.exists("include/package")):
        os.makedirs("include/package")
    classname = 'DefaultFunctionTable'
    #Generate head contents
    headcontent = ''
    headcontent += '#include \"user/IFunctionTable.h\"\n'
    headcontent += 'class ' + classname + ': public IFunctionTable\n'
    headcontent += '{\n' + 'public:\n' + 'virtual void* vGetFunction(const std::string& name);\n' + '};\n'
    with open('include/package/'+classname+'.h', 'w') as f:
        f.write(headcontent)
    cppcontent = ''
    cppcontent += '#include \"package/DefaultFunctionTable.h\"\n'
    cppcontent += '#include \"GPPackage.h\"\n'
    cppcontent += '#include \"GPTypes.h\"\n'
    cppcontent += 'void* ' + classname + '::vGetFunction(const std::string& name)\n{\n'
    for f in allfunctions:
        fname = renameFunction(f.name)
        cppcontent += 'if (name == \"' + fname + '\")\n{\n' + 'return (void*)' + fname + ';\n}\n'
    fname = 'GP_IStatusType_Create' 
    cppcontent += 'if (name == \"' + fname + '\")\n{\n' + 'return (void*)' + fname + ';\n}\n'
    cppcontent += 'return NULL;\n}\n'
    with open('src/package/'+ classname + '.cpp', 'w') as f:
        f.write(cppcontent)

    xmlcontents = generateXML(allfunctions)
    with open('DefaultFunctionTable.xml', 'w') as f:
        f.write(xmlcontents)
