#include "GPPiecesFunctionCreator.h"
class GPBasicPiecesFunctionCreator:public GPPiecesFunctionCreator
{
public:
    virtual GPPiecesFunction* vCreateFromFuncTree(const GPFunctionTree* tree, const IParallelMachine* machine) const;
    GPBasicPiecesFunctionCreator();
    virtual ~GPBasicPiecesFunctionCreator();

};
