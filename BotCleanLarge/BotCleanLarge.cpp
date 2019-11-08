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

    void findConvexHull(Point p0, Point endpoint){

        auto it1 = OffPathPoints.begin();
        for(auto it = OffPathPoints.begin(); it != OffPathPoints.end(); ++ it ){
            auto sign = (it1->first - p0.first) * (it->second - p0.second) - 
                        (it1->second - p0.second) * (it->first - p0.first);
            if(sign > 0) it1 = it;
        }
        if(*it1 == endpoint) return;
        OnPathPoints.push_back(*it1);
        OffPathPoints.erase(it1);
    }
};



