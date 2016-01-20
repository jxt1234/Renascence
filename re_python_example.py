import Renascence

def main():
    Renascence.setStreamPath("./")
    Renascence.setLibPath("./")
    producer = Renascence.init(["func.xml"])
    print producer.listAllFunctions()
    print producer.listAllTypes()
    x0 = producer.load('TrBmp', "input.jpg")
    x1 = producer.load('TrBmp', "input_sharp.jpg")
    x2 = producer.load('TrBmp', "output.jpg")
    [Trainner, BestValue] = producer.train("S(ADF(Treator,x0,x1))", producer.merge(x0, x1), 500, cacheFile='temp.txt', postFormula='FIT(x0, x1)', postExtraInput=x2)
    formula = Trainner.ADF("")
    print 'Formula: ', formula
    print 'Parameters: ', Trainner.parameters()
    print BestValue
    predictor = producer.build(formula, Trainner.parameters())
    y_p = predictor.run(producer.merge(x0,x1))
    y_p.save("output/output_trained.jpg")

main()
