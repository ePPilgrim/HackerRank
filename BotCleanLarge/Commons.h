#include <iostream>
#include <vector>
#include<list>
#include <string>
#include <algorithm>

using namespace std;

struct Point{
    int x;
    int y;
    Point(int xx, int yy) : x(xx), y(yy){}
};

struct DP{
    int prev;
    int dist;
    DP(int _prev, int _dist) : prev(_prev), dist(_dist){}
};

inline int dist(const Point& p1, const Point& p2){return (p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y);}

inline vector<string> createBlankDrawing(const vector<Point>& points){
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

inline void drawPoint(Point p, string str, vector<string>& path){
    p.x = 2 * (p.x + 1);
    p.y = 2 * (p.y + 1);
    path[p.y][p.x] = str[0];
    path[p.y][p.x + 1] = str[1];
}

inline void Draw(Point botpoint, const vector<Point>& OnPathPoints){
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
    cout<<std::endl;
}
