#include "NextPointStrategy.h"

class ConvexStrategy : public NextPointStrategy{
    public:
    list<int>& FindOptimalPath(const vector<Point>& points, const Point& bot){ 
        Points = points;
        Bot = Points.size();
        Points.push_back(bot);
        int cnt = Points.size();
        if(cnt == 1) return *new list<int>();
        if(cnt == 2) return *new list<int>(1,0);
        int min_ix = 0;
        for(int i = 1; i < cnt; ++ i) {
            if(Points[i].x < Points[min_ix].x){
                OffPathPoints.insert(min_ix);
                min_ix = i;
            }else OffPathPoints.insert(i);   
        }
        OnPathPoints.push_back(min_ix);
        Dist.resize(cnt);
        for(int i = 0; i < cnt; ++ i){
            Dist[i].resize(cnt);
            for(int j = 0; j < cnt; ++ j) Dist[i][j] = dist(points[i],points[j]);
        }
        for(;findConvexHull() == false;);
        findShortestPath();
        int res_point = -1;
        for(auto it = OnPathPoints.begin(); it != OnPathPoints.end(); ++ it){
            if(*it == Bot){
                int prev = (it == OnPathPoints.begin()) ? *std::prev(OnPathPoints.end()) : *std::prev(it);
                auto next_it = std::next(it);
                if(next_it == OnPathPoints.end()) next_it = OnPathPoints.begin();
                res_point = *next_it;
                if(Dist[res_point][*it] > Dist[prev][*it]) res_point = prev;
                else if((Dist[res_point][*it] == Dist[prev][*it]) && (rand() % 2)) res_point = prev;
            }
        }
        if(res_point >= 0){
            return *new std::list<int>(1,res_point);
        }
        return *new std::list<int>();
    }

    private:
    std::set<int> OffPathPoints;
    std::list<int> OnPathPoints;
    std::vector<vector<int>> Dist;
    std::vector<Point> Points;
    int Bot;

    void findShortestPath(){
        int pathLength = 0;
        for(auto it=OnPathPoints.begin(), next_it = ++OnPathPoints.begin(); it != OnPathPoints.end(); ++ it, ++next_it){
            if(next_it == OnPathPoints.end()) next_it = OnPathPoints.begin();
            pathLength += Dist[*it][*next_it];
        }
        for(;OffPathPoints.size() != 0;){
            int min_len = std::numeric_limits<int>::max();
            auto min_it = OffPathPoints.begin();
            auto min_jt = OnPathPoints.begin();
            for(auto it = OffPathPoints.begin(); it != OffPathPoints.end(); ++ it){
                for(auto jt=OnPathPoints.begin(), next_jt = ++OnPathPoints.begin(); jt != OnPathPoints.end(); ++ jt, ++next_jt){
                    if(next_jt == OnPathPoints.end()) next_jt = OnPathPoints.begin();
                    auto len = pathLength - Dist[*jt][*next_jt] + Dist[*jt][*it] + Dist[*next_jt][*it];
                    if(len < min_len) min_len = len, min_jt = next_jt, min_it = it;
                    else if(len == min_len){
                        if((Dist[Bot][*it] < Dist[Bot][*min_it]) ||
                         ((Dist[Bot][*it] == Dist[Bot][*min_it]) && (std::rand() % 2)))
                        {
                            min_len = len, min_jt = next_jt, min_it = it;
                        } 
                    }
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
            int sign = (Points[p1].x - Points[p0].x) * (Points[*it].y - Points[p0].y) - 
                        (Points[p1].y - Points[p0].y)*(Points[*it].x - Points[p0].x); 
            if(sign > 0 || 
                ((sign == 0) && (Dist[p0][p1] > Dist[p0][*it]))) p1 = *it;
        }
        if(p1 == OnPathPoints.front()) return true;
        OnPathPoints.push_back(p1);
        OffPathPoints.erase(p1);
        return false;
    }
};