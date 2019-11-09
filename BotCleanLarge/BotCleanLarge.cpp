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

    void findShortestPath(){}

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



