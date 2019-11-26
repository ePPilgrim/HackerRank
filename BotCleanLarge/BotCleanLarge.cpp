#include "DPStrategy.h"
#include "HDStrategy.h"
#include "ShortestDistanceStrategy.h"
#include "RegionStrategy.h"
#include "ConvexStrategy.h"
#include <set>
#include<algorithm>
#include<random>
#include<ctime>

using namespace std;

int cccc = 0;

string next_move1(int posr, int posc, int dimh, int dimw, vector <string> board) {
    
    if(board[posr][posc] == 'd') return "CLEAN";
    Point bot = Point(posc, posr);
    float globalThreshold =1.0f;// 0.0000500001f;
    float localThreshold = 1.0f;//0.5f;
    NextPointStrategy* sparseStrategy = new DPStrategy();
    //NextPointStrategy* sparseStrategy = new ConvexStrategy();
    NextPointStrategy* densStrategy = new HDStrategy();
    //NextPointStrategy* densStrategy = new ShortestDistanceStrategy();
   // auto ttt = time(NULL);
    //auto t1 = std::clock();
    
    //std::cout<< "Time = "<<ttt<<std::endl;
    //srand (cccc);
    //cccc += 13;
    auto partition = RegionStrategy(localThreshold, globalThreshold, 
                                    sparseStrategy, densStrategy,
                                    board, bot);
    Point p = partition.FindNextPoint(5,5);
    if(p.x < 0) return "EMPTY";
    
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
    int TotalNumberOfDPoint = 7;
    board.resize(5);
    board[0] = "----d";
    board[1] = "-d--d";
    board[2] = "--dd-";
    board[3] = "--d--";
    board[4] = "----d";

    /*dim[0] = 34;
    dim[1] = 46;
    pos[0] = 8;
    pos[1] = 22;
    board.resize(34);
    board[0] = "-d--d---d--d----d--d--d-d--d---d--d----d--d--d";
    board[1] = "-d--d--d---d--d--d--d---d--d---d--d----d--d--d";
    board[2] = "--dd-ddddddd-dddddddddd--d-ddd------ddddd-d-d-";
    board[3] = "--d-ddd------ddddd-d-d-ddddd-d-d--d--dddd--dd-";
    board[4] = "ddddd-d-d--d--dddd--dd---dd-ddddd---ddd--ddddd";
    board[5] = "--dd-dddddddd-ddddddddd--d-ddd------ddddd-d-d-";
    board[6] = "--d-ddd------ddddd-d-d-ddddd-d-d--d--dddd--dd-";
    board[7] = "-d--d--d---d--d--d--d---d--d---d--d----d--d--d";
    board[8] = "--dd-dddddd-ddddddd-ddd--d-ddd------ddddd-d-d-";
    board[9] = "--d-ddd------ddddd-d-d-ddddd-d-d--d--dddd--dd-";
    board[10] = "ddddd-d-d--d--dddd--dd---dd-dddd-dd-dd-ddddddd";
    board[11] = "-d--d---d--d----d--d--d-d--d---d--d----d--d--d";
    board[12] = "ddddd-d-d--d--dddd--dd---dd-ddddddddddd-dddddd";
    board[13] = "--dd-ddddd---dddddddddd--d-ddd------ddddd-d-d-";
    board[14] = "--d-ddd------ddddd-d-d-ddddd-d-d--d--dddd--dd-";
    board[15] = "-d--d--d---d--d--d--d---d--d---d--d----d--d--d";
    board[16] = "--dd-dddddddddddddddddd--d-ddd------ddddd-d-d-";
    board[17] = "-d--d---d--d----d--d--d-d--d---d--d----d--d--d";
    board[18] = "-d--d--d---d--d--d--d---d--d---d--d----d--d--d";
    board[19] = "--dd-dddddddd-ddddddddd--d-ddd------ddddd-d-d-";
    board[20] = "--d-ddd------ddddd-d-d-ddddd-d-d--d--dddd--dd-";
    board[21] = "ddddd-d-d--d--dddd--dd---dd-dd----------dddddd";
    board[22] = "--dd-ddddd--ddddddddddd--d-ddd------ddddd-d-d-";
    board[23] = "--d-ddd------ddddd-d-d-ddddd-d-d--d--dddd--dd-";
    board[24] = "-d--d--d---d--d--d--d---d--d---d--d----d--d--d";
    board[25] = "--dd-dddddddddd----dddd--d-ddd------ddddd-d-d-";
    board[26] = "--d-ddd------ddddd-d-d-ddddd-d-d--d--dddd--dd-";
    board[27] = "ddddd-d-d--d--dddd--dd---dd-dddddddddddddddddd";
    board[28] = "-d--d---d--d----d--d--d-d--d---d--d----d--d--d";
    board[29] = "ddddd-d-d--d--dddd--dd---dd-ddddd-----dddddddd";
    board[30] = "--dd-ddddddd---dddddddd--d-ddd------ddddd-d-d-";
    board[31] = "--d-ddd------ddddd-d-d-ddddd-d-d--d--dddd--dd-";
    board[32] = "-d--d--d---d--d--d--d---d--d---d--d----d--d--d";
    board[33] = "--dd-dddddddd--dddddddd--d-ddd------ddddd-d-d-";


    dim[0] = 50;
    dim[1] = 50;
    pos[0] = 33;
    pos[1] = 45;
    board = vector<string>(50);
    TotalNumberOfDPoint = 0;
    for(int i = 0; i < 50; i ++){
        for(int j = 0; j < 50; ++ j){
            char ch = '-';//(rand()%2) ? '-' : 'd';
            board[i].push_back(ch);
        }
    }

    TotalNumberOfDPoint = 0;
    int nn = 20;
    for(int i = nn; i >= 0; -- i){
        int y = (i + rand()) % 47;
        int x = (y * y * y + rand()) % 47;
        if(board[y][x] != 'd'){
            board[y][x] = 'd';
            TotalNumberOfDPoint ++;
        }
        
    }
    for(int i = 0; i < 50; ++ i){
        std::cout<<board[i] << std::endl;
    }


    /*dim[0] = 5;
    dim[1] = 5;
    pos[0] = 0;
    pos[1] = 0;
    board.resize(5);
    board[0] = "-d---";
    board[1] = "-d---";
    board[2] = "---d-";
    board[3] = "---d-";
    board[4] = "--d-d";*/
    
   int ff = 0;
    for(auto status = next_move1(pos[0], pos[1], dim[0], dim[1], board); status != "EMPTY";){
       
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
        status = next_move1(pos[0], pos[1], dim[0], dim[1], board);
    } 
    std::cout << "Total number of points = "<< TotalNumberOfDPoint << std::endl;
    std::cout << "Number of moves = " << ff << std::endl;
    std::cout << "Portion of moves = " << (float)ff/(float)TotalNumberOfDPoint<<std::endl;
    return 0;
}





