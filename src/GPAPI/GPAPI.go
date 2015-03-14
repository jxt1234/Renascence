package GPAPI

/*
#cgo CFLAGS:-I/Users/jiangxiaotang/Documents/Genetic-Program-Frame/include
#cgo LDFLAGS:/Users/jiangxiaotang/Documents/Genetic-Program-Frame/libGP.so
#include <stdio.h>
#include <stdlib.h>
#include "user/adapter.h"
*/
import "C"

import (
	"unsafe"
)

func CreateProducer(filename string, gptype int) unsafe.Pointer {
	cstr := C.CString(filename)
	res := C.GO_GP_Producer_Create(cstr, C.int(gptype))
	C.free(unsafe.Pointer(cstr))
	return unsafe.Pointer(res)
}

func DestroyProducer(p unsafe.Pointer) {
	C.GO_GP_Producer_Destroy(p)
}

func CreateFuncByTypes(producer unsafe.Pointer, output string, input string, inputRepeat int) unsafe.Pointer {
	outputTypes := C.CString(output)
	defer C.free(unsafe.Pointer(outputTypes))
	inputTypes := C.CString(input)
	defer C.free(unsafe.Pointer(inputTypes))
	res := C.GO_GP_Function_Create_ByType(producer, outputTypes, inputTypes, C.int(inputRepeat))
	return unsafe.Pointer(res)
}

func CreateFuncByFile(producer unsafe.Pointer, filename string) unsafe.Pointer {
	filenamesc := C.CString(filename)
	defer C.free(unsafe.Pointer(filenamesc))
	return unsafe.Pointer(C.GO_GP_Function_Create_ByFile(producer, filenamesc))
}

func SaveFuncAsFile(function unsafe.Pointer, filename string) {
	cstring := C.CString(filename)
	defer C.free(unsafe.Pointer(cstring))
	C.GO_GP_Function_Save(function, cstring)
}

func RunFunc(functionp unsafe.Pointer, inputss unsafe.Pointer) unsafe.Pointer {
	res := C.GO_GP_Function_Run(functionp, inputss)
	return unsafe.Pointer(res)
}

func ProduceContent(producer unsafe.Pointer, types string, files string) unsafe.Pointer {
	ctypes := C.CString(types)
	cfiles := C.CString(files)
	defer C.free(unsafe.Pointer(ctypes))
	defer C.free(unsafe.Pointer(cfiles))
	return unsafe.Pointer(C.GO_GP_CreateContentsByTypes(producer, ctypes, cfiles))
}
