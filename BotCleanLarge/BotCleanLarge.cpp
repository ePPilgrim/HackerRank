#include <iostream>
#include <vector>
#include<list>
#include <string>

using namespace std;

typedef pair<int,int> Point;

class FindShortestWay{
    public:
    vector<vector<int>> Distance;
    vector<Point> Points;
    list<int> OffPathPoints;
    list<int> OnPathPoints;

    FindShortestWay(const vector<Point>& points){
        Points = points;
        for(int i = 0; i < Points.size(); ++ i) OffPathPoints.push_back(i);
        Distance = vector<vector<int>>();
        Distance.resize(Points.size());
        for(int i = 0; i < Distance.size(); ++ i){
            Distance[i] = vector<int>();
            Distance[i].resize(Points.size());
            Distance[i][i] = 0;
            for(int j = 0; j < i; ++ j){
                Distance[j][i] = Distance[i][j] = (Points[j].first - Points[i].first) * (Points[j].first - Points[i].first)
                                + (Points[j].second - Points[i].second) * (Points[j].second - Points[i].second);
            }
        }

    }

    void findShortestPath(){
        int pathLength = Distance[OnPathPoints.front()][OnPathPoints.back()];
        for(auto it=OnPathPoints.begin(); ; ++ it){
            auto next_it = it;
            next_it ++;
            if(next_it == OnPathPoints.end()) next_it = OnPathPoints.begin();
            pathLength += Distance[*it][*next_it];
        }
        for(;OffPathPoints.size() != 0;){
            int min_len = 1000000000;
            auto min_it = OffPathPoints.begin();
            auto min_jt = OnPathPoints.begin();
            for(auto it = OffPathPoints.begin(); it != OffPathPoints.end(); ++ it){
                for(auto jt = OnPathPoints.begin(); jt != OnPathPoints.end(); ++ jt){
                    auto next_jt = jt;
                    next_jt ++;
                    if(next_jt == OnPathPoints.end()) next_jt = OnPathPoints.begin();
                    auto len = pathLength - Distance[*jt][*next_jt] + Distance[*jt][*it] + Distance[*next_jt][*it];
                    if(len < min_len) min_len = len, min_jt = next_jt, min_it = it;
                }
            }
            pathLength = min_len;
            OnPathPoints.insert(min_jt, *min_it);
            OffPathPoints.erase(min_it);
        }
    }

    bool findConvexHull(){
        auto id0 = OnPathPoints.back();
        auto p0 = Points[id0];
        auto it1 = OffPathPoints.begin();
        for(auto it = OffPathPoints.begin(); it != OffPathPoints.end(); ++ it ){
            int x1 = (Points[*it1].first - p0.first);
            int y1 = (Points[*it1].second - p0.second);
            int x2 = (Points[*it].first - p0.first);
            int y2 = (Points[*it].second - p0.second);
            auto sign = x1 * y2 - y1 * x2;
            
            if(sign > 0 || 
                ((sign == 0) && (Distance[id0][*it1] > Distance[id0][*it]))) it1 = it;
        }
        if(*it1 == OnPathPoints.front()) return true;
        OnPathPoints.push_back(*it1);
        OffPathPoints.erase(it1);
        return false;
    }
};

string next_move(int posr, int posc, int dimh, int dimw, vector <string> board) {
    if(board[posr][posc] == 'd') {
        cout << "CLEAN" <<std::endl;
        return;
    }
    vector<Point> vec = vector<Point>();
    vec.reserve(2500);
    vec.push_back(Point(posc,posr));
    for(int i = 0; i < dimh; ++ i){
        for(int j = 0; j < dimw; ++ j){
            if(board[i][j] == 'd')  vec.push_back(Point(j,i));
        }
    }
    if(vec.size() == 1) return "EMPTY";
    FindShortestWay path = FindShortestWay(vec);
    auto min_it = path.OffPathPoints.begin();
    for(auto it = path.OffPathPoints.begin(); it != path.OffPathPoints.end(); ++ it){
        if(vec[*it].first < vec[*min_it]) min_it = it;
    }
    path.OffPathPoints.erase(min_it);
    path.OnPathPoints.push_back(*min_it);
    for(;path.findConvexHull() == false;);
    path.findShortestPath();
    for(auto it = path.OnPathPoints.begin(); it != path.OnPathPoints.end(); ++ it){
        if(*it == 0){
            auto next_it = it;
            next_it ++;
            if(next_it == path.OnPathPoints.end()) next_it = path.OnPathPoints.begin();
            int x = vec[*next_it].first - vec[*it].first;
            int y = vec[*next_it].second - vec[*it].second;
            string res = "EMPTY";
            if(x > 0) res = "RIGHT";
            if(x < 0) res = "LEFT";
            if(y > 0) res = "DOWN";
            if(y < 0) res = "UP";
            return res;
            // if(x > 0) cout << "RIGHT" << std::endl;
            // if(x < 0) cout << "LEFT" << std::endl;
            // if(y > 0) cout << "DOWN" << std::endl;
            // if(y < 0) cout << "UP" << std::endl;
            //return;
        }
    }
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
    } 
    return 0;
}



