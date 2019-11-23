#include "NextPointStrategy.h"

class DPStrategy : public NextPointStrategy{
    public:
    list<int>& FindOptimalPath(const vector<Point>& points, const Point& bot){
        int cnt = points.size();
        if(cnt < 1) return *new list<int>(cnt,0);    

        auto dp = vector<vector<DP>>();
        for(int i = 0; i < cnt; ++ i) dp.push_back(vector<DP>(cnt,DP(-1,std::numeric_limits<int>::max())));
        for(int i = 0; i < cnt; ++ i) dp[0][i].dist = dist(bot,points[i]);
        auto flags = vector<int>(cnt,-1);
        for(int i = 1; i < cnt; ++ i){
            std::fill(flags.begin(),flags.end(),-1);
            for(int j = 0; j < cnt; ++ j){
                if(i == 1 || dp[i - 1][j].prev >= 0){
                    int m = j;
                    for(int k = i - 1;k >= 0; -- k) {flags[m] = j; m = dp[k][m].prev;}
                    for(int k = 0; k < cnt; ++ k){
                        if(flags[k] != j){
                            int dd = dp[i-1][j].dist + dist(points[j],points[k]);
                            if(dp[i][k].dist > dd){
                                dp[i][k].dist = dd;
                                dp[i][k].prev = j;
                            } 
                            if(dd == dp[i][k].dist){
                                if(std::rand() % 2){
                                    dp[i][k].dist = dd;
                                    dp[i][k].prev = j; 
                                }
                            }
                        }
                    }
                }
            }   
        }
        int min_dist = std::numeric_limits<int>::max();
        int k_min = -1;
        for(int i = 0; i < cnt; ++ i){
            if(min_dist > dp[cnt-1][i].dist){
                k_min = i;
                min_dist = dp[cnt-1][i].dist;
            }
        }

        auto res = new list<int>();    
        res->push_front(k_min);
        for(int i = cnt - 1; i > 0; -- i){
            k_min = dp[i][k_min].prev; 
            res->push_front(k_min);
        }
        return *res;
    }
};