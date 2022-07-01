#include"Solution.h"
#include<string>

using namespace std;

class CommonChild : public Solution{

 public:  
    CommonChild(string path){
        SetInputStream(path);
    }
    CommonChild(){
        SetInputStream();
    }
    void FindSolution(){
        string s1,s2;
        stream >> s1 >> s2;
        int n = s1.length();
        vector<vector<int>> dp(n, vector<int>(n, 0));
        vector<int> distr(n, -1);
        for(int i = 0; i < n; ++ i) dp[0][i] = (s2[i] == s1[0]) ? 1 : 0;
        
        for(int i = 1; i < n; ++ i){
            char ch = s1[i];
            int found = -1;
            for(int j = n - 1; j >= 0; --j){
                if(s2[j] == ch) {
                    found = j;
                    dp[i][j] = 1;
                }
                distr[j] = found;
            }
            for(int j = 0; j < n; ++ j){
                dp[i][j] = max(dp[i-1][j], dp[i][j]);
                if( j < n - 1 && distr[j + 1] != -1){
                    dp[i][distr[j + 1]] = max(dp[i-1][j] + 1, dp[i][distr[j + 1]]);
                }
            }
        }
        int res = *max_element(dp[n - 1].begin(), dp[n - 1].end());
        cout << res << endl;
    }
};

int main(){
    CommonChild task("TestCases\\inputCommonChild2.txt");
    for(int i = 0; i < 1; ++ i) task.FindSolution();
    return 0;
}