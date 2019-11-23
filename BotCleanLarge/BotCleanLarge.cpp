#include <iostream>
#include <vector>
#include<list>
#include <string>
#include <set>
#include<algorithm>
#include<random>

using namespace std;

struct Point{
    int x;
    int y;
    Point(int xx, int yy) : x(xx), y(yy){}
};
inline int dist(const Point& p1, const Point& p2){return (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y);}

  vector<string> createBlankDrawing(const list<Point>& points){
        int maxX = 0;
        int maxY = 0;
        for(auto it = points.begin(); it != points.end(); ++ it){
            if(maxX < it->x) maxX = it->x;
            if(maxY < it->y) maxY = it->y;
        }
        maxX = 2 * (maxX + 2);
        maxY = 2 * (maxY + 2);
        vector<string> res = vector<string>();
        res.resize(maxY);
        for(unsigned int i = 0; i < maxY; ++ i){
            for(int j = 0; j < maxX; ++ j){
                res[i] += '-';
            }
        }
        return res;
    }

    void drawPoint(Point p, string str, vector<string>& path){
        p.x = 2 * (p.x + 1);
        p.y = 2 * (p.y + 1);
        path[p.y][p.x] = str[0];
        path[p.y][p.x + 1] = str[1];
    }

    void Draw(Point botpoint, const list<Point>& OnPathPoints){
        std::cout<<"********************************************************"<<std::endl;
        auto Path = createBlankDrawing(OnPathPoints);
        int order = 0;

        for(auto it = OnPathPoints.begin(); it != OnPathPoints.end(); ++ it){
            auto p = *it;
            auto num = to_string(order);
            if(num.length()==1) num = "0" + num;
            drawPoint(p,num,Path);
            order ++;
        }
        drawPoint(botpoint,"BB",Path);
        for(unsigned int i = 0; i < Path.size(); ++ i){
            for(int j = 0; j < Path[0].size(); ++ j){
                std::cout << Path[i][j];
            }
            std::cout<<std::endl;
        }
        std::cout<<std::endl;
    }

struct DP{
    int prev;
    int dist;
    DP(int _prev, int _dist) : prev(_prev), dist(_dist){}
};

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
    int cnt = points.size();
    auto p = points[0];
    if(cnt > 1){
        std::cout<< cnt << std::endl;
        auto dp = vector<vector<DP>>();
        for(int i = 0; i < cnt; ++ i){
            dp.push_back(vector<DP>(cnt,DP(-1,std::numeric_limits<int>::max())));
        }

        for(int i = 0; i < cnt; ++ i) dp[0][i].dist = dist(bot,points[i]);
        auto flags = vector<int>(cnt,-1);
        for(int i = 1; i < cnt; ++ i){
            std::fill(flags.begin(),flags.end(),-1);
            for(int j = 0; j < cnt; ++ j){
                if(i == 1 || dp[i - 1][j].prev >= 0){
                    int m = j;
                    for(int k = i - 1;k >= 0; -- k) {flags[m] = j; m = dp[k][m].prev;}
                    for(int k = 0; k < cnt; ++ k){
                        if(flags[k] != j){
                            int dd = dp[i-1][j].dist + dist(points[j],points[k]);
                            if(dp[i][k].dist > dd){
                                dp[i][k].dist = dd;
                                dp[i][k].prev = j;
                            } 
                            if(dd == dp[i][k].dist){
                                if(std::rand() % 2){
                                    dp[i][k].dist = dd;
                                    dp[i][k].prev = j; 
                                }
                            }
                        }
                    }
                }
            }   
        }
        int min_dist = std::numeric_limits<int>::max();
        int k_min = -1;
        for(int i = 0; i < cnt; ++ i){
            if(min_dist > dp[cnt-1][i].dist){
                k_min = i;
                min_dist = dp[cnt-1][i].dist;
            }
        }

        list<Point> path = list<Point>();
        path.push_front(points[k_min]);
        for(int i = cnt - 1; i > 0; -- i){
            k_min = dp[i][k_min].prev; 
            path.push_front(points[k_min]);
        }
        p = points[k_min];
        //Draw(bot, path);
    }
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

    dim[0] = 17;
    dim[1] = 46;
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

    /*pos[1] = 0;
    board.resize(5);
    board[0] = "-d---";
    board[1] = "-d---";
    board[2] = "---d-";
    board[3] = "---d-";
    board[4] = "--d-d";*/
    
   int ff = 0;
    for(auto status = next_move(pos[0], pos[1], dim[0], dim[1], board); status != "EMPTY";){
        ff ++;
        if(status == "CLEAN"){
            board[pos[0]][pos[1]] = '-';
        }
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
        
        std::cout<<status<<std::endl;
        status = next_move(pos[0], pos[1], dim[0], dim[1], board);
    } 
    std::cout << "Total number of points = "<< dim[0] * dim[1] << std::endl;
    std::cout << "Number of moves = " << ff << std::endl;
    return 0;
}





