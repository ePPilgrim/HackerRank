#include "NextPointStrategy.h"

class HDStrategy : public NextPointStrategy{
    public:
    list<int>& FindOptimalPath(const vector<Point>& points, const Point& bot){
        int index,bx,by = -1;
        const int cnt = points.size();
        bool isOnBorder = true;
        auto compX = [](const Point& p1, const Point& p2){return p1.x < p2.x;};
        int x_min = std::min_element(points.begin(), points.end(), compX)->x;
        if(bot.x == x_min) bx = x_min;
        else{
            int x_max = std::max_element(points.begin(), points.end(), compX)->x;
            if(bot.x == x_max) bx = x_max;
            else{
                auto compY = [](const Point& p1, const Point& p2){return p1.y < p2.y;};
                int y_min = std::min_element(points.begin(), points.end(), compY)->y;
                if(bot.y == y_min) by = y_min;
                else{
                    int y_max = std::max_element(points.begin(), points.end(), compY)->y;
                    if(bot.y == y_max) by = y_max;
                    else isOnBorder = false;
                }
            }
        }
        int min_dist = std::numeric_limits<int>::max();
        for(int i = 0; i < cnt; ++ i){
            int ddist = dist(bot,points[i]);
            if( isOnBorder ){
                if(points[i].x == bot.x) ddist = std::abs(points[i].y - bot.y);
                else ddist = std::abs(points[i].x - bot.x);   
            }
            if((ddist < min_dist) || (ddist == min_dist && std::rand() % 2)){
                min_dist = ddist;
                index = i;
            }
        }
        return *new std::list<int>(1,index);    
    }
};