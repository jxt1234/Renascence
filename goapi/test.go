package main

import (
	"GPAPI"
)

func main() {
	p := GPAPI.CreateProducer("func.xml", 0)
	GPAPI.DestroyProducer(p)
}
