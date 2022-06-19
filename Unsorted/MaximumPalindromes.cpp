#include"./utils/Solution.h"
#include<string>

using namespace std;

class MaximumPalindromes : public Solution{
    const static ilong PRIME = 1000000007ULL;
    const static uint MAX_PALINDROM_LENGTH = 100000;
    const static uint MAX_LETTER_CNT = 26;

    vector<vector<uint>> lhistogram;
    vector<ilong> factorials;

    ilong devisionByModulo(ilong nom, ilong denom){
        ilong x_1  = 1LL;
        ilong x_2 = 0;
        ilong r1 = PRIME;
        ilong r2 = denom;
        for(;r2 != 1;){
            ilong temp = x_1;
            x_1 = (x_2 - temp * (r1 / r2)) % PRIME;
            x_2 = temp;
            temp = r2;
            r2 = r1 % r2;
            r1 = temp;
        }
        ilong invdenom = PRIME + x_1;
        return (nom * invdenom) % PRIME;
    }
public:
    MaximumPalindromes(){
        SetInputStream();
        
        factorials.resize(MAX_PALINDROM_LENGTH + 1, 1);
        for(int i = 1; i <= MAX_PALINDROM_LENGTH; ++ i){
            factorials[i] = (i * factorials[i - 1]) % PRIME;
        }
        string palindrome;
        stream >> palindrome;
        lhistogram.resize(MAX_PALINDROM_LENGTH + 1,vector<uint>(MAX_LETTER_CNT,0));
        for( int i = 1; i <= palindrome.length(); ++ i){
            std::copy(lhistogram[i - 1].begin(), lhistogram[i - 1].end(), lhistogram[i].begin());
            lhistogram[i][uint(palindrome[i - 1]) - uint('a')] ++;
        }
    }
    void FindSolution(){
        int q,l,r;
        stream >> q;
        for(int i = 0; i < q; ++ i){
            stream >> l >> r;
            ilong odd = 0;
            ilong denom = 1LL;
            uint maxLength = 0;
            for(int j = 0; j < MAX_LETTER_CNT; ++ j){
                uint pairs = lhistogram[r][j] - lhistogram[l - 1][j];
                maxLength += pairs / 2;
                denom = (denom * factorials[pairs / 2]) % PRIME;
                odd += pairs % 2;
            }
            ilong nom = factorials[maxLength];
            if(odd != 0) nom = (nom * odd) % PRIME;
            cout << devisionByModulo(nom, denom) << endl;  
        }
    }
};

int main(){
    MaximumPalindromes task;
    task.FindSolution();
    return 0;
}