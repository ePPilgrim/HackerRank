#include"Solution.h"

using namespace std;

class SteadyGene : public Solution{
    vector<vector<int>> mHistograms;
    int mLength;
    int mHight;

    void init(){
        stream >> mLength;
        mHight = mLength / 4;
        string gene;
        stream >> gene;
        map<char, int> dict;
        dict['A'] = 0;
        dict['C'] = 1;
        dict['G'] = 2;
        dict['T'] = 3;
        mHistograms = vector<vector<int>>(4,vector<int>(mLength + 1,0));
        for(int i = mLength - 1; i >= 0; i --){
            int k = dict[gene[i]];
            mHistograms[k][i] = 1;
            for(int j = 0; j < 4; ++ j){
                mHistograms[j][i] += mHistograms[j][i + 1];
            }
        }
    }

    int binary_search(int f, int t, int val, const vector<int>& vec){
        if(vec[f] <= val) return f;
        int m = (f + t) / 2;
        if(vec[m] > val) return binary_search(m + 1, t, val, vec);
        return binary_search(f, m, val, vec);
    }

    public:
    SteadyGene(string filepath){
        SetInputStream(filepath);
        init();
    }

    SteadyGene(){
        SetInputStream();
        init();
    }

    void FindSolution() {
        int minLen = mLength;
        for(int i = 0; i <= mLength; ++ i){
            vector<int> sub_histogram(4, 0);
            bool flag = false;
            for(int j = 0; j < 4; ++ j){
                sub_histogram[j] = mHistograms[j][0] - mHistograms[j][i];
                flag |= sub_histogram[j] > mHight; 
            }
            if(flag) break;
            int k = i;
            for(int j = 0; j < 4; ++ j){
                int m = binary_search(i ,mLength, mHight - sub_histogram[j], mHistograms[j]);
                if(m > k) k = m;
            }
            if( minLen > k - i) minLen = k - i;
        }
        cout << minLen << endl;
    }   
};

int main(){
    SteadyGene task("TestCases\\inputSteadyGene.txt");
    task.FindSolution();
    return 0;
}