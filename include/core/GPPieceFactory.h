/******************************************************************
   Copyright 2016, Jiang Xiao-tang

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
******************************************************************/
#ifndef CORE_GPPIECEFACTORY_H
#define CORE_GPPIECEFACTORY_H
#include "lowlevelAPI/GPPieces.h"
class GPPieceFactory
{
public:
    static GPPieces* createMemoryPiece(const std::vector<unsigned int> &keydimesions);
    static GPPieces* createLocalFilePiece(const std::vector<unsigned int> &keydimesions, const IStatusType* type, const char* srcPath, size_t maxMemoryCacheSize/*MB*/);
    static std::string getFilePath(unsigned int* pKey, unsigned int keynum, const std::string& types, const std::string& basicPath);
};
#endif
