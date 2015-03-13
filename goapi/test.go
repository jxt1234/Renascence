package main

import (
	"GPAPI"
)

func main() {
	p := GPAPI.createProducer("func.xml", 0)
	GPAPI.destroyProducer(p)
}
