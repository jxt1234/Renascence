#!/usr/bin/python
FILE='func.xml'
soName = 'libpics.so'
OUTPUT_FUNC='system_func_cpp'
OUTPUT_STATUS='system_status_cpp'
OUTPUT_STATUS_USER_CPP = 'status_user.cpp'
OUTPUT_STATUS_USER_H = 'status_user.h'

statusFunctions = ['alloc','free', 'vary', 'copy','print'];
statusFuncType = ['STATUS_ALLOC', 'STATUS_VARY', 'STATUS_VARY','STATUS_COPY',  'STATUS_PRINT'];
statusFuncNameStyle = ['void* STYLE()', 'void STYLE(void* src)', 'void STYLE(void* src)', 'void STYLE(void* src, void* dst)', 'std::string STYLE(void* src)']

gFunctionTable = []
gStatus = []

#Load all functions and status
f = open(FILE);
content = f.read();
f.close();
def loadAttribute(c, x):
	start = '<'+x+'>'
	end = '</'+x+'>'
	origin = (c.split(start)[1]).split(end)[0].split('\n');
	result = []
	for line in origin:
		if len(line)<=0:
			continue
		newline = line.replace(' ','')
		newline = newline.replace('\t','')
		result.append(newline)
	return result

gFunctionTable = loadAttribute(content, 'functionTable')
gStatus = loadAttribute(content, 'statusTable');
print gFunctionTable
print gStatus

#Generate Function Code
#Machine sign
funcCode = '/*AUTO-GENERATE CODE STYLE*/\n'

#Head files
funcCode += '#include "user/total.h"\n#include <dlfcn.h>\n#include <assert.h>\n'
#Functions
funcCode+='computeFunction* gAllFunctions;\nint gAllFunctionsNumber;\n'
funcNumber = '%d' %(len(gFunctionTable))

#For so 
funcCode+='void* gFunctionHandle=NULL;\n'
#Detroy function
funcCode+='void destroyFunction()\n{\n    dlclose(gFunctionHandle);\n}\n'

#Construct function
funcCode+='void initFunctions()\n{\n    if(NULL==gFunctionHandle) gFunctionHandle = dlopen(\"libpics.so\", RTLD_NOW);\n    assert(NULL!=gFunctionHandle);\n'.replace('libpics.so', soName);
funcCode+='    gAllFunctions = new computeFunction[4];\n    gAllFunctionsNumber = 4;\n'.replace('4', funcNumber)
for i in range(0,len(gFunctionTable)):
	funcCode+='    gAllFunctions['+ '%d' %i + ']=(computeFunction)dlsym(gFunctionHandle, \"'+gFunctionTable[i]+'\");\n'
funcCode+='}\n'
f = open(OUTPUT_FUNC,'w')
f.write(funcCode);
f.close()

#Generate status code
statusCode='/*AUTO-GENERATE CODE STYLE*/\n'
#HEAD file
statusCode += '#include "user/total.h"\n#include <dlfcn.h>\n#include <stdlib.h>\n#include <assert.h>\n#include <string>\n'
#so relative
statusCode+='extern void* gFunctionHandle;\n'
statusCode+='typedef void*(*STATUS_ALLOC)();\n'
statusCode+='typedef void(*STATUS_VARY)(void*);\n'
statusCode+='typedef void(*STATUS_COPY)(void*, void*);\n'
statusCode+='typedef std::string(*STATUS_PRINT)(void*);\n'
statusCode+='void destroyStatus()\n{\n}\n'
statusCode+='vector<int> initStatusType()\n{\n    vector<int> result;\n';


statusCode+='    if(gFunctionHandle==NULL) gFunctionHandle = dlopen(\"'+soName+'\", RTLD_NOW);\n';
for statusName in gStatus:
	for i in range(0,len(statusFunctions)):
		funcName = statusName+'_'+statusFunctions[i]
		code = '    ' + statusFuncType[i] + ' '+ funcName +' = ('+ statusFuncType[i]+')dlsym(gFunctionHandle, \"'+funcName+'\");\n' 
		statusCode+=code;


statusUserCode_cpp = '/*AUTO-GENERATE*/\n#include \"'+OUTPUT_STATUS_USER_H+'\"\n'
statusUserCode_h = '/*AUTO-GENERATE*/\n#include <string>\nextern \"C\"{\n'

def getStatusFunc(statusName):
	hresult = []
	cppresult = []
	for i in range(0,len(statusFunctions)):
		func = statusFunctions[i];
		funcName = statusName+'_'+func;
		code = statusFuncNameStyle[i].replace('STYLE', funcName)
		hcode = code+';\n'
		cppcode = code +'\n{\n}\n'
		hresult.append(hcode)
		cppresult.append(cppcode)
	return hresult,cppresult

#Add all status function
for statusName in gStatus:
	hresult, cppresult = getStatusFunc(statusName)
	for line in hresult:
		statusUserCode_h+=(line+'\n')
	for line in cppresult:
		statusUserCode_cpp+=(line+'\n')

statusUserCode_h+='}\n'
f = open(OUTPUT_STATUS_USER_CPP, 'w')
f.write(statusUserCode_cpp)
f.close()
f = open(OUTPUT_STATUS_USER_H, 'w')
f.write(statusUserCode_h)
f.close()



for statusName in gStatus:
	status_type_code = '    result.push_back(status_allocType(0'
	for func in statusFunctions:
		status_type_code+=', '+statusName+'_'+func;
	status_type_code+='));\n'
	statusCode+=status_type_code

statusCode+='    return result;\n}\n'
f=open(OUTPUT_STATUS, 'w')
f.write(statusCode)
f.close()
