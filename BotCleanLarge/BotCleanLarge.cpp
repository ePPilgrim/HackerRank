#include <iostream>
#include <vector>
#include<list>
#include <string>
#include <set>
#include<algorithm>


using namespace std;

typedef pair<int,int> Point;
inline int dist(Point p1, Point p2){return (p2.first - p1.first) * (p2.first - p1.first) + (p2.second - p1.second) * (p2.second - p1.second);}

class FindShortestWay{
    public:
    set<Point> OffPathPoints;
    list<Point> OnPathPoints;
    vector<string> Path;

    void CreateBlankDrawing();

    void drawPoint(Point p, string str);

    void Draw(Point botpoint);

    FindShortestWay(const set<Point>& points){
        OffPathPoints = points;
        CreateBlankDrawing();
    }

    void findShortestPath(){
        int pathLength = 0;
        for(auto it=OnPathPoints.begin(), next_it = ++OnPathPoints.begin(); it != OnPathPoints.end(); ++ it, ++next_it){
            if(next_it == OnPathPoints.end()) next_it = OnPathPoints.begin();
            pathLength += dist(*it,*next_it);
        }
        for(;OffPathPoints.size() != 0;){
            int min_len = 1000000000;
            auto min_it = OffPathPoints.begin();
            auto min_jt = OnPathPoints.begin();
            for(auto it = OffPathPoints.begin(); it != OffPathPoints.end(); ++ it){
                for(auto jt=OnPathPoints.begin(), next_jt = ++OnPathPoints.begin(); jt != OnPathPoints.end(); ++ jt, ++next_jt){
                    if(next_jt == OnPathPoints.end()) next_jt = OnPathPoints.begin();
                    auto len = pathLength - dist(*jt,*next_jt) + dist(*jt,*it) + dist(*next_jt,*it);
                    if(len < min_len) min_len = len, min_jt = next_jt, min_it = it;
                }
            }
            pathLength = min_len;
            OnPathPoints.insert(min_jt, *min_it);
            OffPathPoints.erase(min_it);
        }
    }

    bool findConvexHull(){
        auto p0 = OnPathPoints.back();
        auto p1 = OnPathPoints.front();
        if(p0 == p1) p1 = *OffPathPoints.begin();
        for(auto it = OffPathPoints.begin(); it != OffPathPoints.end(); ++ it ){
            int sign = (p1.first - p0.first) * (it->second - p0.second) - (p1.second - p0.second)*(it->first - p0.first); 
            if(sign > 0 || 
                ((sign == 0) && (dist(p0,p1) > dist(p0,*it)))) p1 = *it;
        }
        if(p1 == OnPathPoints.front()) return true;
        OnPathPoints.push_back(p1);
        OffPathPoints.erase(p1);
        return false;
    }
};

string next_move(int posr, int posc, int dimh, int dimw, vector <string> board) {
    
    if(board[posr][posc] == 'd') {
        //cout << "CLEAN" <<std::endl;
        return "CLEAN";
    }
    auto pset = set<Point>();
    auto bot = Point(posc,posr);
    pset.insert(bot);
    for(int i = 0; i < dimh; ++ i){
        for(int j = 0; j < dimw; ++ j){
            if(board[i][j] == 'd')  pset.insert(Point(j,i));
        }
    }
    
    if(pset.size() == 1) return "EMPTY";
    FindShortestWay path = FindShortestWay(pset);
    auto min_it = std::min_element(path.OffPathPoints.begin(), path.OffPathPoints.end());
    path.OffPathPoints.erase(min_it);
    path.OnPathPoints.push_back(*min_it);
    for(;path.findConvexHull() == false;);
    path.findShortestPath();
    //path.Draw(bot);
    string res = "EMPTY";
    int min_dist = 1000000000;
    int min_id = -1;
    Point bot2 = bot;
    for(auto it = path.OnPathPoints.begin(); it != path.OnPathPoints.end(); ++ it){
        if(*it == bot){
            auto next_it = it;
            auto prev_it = it;
            next_it ++;
            if(next_it == path.OnPathPoints.end()) next_it = path.OnPathPoints.begin();
            if(prev_it == path.OnPathPoints.begin()) prev_it = -- path.OnPathPoints.end();
            else prev_it --;
            auto dist_next = dist(*it,*next_it);
            auto dist_prev = dist(*it,*prev_it);
            auto point = (dist_next > dist_prev) ? *prev_it : *next_it;
            int x = point.first - it->first;
            int y = point.second - it->second;
            if(x > 0) {res = "RIGHT"; bot2.first ++;break;}
            if(x < 0) {res = "LEFT"; bot2.first --;break;}
            if(y > 0) {res = "DOWN"; bot2.second ++;break;}
            if(y < 0) {res = "UP"; bot2.second --;break;}
            // if(x > 0) cout << "RIGHT" << std::endl;
            // if(x < 0) cout << "LEFT" << std::endl;
            // if(y > 0) cout << "DOWN" << std::endl;
            // if(y < 0) cout << "UP" << std::endl;
            //return;
        }
    }
    
    return res;
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

    /*pos[1] = 0;
    board[0] = "-d---";
    board[1] = "-d---";
    board[2] = "---d-";
    board[3] = "---d-";
    board[4] = "--d-d";*/
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
    for(auto status = next_move(pos[0], pos[1], dim[0], dim[1], board); status != "EMPTY";){
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
    return 0;
}


    void FindShortestWay::CreateBlankDrawing(){
        int maxX = 0;
        int maxY = 0;
        for(auto it = OffPathPoints.begin(); it != OffPathPoints.end(); ++ it){
            if(maxX < it->first) maxX = it->first;
            if(maxY < it->second) maxY = it->second;
        }
        maxX = 2 * (maxX + 2);
        maxY = 2 * (maxY + 2);
        Path.resize(maxY);
        for(unsigned int i = 0; i < maxY; ++ i){
            for(int j = 0; j < maxX; ++ j){
                Path[i] += '-';
            }
        }
    }

    void FindShortestWay::drawPoint(Point p, string str){
        p.first = 2 * (p.first + 1);
        p.second = 2 * (p.second + 1);
        Path[p.second][p.first] = str[0];
        Path[p.second][p.first + 1] = str[1];
    }

    void FindShortestWay::Draw(Point botpoint){
        std::cout<<"********************************************************"<<std::endl;
        for(unsigned int i = 0; i < Path.size(); ++ i){
            for(int j = 0; j < Path[0].size(); ++ j){
                Path[i][j] = '-';
            }
        }
        int order = 0;

        for(auto it = OnPathPoints.begin(); it != OnPathPoints.end(); ++ it){
            auto p = *it;
            auto num = to_string(order);
            if(num.length()==1) num = "0" + num;
            drawPoint(p,num);
            order ++;
        }
        //drawPoint(botpoint,"BB");
        for(unsigned int i = 0; i < Path.size(); ++ i){
            for(int j = 0; j < Path[0].size(); ++ j){
                std::cout << Path[i][j];
            }
            std::cout<<std::endl;
        }
        std::cout<<std::endl;
    }


