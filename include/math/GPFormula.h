#include <vector>
#include <string>

class GPFormula
{
    public:
        GPFormula(const std::string& formula);
        ~GPFormula();
        static void analysis(std::vector<int>& func, std::vector<int>& number, const std::string& formula);
        inline const std::vector<int>& get() const {return mQueue;}
    private:
        std::vector<int> mQueue;
};
