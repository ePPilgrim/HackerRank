#include "NextPointStrategy.h"

class ShortestDistanceStrategy : public NextPointStrategy{
    public:
    list<int>& FindOptimalPath(const vector<Point>& points, const Point& bot){
        int cnt = points.size();
        if(cnt == 0) return *new std::list<int>();
        int index = -1;
        int min_dist = std::numeric_limits<int>::max();
        for(int i = 0; i < cnt; ++ i){
            int dd = dist(bot,points[i]);
            if((dd < min_dist) || (dd == min_dist && std::rand() % 2)){
                min_dist = dd;
                index = i;
            }
        }
        return *new std::list<int>(1,index);    
    }
};