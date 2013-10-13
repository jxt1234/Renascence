#ifndef XML_READER_H
#define XML_READER_H
#include <string>
#include <vector>

#define XMLAPI


using namespace std;
class xmlReader
{
    public:
        XMLAPI void loadFile(const char* file);
        XMLAPI void writeFile(const char* file);
        xmlReader(const char* file = NULL);
        virtual ~xmlReader();
    protected:
        virtual void attributeUnflatten() = 0;
        void clear();
        //For child class to clear their self data
        virtual void subClear(){}
        struct package
        {
            string name;
            vector<string> attr;
            vector<package*> children;
        };
        void analysisLine(const string& line);
        package* loadPackage(ifstream& file);
        package* mAttributes;
        string mFile;
    private:
        vector<package*> mPackage;
        string mString;
        package* mCurPackage;
};

class xmlReaderTest:public xmlReader
{
    public:
        XMLAPI void print();
        xmlReaderTest(const char* file = NULL):xmlReader(file){}
        virtual ~xmlReaderTest(){}
    protected:
        void printUnit(package* p);
        virtual void attributeUnflatten(){print();}
};

#endif
