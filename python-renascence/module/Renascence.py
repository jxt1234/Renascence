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
    def size(self):
        return RenascenceBasic.GP_Contents_Size(self.nativeContent)
    def __del__(self):
        RenascenceBasic.GP_Contents_Destroy(self.nativeContent)
    def get(self):
        return self.nativeContent
    def type(self):
        inputTypes = RenascenceBasic.GP_Contents_Types(inputs.get())
        inputTypes_python = RenascenceBasic.GP_Strings_Get(inputTypes, 0)
        RenascenceBasic.GP_Strings_Free(inputTypes)
        return inputTypes_python
    def save(self, filename, pos=0):
        output = RenascenceBasic.GP_WStream_Create(filename)
        RenascenceBasic.GP_Contents_Save(self.nativeContent, output, pos)
        RenascenceBasic.GP_WStream_Destroy(output)
    def dump(self, pos=0):
        contentString = RenascenceBasic.GP_Contents_Dump(self.nativeContent, pos)
        content = RenascenceBasic.GP_Strings_Get(contentString, 0)
        RenascenceBasic.GP_Strings_Free(contentString)
        return content
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

class Piece:
    def __init__(self, piece, producer):
        self.nativePiece = piece
        self.producer = producer
    def __del__(self):
        RenascenceBasic.GP_Pieces_Destroy(self.nativePiece)
    def get(self):
        return self.nativePiece

class PieceArray:
    def __init__(self, pieces):
        self.pieces = pieces
        n = len(pieces)
        self.nativePieceArray = RenascenceBasic.GP_Pieces_CreateArray(n)
        for i in range(0, n):
            RenascenceBasic.GP_Pieces_Array_Set(self.nativePieceArray, pieces[i].get(), i)
    def __del__(self):
        RenascenceBasic.GP_Pieces_Array_Free(self.nativePieceArray)

    def get(self):
        return self.nativePieceArray
    def size(self):
        return len(self.pieces)

class PieceFunction:
    def __init__(self, nativeFunction, producer):
        self.nativeFunction = nativeFunction
        self.producer = producer
    def __del__(self):
        RenascenceBasic.GP_PiecesFunction_Destroy(self.nativeFunction)
    def run(self, pieces):
        pieceArray = PieceArray(pieces)
        return Piece(RenascenceBasic.GP_PiecesFunction_Run(self.nativeFunction, pieceArray.get(), pieceArray.size()), self.producer)

class PieceFunctionProducer:
    def __init__(self, basicProducer, pieceLibMetaFileList, mapReduceMetaFileList):
        self.basicProducer = basicProducer
        libNumber = len(pieceLibMetaFileList)
        libStreams = RenascenceBasic.GP_Streams_Malloc(libNumber)
        mapReduceMetaNumber = len(mapReduceMetaFileList)
        mapReduceMeta = RenascenceBasic.GP_Streams_Malloc(mapReduceMetaNumber)
        for [i, name] in enumerate(mapReduceMetaFileList):
            stream = RenascenceBasic.GP_Stream_Create(name)
            RenascenceBasic.GP_Streams_Set(mapReduceMeta, stream, i)
        for [i, name] in enumerate(pieceLibMetaFileList):
            stream = RenascenceBasic.GP_Stream_Create(name)
            RenascenceBasic.GP_Streams_Set(libStreams, stream, i)
        self.nativeProducer = RenascenceBasic.GP_PiecesProducer_Create(basicProducer.get(), libStreams, None, libNumber, mapReduceMeta, mapReduceMetaNumber)
        for i in range(0, libNumber):
            stream = RenascenceBasic.GP_Streams_Get(libStreams, i)
            RenascenceBasic.GP_Stream_Destroy(stream)
        for i in range(0, mapReduceMetaNumber):
            stream = RenascenceBasic.GP_Streams_Get(mapReduceMeta, i)
            RenascenceBasic.GP_Stream_Destroy(stream)
        RenascenceBasic.GP_Streams_Free(libStreams)
        RenascenceBasic.GP_Streams_Free(mapReduceMeta)
    def __del__(self):
        RenascenceBasic.GP_PiecesProducer_Destroy(self.nativeProducer)
    def listType(self):
        result = []
        nativeResult = RenascenceBasic.GP_PiecesProducer_ListType(self.nativeProducer)
        number = RenascenceBasic.GP_Strings_Number(nativeResult)
        for i in range(0, number):
            result.append(RenascenceBasic.GP_Strings_Get(nativeResult, i))
        RenascenceBasic.GP_Strings_Free(nativeResult)
        return result
    def get(self):
        return self.nativeProducer

class PieceFunctionProducerParallel:
    def __init__(self, producer, parallelType):
        self.parallelType = parallelType
        self.producer = producer
    def __del__(self):
        #Nothing needed to do
        return
    def createFunction(self, formula, inputTypes):
        return PieceFunction(RenascenceBasic.GP_PiecesFunction_Create(self.producer.get(), formula, inputTypes, self.parallelType), self)
    def createInput(self, path, dataType, keyDimesions):
        n = len(keyDimesions)
        nativeKeyDimesions = RenascenceBasic.GP_Unsigned_Int_Array_Create(n)
        for i in range(0, n):
            RenascenceBasic.GP_Unsigned_Int_Array_Set(nativeKeyDimesions, keyDimesions[i], i)
        piece = Piece(RenascenceBasic.GP_Pieces_Create(self.producer.get(), self.parallelType, dataType, path, nativeKeyDimesions, n, RenascenceBasic.GP_PIECES_INPUT), self)
        RenascenceBasic.GP_Unsigned_Int_Array_Free(nativeKeyDimesions)
        return piece
    def createOutput(self, path):
        piece = Piece(RenascenceBasic.GP_Pieces_Create(self.producer.get(), self.parallelType, None, path, None, 0, RenascenceBasic.GP_PIECES_OUTPUT), self)
        return piece
    def copyPiece(self, read, write):
        RenascenceBasic.GP_Pieces_Copy(self.producer.get(), self.parallelType, read.get(), write.get())

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
