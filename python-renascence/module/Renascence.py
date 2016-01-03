import RenascenceBasic
class AutoDefFunction:
    def __init__(self, nativeFunction):
        self.nativeFunction = nativeFunction
        self.bestValue = 0.0
    def run(self, inputs):
        return RenascenceBasic.GP_Function_Run(self.nativeFunction, inputs)
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
    def train(self, formula, inputs, types, depth=0, times=2000):
        optinfo = RenascenceBasic.GP_OptimzorInfo_CreateTemplate(depth, times, types, inputs)
        #TODO check formula before transmit to GP
        inputTypes = RenascenceBasic.GP_Contents_Types(inputs)
        inputTypes_python = RenascenceBasic.GP_Strings_Get(inputTypes, 0)
        RenascenceBasic.GP_Strings_Free(inputTypes)
        adf = RenascenceBasic.GP_Function_Create_ByFormula(self.nativeProducer, formula, inputTypes_python, optinfo)
        RenascenceBasic.GP_Function_Optimize(adf, optinfo)
        RenascenceBasic.GP_OptimzorInfo_FreeTemplate(optinfo)
        result = AutoDefFunction(adf)
        bestValue = RenascenceBasic.GP_Function_Run(adf, inputs)
        bestValue_python = RenascenceBasic.GP_Contents_GetDouble(bestValue, 0)
        RenascenceBasic.GP_Contents_Destroy(bestValue)
        return result, bestValue_python
    def build(self, formula):
        adf = RenascenceBasic.GP_Function_Create_ByFormula(self.nativeProducer, formula, "", None)
        result = AutoDefFunction(adf)
        return result
    def type(self, name):
        class GPTypes:
            def __init__(self, name, producer):
                self.nativeProducer = producer
                self.name = name
            def load(self, filename):
                stream = RenascenceBasic.GP_Stream_Create(filename)
                streams = RenascenceBasic.GP_Streams_Malloc(1)
                RenascenceBasic.GP_Streams_Set(streams, stream, 0)
                result = RenascenceBasic.GP_Contents_Load(self.nativeProducer, streams, self.name, 1)
                RenascenceBasic.GP_Stream_Destroy(stream)
                RenascenceBasic.GP_Streams_Free(streams)
                return result
            def save(self, content, filename):
                output = RenascenceBasic.GP_WStream_Create(filename)
                RenascenceBasic.GP_Contents_Save(content, output, 0)
                RenascenceBasic.GP_WStream_Destroy(output)
        result = GPTypes(name, self.nativeProducer)
        return result;
def init(xmlFileList, producerType=0):
    return Producer(xmlFileList, producerType)
def merge(*contents):
    n = len(contents)
    result = RenascenceBasic.GP_Contents_CreateCollector()
    for i in range(0, n):
        c = contents[i]
        l = RenascenceBasic.GP_Contents_Size(c)
        for j in range(0, l):
            RenascenceBasic.GP_Contents_Collect(result, c, j)
    return result
def freeContents(*contents):
    for c in contents:
        RenascenceBasic.GP_Contents_Destroy(c)
def setStreamPath(path):
    RenascenceBasic.GP_Set_Stream_Path(path)
def setLibPath(path):
    RenascenceBasic.GP_Set_Lib_Path(path)
