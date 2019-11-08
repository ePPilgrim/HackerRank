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
    list<Point> OffPathPoints;
    list<Point> OnPathPoints;

    void findShortestPath(){}

    void findConvexHull(Point p0, Point endpoint){
        auto it1 = OffPathPoints.begin();
        for(auto it = OffPathPoints.begin(); it != OffPathPoints.end(); ++ it ){
            int x1 = (it1->first - p0.first);
            int y1 = (it1->second - p0.second);
            int x2 = (it->first - p0.first);
            int y2 = (it->second - p0.second);
            auto sign = x1 * y2 - y1 * x2;
            if(sign > 0 || 
                ((sign == 0) && ((x1 * x1 + y1 * y1) > (x2 * x2 + y2 * y2)))) it1 = it;
        }
        if(*it1 == endpoint) return;
        OnPathPoints.push_back(*it1);
        OffPathPoints.erase(it1);
    }
};



