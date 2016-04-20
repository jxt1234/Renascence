/******************************************************************
   Copyright 2014, Jiang Xiao-tang

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
#ifndef UTILS_AUTOSTORAGE_H
#define UTILS_AUTOSTORAGE_H
template <typename T>
class AutoStorage
{
    public:
        AutoStorage(int size)
        {
            mT = new T[size];
        }
        ~AutoStorage()
        {
            delete [] mT;
        }
        T* get() {return mT;}
    private:
        T* mT;
};
#define AUTOSTORAGE(x, type, size)\
    AutoStorage<type> __##x(size);\
    type * x = __##x.get();
#endif
