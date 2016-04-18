#include "MGPCreator.h"
#include "MGPPiecesFactory.h"
#include "MGPUtils.h"
GPPieces* MGPCreator::vPrepare(GPPieces** inputs, int inputNumber) const
{
    MGPASSERT(NULL!=inputs);
    MGPASSERT(inputNumber>0);
    std::vector<unsigned int> keyDms;
    for (auto& p : mOutputKey)
    {
        MGPASSERT(p.first < inputNumber);//TODO
        GPPieces* inp = inputs[p.first];
        MGPASSERT(inp->nKeyNumber > p.second);//TODO
        keyDms.push_back(inp->pKeySize[p.second]);
    }
    return MGPPiecesFactory::create(keyDms);
}
