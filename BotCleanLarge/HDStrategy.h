#include "NextPointStrategy.h"

class HDStrategy : public NextPointStrategy{
    public:
    list<int>& FindOptimalPath(const vector<Point>& points, const Point& bot){
        int index = -1;
        const int cnt = points.size();
        auto compX = [](const Point& p1, const Point& p2){return p1.x < p2.x;};
        int x_min = std::min_element(points.begin(), points.end(), compX)->x;
        int x_max = std::max_element(points.begin(), points.end(), compX)->x;
        auto compY = [](const Point& p1, const Point& p2){return p1.y < p2.y;};
        int y_min = std::min_element(points.begin(), points.end(), compY)->y;
        int y_max = std::max_element(points.begin(), points.end(), compY)->y;
        int min_dist = std::numeric_limits<int>::max();
        bool isOnBorder = x_min == bot.x || x_max == bot.x || y_min == bot.y || y_max == bot.y;
        for(int i = 0; i < cnt; ++ i){
            int ddist = dist(bot,points[i]);
            if( isOnBorder ){
                if(points[i].x == x_min || points[i].x == x_max) ddist = std::abs(points[i].y - bot.y);
                else ddist = std::abs(points[i].x - bot.x);   
            }
            if((ddist < min_dist) || (ddist == min_dist && std::rand() % 2)){
                min_dist = ddist;
                index = i;
            }
        }
        return std::list<int>(1,index);    
    }
};