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
    x0x1x2 = producer.merge(x0, x1, x2)
    [Trainner, BestValue] = producer.train("FIT(ADF(Treator,x0,x1), x2)", x0x1x2, 500)
    formula = Trainner.ADF("Treator")
    print 'Formula: ', formula
    print 'Parameters: ', Trainner.parameters()
    print BestValue
    predictor = producer.build(formula, Trainner.parameters())
    x0x1 = producer.merge(x0, x1)
    y_p = predictor.run(x0x1)
    u = producer.content(9.0)
    y_p.save("output/output_trained.jpg")

main()
