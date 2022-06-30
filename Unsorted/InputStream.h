
#include"./Types.h"

class InputStream{
    std::istream* ptrToStream;
    bool tryToSetStreamFromFile(std::string filePath = "") {
        if(filePath.empty()){
            return false;
        }
        std::ifstream* ptr_fstream = new std::ifstream();
        ptr_fstream->open(filePath);
        if(ptr_fstream->fail()){
            return false;
        }
        ptrToStream = ptr_fstream;
        return true;  
    }

    public:

    InputStream(std::string filePath = ""){
        if(tryToSetStreamFromFile(filePath)){
            std::cout << "Data is loaded from file: " << filePath << std::endl;   
        } 
        else{
            ptrToStream = &std::cin;
        }
    }

    std::istream& Get() const{
        return *ptrToStream;
    }

    InputStream& operator = (const InputStream& otherInputStream){
        if(ptrToStream == &otherInputStream.Get()){
            return *this;
        }
        ptrToStream = &otherInputStream.Get();
        return *this;
    }
};

template<class T>
inline InputStream& operator >> (InputStream& stream, T& val){
    stream.Get() >> val;
    return stream;
}


    
