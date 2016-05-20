import RenascenceBasic
class Content:
    def __init__(self, nativeContent=None, producer=None):
        self.producer = producer
        if None == nativeContent:
            self.nativeContent = RenascenceBasic.GP_Contents_CreateCollector()
        elif isinstance(nativeContent, int) or isinstance(nativeContent, float):
            self.nativeContent = RenascenceBasic.GP_Contents_CreateDouble(nativeContent)
        elif isinstance(nativeContent, basestring):
            self.nativeContent = RenascenceBasic.GP_Contents_CreateString(nativeContent)
        else:
            self.nativeContent = nativeContent
        self.referedContents = []
    def __del__(self):
        RenascenceBasic.GP_Contents_Destroy(self.nativeContent)
    def get(self):
        return self.nativeContent
    def type(self):
        inputTypes = RenascenceBasic.GP_Contents_Types(inputs.get())
        inputTypes_python = RenascenceBasic.GP_Strings_Get(inputTypes, 0)
        RenascenceBasic.GP_Strings_Free(inputTypes)
        return inputTypes_python
    def save(self, filename):
        output = RenascenceBasic.GP_WStream_Create(filename)
        RenascenceBasic.GP_Contents_Save(self.nativeContent, output, 0)
        RenascenceBasic.GP_WStream_Destroy(output)
class AutoDefFunction:
    def __init__(self, nativeFunction, producer):
        self.nativeFunction = nativeFunction
        self.producer = producer
        self.bestValue = 0.0
    def __del__(self):
        RenascenceBasic.GP_Function_Destroy(self.nativeFunction)
    def run(self, inputs):
        return Content(RenascenceBasic.GP_Function_Run(self.nativeFunction, inputs.get()), self.producer)
    def ADF(self, name):
        stringsname = RenascenceBasic.GP_Function_GetFormula(self.nativeFunction, name)
        result = RenascenceBasic.GP_Strings_Get(stringsname, 0)
        RenascenceBasic.GP_Strings_Free(stringsname)
        return result
    def parameters(self):
        string_native = RenascenceBasic.GP_Function_GetParameters(self.nativeFunction)
        string_python = RenascenceBasic.GP_Strings_Get(string_native, 0)
        RenascenceBasic.GP_Strings_Free(string_native)
        return string_python

class Producer:
    def __init__(self, xmlFileList, producerType):
        n = len(xmlFileList)
        streams = RenascenceBasic.GP_Streams_Malloc(n)
        for [i, name] in enumerate(xmlFileList):
            stream = RenascenceBasic.GP_Stream_Create(name)
            RenascenceBasic.GP_Streams_Set(streams, stream, i)
        self.nativeProducer = RenascenceBasic.GP_Producer_Create(streams, None, n, producerType)
        for i in range(0, n):
            stream = RenascenceBasic.GP_Streams_Get(streams, i)
            RenascenceBasic.GP_Stream_Destroy(stream)
        RenascenceBasic.GP_Streams_Free(streams)
    def __del__(self):
        RenascenceBasic.GP_Producer_Destroy(self.nativeProducer)
        return
    def content(self, value):
        return Content(value, self)
    def merge(self, *contents):
        n = len(contents)
        resultWrap = Content(None, self)
        result = resultWrap.nativeContent
        for i in range(0, n):
            c = None
            if isinstance(contents[i], int) or isinstance(contents[i], float) or isinstance(contents[i], basestring):
                temp = Content(contents[i], self)
                resultWrap.referedContents.append(temp)
                c = temp.get()
            else:
                resultWrap.referedContents.append(contents[i])
                c = contents[i].get()
            l = RenascenceBasic.GP_Contents_Size(c)
            for j in range(0, l):
                RenascenceBasic.GP_Contents_Collect(result, c, j)
        return resultWrap
    def get(self):
        return self.nativeProducer
    def listAllFunctions(self):
        functionStrings = RenascenceBasic.GP_Producer_ListFunctions(self.nativeProducer)
        n = RenascenceBasic.GP_Strings_Number(functionStrings)
        result = []
        for i in range(0, n):
            result.append(RenascenceBasic.GP_Strings_Get(functionStrings, i))
        RenascenceBasic.GP_Strings_Free(functionStrings)
        return result
    def listAllTypes(self):
        functionStrings = RenascenceBasic.GP_Producer_ListTypes(self.nativeProducer)
        n = RenascenceBasic.GP_Strings_Number(functionStrings)
        result = []
        for i in range(0, n):
            result.append(RenascenceBasic.GP_Strings_Get(functionStrings, i))
        RenascenceBasic.GP_Strings_Free(functionStrings)
        return result
    def train(self, formula, inputs, times=2000, optType=0, depth=0, postFormula=None, postExtraInput=None, cacheFile=None):
        '''
        optType: 0 for Best Value and 1 for least Time
        '''
        postADF = None
        if None != postFormula:
            postADF = RenascenceBasic.GP_Function_Create_ByFormula(self.nativeProducer, postFormula, "", None)
        streamscache = None
        if None != cacheFile:
            streamscache = RenascenceBasic.GP_WStream_Create(cacheFile)
        postExtraInputNative = None
        if None != postExtraInput:
            postExtraInputNative = postExtraInput.get()
        optinfo = RenascenceBasic.GP_OptimzorInfo_CreateTemplate(depth, times, optType, inputs.get(), streamscache, postADF, postExtraInputNative)
        #TODO check formula before transmit to GP
        inputTypes = RenascenceBasic.GP_Contents_Types(inputs.get())
        inputTypes_python = RenascenceBasic.GP_Strings_Get(inputTypes, 0)
        RenascenceBasic.GP_Strings_Free(inputTypes)
        adf = RenascenceBasic.GP_Function_Create_ByFormula(self.nativeProducer, formula, inputTypes_python, optinfo)
        RenascenceBasic.GP_Function_Optimize(adf, optinfo)
        bestValue = RenascenceBasic.GP_OptimzorInfo_TemplateGetBestValue(optinfo)
        RenascenceBasic.GP_OptimzorInfo_FreeTemplate(optinfo)
        if None != cacheFile:
            RenascenceBasic.GP_WStream_Destroy(streamscache)
        if None != postADF:
            RenascenceBasic.GP_Function_Destroy(postADF)
        result = AutoDefFunction(adf, self)
        return result, bestValue
    def build(self, formula, parameters=None):
        adf = RenascenceBasic.GP_Function_Create_ByFormula(self.nativeProducer, formula, "", None)
        if None!=parameters:
            RenascenceBasic.GP_Function_MapParameters(adf, parameters)
        result = AutoDefFunction(adf, self)
        return result
    def load(self, name, filename):
        stream = RenascenceBasic.GP_Stream_Create(filename)
        streams = RenascenceBasic.GP_Streams_Malloc(1)
        RenascenceBasic.GP_Streams_Set(streams, stream, 0)
        result = Content(RenascenceBasic.GP_Contents_Load(self.nativeProducer, streams, name, 1), self)
        RenascenceBasic.GP_Stream_Destroy(stream)
        RenascenceBasic.GP_Streams_Free(streams)
        return result
def init(xmlFileList, producerType=0):
    return Producer(xmlFileList, producerType)
def setStreamPath(path):
    RenascenceBasic.GP_Set_Stream_Path(path)
def setLibPath(path):
    RenascenceBasic.GP_Set_Lib_Path(path)
