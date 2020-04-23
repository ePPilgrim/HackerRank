#include<iostream>
#include<vector>
using namespace std;

void next_move(int posr, int posc, vector <string> board) {
    if(board[posr][posc] == 'o') {
        std::cout<<"CLEAN"<<std::endl;
        return;
    }
    if(posc == 0 && posr != 0){
        std::cout<<"UP"<<std::endl;
        return;
    }
    if(posc == 0 && posr == 0){
        std::cout<<"RIGHT"<<std::endl;
        return;
    }
    if(posr % 2 == 0){
        if(posc == 1)
    }
}

int main(void) {
    int pos[2];
    vector <string> board;
    cin>>pos[0]>>pos[1];
    for(int i=0;i<5;i++) {
        string s;cin >> s;
        board.push_back(s);
    }
    next_move(pos[0], pos[1], board);
    return 0;
}