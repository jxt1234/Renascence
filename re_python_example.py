import Renascence

def main():
    Renascence.setStreamPath("./")
    Renascence.setLibPath("./")
    producer = Renascence.init(["func.xml"])
    print producer.listAllFunctions()
    print producer.listAllTypes()
    TrBmp = producer.type("TrBmp")
    x0 = TrBmp.load("input.jpg")
    x1 = TrBmp.load("input_sharp.jpg")
    x2 = TrBmp.load("output.jpg")
    x0x1x2 = Renascence.merge(x0, x1, x2)
    [Trainner, BestValue] = producer.train("FIT(ADF(Treator,x0,x1), x2)", x0x1x2, 0)
    formula = Trainner.ADF("Treator")
    print 'Formula: ', formula
    print 'Parameters: ', Trainner.parameters()
    print BestValue
    predictor = producer.build(formula)
    x0x1 = Renascence.merge(x0, x1)
    y_p = predictor.run(x0x1)
    TrBmp.save(y_p, "output/output_trained.jpg")

    Renascence.freeContents(x0, x1, x2, x0x1x2, x0x1)
main()
