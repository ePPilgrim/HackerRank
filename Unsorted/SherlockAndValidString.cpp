#include <vector>
#include <string>
#include <iostream>
#include <map>

std::string isValid(std::string s) {
    int nullpos = int('a');
    int cnt = int('z') - nullpos;
    std::vector<int> hist(cnt,0);
    for(int i = 0; i < s.length(); ++ i){
        hist[int(s[i] - nullpos)] ++;   
    }
    std::map<int,int> histHist;
    for(int i = 0; i < hist.size(); ++ i){
        histHist[hist[i]] ++;
    }
    histHist.erase(0);
    if(histHist.size() == 1) return "YES";
    if(histHist.size() > 2) return "NO";
    auto fi = histHist.cbegin();
    auto si = histHist.cend();
    si--;
    if(fi->first == 1 && fi->second == 1) return "YES";
    if(si->second == 1 && si->first - fi->first == 1) return "YES"; 

    return "NO";  
}

int main()
{
    std::string str;
    std::cin >> str;
    std::cout<<isValid(str)<<std::endl;

    return 0;
}

