#include "DPStrategy.h"
#include <set>
#include<algorithm>
#include<random>

using namespace std;

string next_move(int posr, int posc, int dimh, int dimw, vector <string> board) {
    
    if(board[posr][posc] == 'd') return "CLEAN";
    Point bot = Point(posc, posr);
    vector<Point> points = vector<Point>();
    points.reserve(2500);
    for(int i = 0; i < dimh; ++ i){
        for(int j = 0; j < dimw; ++ j){
            if(board[i][j] == 'd'){
                points.push_back(Point(j,i));    
            }
        }
    }
    auto pStrategy = DPStrategy();
    auto llist = pStrategy.FindOptimalPath(points, bot);

    auto path = vector<Point>();
    for(auto it = llist.begin(); it != llist.end(); ++ it){
        path.push_back(points[*it]);
    }

    //Draw(bot, path);
    auto p = path[0];
    
    if(bot.x == p.x){
        if(bot.y < p.y) return "DOWN";
        if(bot.y > p.y) return "UP";
    }
    if(bot.y == p.y){
        if(bot.x < p.x) return "RIGHT";
        if(bot.x > p.x) return "LEFT";
    }
    if(std::rand() % 2){
        if(bot.x < p.x) return "RIGHT";
        if(bot.x > p.x) return "LEFT";
    } else{
        if(bot.y < p.y) return "DOWN";
        if(bot.y > p.y) return "UP";
    }
    return "EMPTY";
}



int main()
{
    int pos[2] = {0,1};
    int dim[2] = {5,5};
    vector <string> board;
    /*cin>>pos[0]>>pos[1];
    cin>>dim[0]>>dim[1];
    for(int i=0;i<dim[0];i++) {
        string s;cin >> s;
        board.push_back(s);
    }*/
    board.resize(5);
    board[0] = "----d";
    board[1] = "-d--d";
    board[2] = "--dd-";
    board[3] = "--d--";
    board[4] = "----d";

    //dim[0] = 17;
    //dim[1] = 46;
    //pos[0] = 8;
    //pos[1] = 22;
    board.resize(17);
    board[0] = "-d--d---d--d----d--d--d-d--d---d--d----d--d--d";
    board[1] = "-d--d--d---d--d--d--d---d--d---d--d----d--d--d";
    board[2] = "--dd-dddddddddddddddddd--d-ddd------ddddd-d-d-";
    board[3] = "--d-ddd------ddddd-d-d-ddddd-d-d--d--dddd--dd-";
    board[4] = "ddddd-d-d--d--dddd--dd---dd-dddddddddddddddddd";
    board[5] = "--dd-dddddddddddddddddd--d-ddd------ddddd-d-d-";
    board[6] = "--d-ddd------ddddd-d-d-ddddd-d-d--d--dddd--dd-";
    board[7] = "-d--d--d---d--d--d--d---d--d---d--d----d--d--d";
    board[8] = "--dd-dddddddddddddddddd--d-ddd------ddddd-d-d-";
    board[9] = "--d-ddd------ddddd-d-d-ddddd-d-d--d--dddd--dd-";
    board[10] = "ddddd-d-d--d--dddd--dd---dd-dddddddddddddddddd";
    board[11] = "-d--d---d--d----d--d--d-d--d---d--d----d--d--d";
    board[12] = "ddddd-d-d--d--dddd--dd---dd-dddddddddddddddddd";
    board[13] = "--dd-dddddddddddddddddd--d-ddd------ddddd-d-d-";
    board[14] = "--d-ddd------ddddd-d-d-ddddd-d-d--d--dddd--dd-";
    board[15] = "-d--d--d---d--d--d--d---d--d---d--d----d--d--d";
    board[16] = "--dd-dddddddddddddddddd--d-ddd------ddddd-d-d-";

    pos[1] = 0;
    board.resize(5);
    board[0] = "-d---";
    board[1] = "-d---";
    board[2] = "---d-";
    board[3] = "---d-";
    board[4] = "--d-d";
    
   int ff = 0;
    for(auto status = next_move(pos[0], pos[1], dim[0], dim[1], board); status != "EMPTY";){
       
        if(status == "CLEAN"){
            board[pos[0]][pos[1]] = '-';
        } else
        {
            ff ++;
            if(status == "RIGHT"){
                pos[1] ++;
            }
            if(status == "LEFT"){
                pos[1] --;
            }
            if(status == "DOWN"){
                pos[0] ++;
            }
            if(status == "UP"){
                pos[0] --;
            }
        }
        
        std::cout<<status<<std::endl;
        status = next_move(pos[0], pos[1], dim[0], dim[1], board);
    } 
    std::cout << "Total number of points = "<< dim[0] * dim[1] << std::endl;
    std::cout << "Number of moves = " << ff << std::endl;
    return 0;
}





