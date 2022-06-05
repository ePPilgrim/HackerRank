#include<string>
#include<vector>
#include<iostream>


std::string highestValuePalindrome(std::string s, int n, int k) {
    // int from = 0, len = s.length();
    // for(; from < len/2 && s[from] != '0'; ++ from ){
    //     if(s[len-from-1] != '0'){
    //         s[len-from-1] = '0';
    //         k --;
    //     }
    // }
    // if(k < 0 ) return "-1";
    int oddCnt = 0;
    for(int i = 0; i < n/2; ++ i){
        if(s[n-i-1] != s[i]) oddCnt ++;
    }
    if(k < oddCnt) return "-1";
    for(int i = 0; i < n/2; ++i){
        int j = n - i -1;
        if(s[i] == s[j]) {
            if(s[i] == '9' || oddCnt + 2 > k) continue;
            s[i] = s[j] = '9';
            k -= 2;
        }
        else{
            char maxNumber = int(s[i]) > int(s[j]) ? s[i] : s[j];
            if(maxNumber != '9' &&  oddCnt + 1 <= k){
                maxNumber = '9';
                k --;
            }
            s[i] = s[j] = maxNumber;
            k --;
            oddCnt --;
        }
    }
    if(k > 0 && n % 2 != 0) s[n/2] = '9';
    return s;
}

int main()
{
    int n, k;
    std::string str;
    std::cin >> n >> k >> str;
    std::cout<<highestValuePalindrome(str, n, k)<<std::endl;
    return 0;
}