#include <iostream>
#include <vector>
#include<list>
#include <string>

using namespace std;

typedef pair<int,int> Point;
vector<vector<int>> dists;




int main()
{

}

class FindShortestWay{
    vector<vector<int>> Distance;
    vector<Point> Points;
    list<int> OffPathPoints;
    list<int> OnPathPoints;

    void findShortestPath(){
        int pathLength = Distance[OnPathPoints.front][OnPathPoints.back];
        for(auto it=OnPathPoints.begin(); ; ++ it){
            auto next_it = it;
            next_it ++;
            if(next_it == OnPathPoints.end()) next_it = OnPathPoints.begin();
            pathLength += Distance[*it][*next_it];
        }
        for(;OffPathPoints.size != 0;){
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



