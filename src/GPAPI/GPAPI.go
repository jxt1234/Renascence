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

func createProducer(filename string, gptype int) unsafe.Pointer {
	cstr, err := C.CString(filename)
	res, err := C.GO_GP_Producer_Create(cstr, C.int(gptype))
	C.free(unsafe.Pointer(cstr))
	return unsafe.Pointer(res)
}

func destroyProducer(p unsafe.Pointer) {
	C.GO_GP_Producer_Destroy(p)
}

func createFuncByTypes(producer unsafe.Pointer, output string, input string, inputRepeat bool) unsafe.Pointer {
	outputTypes := C.CString(output)
	defer C.free(unsafe.Pointer(outputTypes))
	inputTypes := C.CString(input)
	defer C.free(unsafe.Pointer(inputTypes))
	res := C.GO_GP_Function_Create_ByType(producer, outputTypes, inputTypes, C.int(inputRepeat))
	return res
}

func createFuncByFile(producer unsafe.Pointer, filename string) unsafe.Pointer {
	filenamesc := C.CString(filename)
	defer C.free(unsafe.Pointer(filenamesc))
	return C.GO_GP_Function_Create_ByFile(producer, filenamesc)
}

func saveFuncAsFile(function unsafe.Pointer, filename string) {
	cstring := C.CString(filename)
	defer C.free(unsafe.Pointer(cstring))
	C.GO_GP_Function_Save(function, cstring)
}

/*
func runFunc(function unsafe.Pointer, inputs unsafe.Pointer) unsafe.Pointer {
	return C.GO_GP_Function_Run(function, inputs)
}
*/

func produceContent(producer unsafe.Pointer, types string, files string) unsafe.Pointer {
	ctypes := C.CString(types)
	cfiles := C.CString(files)
	defer C.free(unsafe.Pointer(ctypes))
	defer C.free(unsafe.Pointer(cfiles))
	return C.GO_GP_CreateContentsByTypes(producer, ctypes, cfiles)
}
