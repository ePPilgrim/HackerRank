#include"Solution.h"
#include<string>

using namespace std;

class SherlockAndAnagrams : public Solution{
    static const uint POWER_OF_ALPHABET = 26;
    struct Node{
        vector<int> Histogram;
        int SetId;
        int SetPower;
        Node() : Histogram(vector<int>(POWER_OF_ALPHABET,0)), SetId(-1), SetPower(0) {}
    };
    vector<Node> Graph;
 public:  
    SherlockAndAnagrams(string path){
        SetInputStream(path);
    }
    SherlockAndAnagrams(){
        SetInputStream();
    }
    void FindSolution(){
        int Q;
        stream >> Q;
        for(int q = 0; q < Q; ++ q){
            string str;
            stream >> str;
            int n = str.length();
            Graph.clear();
            Graph.resize(n);
            int res = 0;
            for(int len = 1; len < n; ++ len)
            {
                int m = n - len;
                for(int j = 0; j <= m; ++j) Graph[j].Histogram[int(str[j + len - 1]) - int('a')] ++;
                for(int j = 0; j <= m; ++ j){
                    if(Graph[j].SetId != -1) continue;
                    Graph[j].SetPower = 1;
                    for(int k = j + 1; k <= m; ++ k){
                        if((Graph[k].SetId == -1) && (Graph[k].Histogram == Graph[j].Histogram)){
                            Graph[k].SetId = 0;
                            Graph[j].SetPower ++;
                        }
                    }
                }

                for_each(Graph.begin(), Graph.end(), [&res](Node& node){
                        res += node.SetPower * (node.SetPower - 1) / 2;
                        node.SetId = -1;
                        node.SetPower = 0; 
                    });
            }
            cout << res << endl;
        }
    }
};

int main(){
    SherlockAndAnagrams task("TestCases\\inputSherlockAndAnagrams.txt");
    task.FindSolution();
    return 0;
}