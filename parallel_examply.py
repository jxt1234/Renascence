#!/usr/bin/python
import Renascence
def main():
    Renascence.setStreamPath("./")
    Renascence.setLibPath("./")
    producer = Renascence.init(["func.xml"])
    p_producer = Renascence.PieceFunctionProducer(producer, ['mgpfunc.xml'], ['Map-Reduce.xml'])
    print p_producer.listType()
    sub_p_producer = Renascence.PieceFunctionProducerParallel(p_producer, 'thread')
    pfunction = sub_p_producer.createFunction('C(S(x0))', 'TrBmp')
    inputPieces = sub_p_producer.createInput('res/pictures/', 'TrBmp', [5])
    outputPieces = sub_p_producer.createOutput('output/pythonTestParallel')
    tempPiece = pfunction.run([inputPieces])
    sub_p_producer.copyPiece(tempPiece, outputPieces)

    input1 = inputPieces.read('0')
    input1.save('output/pythonTestParallel/input.jpg')

    tempPiece.write(input1, '0')
    print input1
main()
