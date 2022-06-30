#include"./InputStream.h"

class Solution{
    protected:
    InputStream stream;
    void SetInputStream(std::string filePath){
        stream = InputStream(filePath);
    }
    void SetInputStream(){
        stream = InputStream("");
    }
    public:
    virtual void FindSolution() = 0;
};