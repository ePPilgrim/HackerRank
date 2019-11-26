#include <vector>
#include <algorithm>

using namespace std;

struct Point{
    int x;
    int y;
    Point(int xx, int yy) : x(xx), y(yy){}
    Point() {x = y = -1;}
    int Dist(const Point& p) {return (x - p.x) * (x - p.x) + (y - p.y) * (y - p.y);}
};

struct Region{
    Point p;
    int w;
    int h;
    Region(int xx = -1, int yy = -1, int ww = 0, int hh = 0) : p(Point(xx,yy)), w(ww), h(hh){}
    bool IsPointInside(const Point& pp){
        return (pp.x < p.x + w) && (pp.x >= p.x) && (pp.y < p.y + h) && (pp.y >= p.y);  
    }
};

struct DP{
    int prev;
    int dist;
    DP(int _prev, int _dist) : prev(_prev), dist(_dist){}
};

class RegionStrategy
{
    public:
    float LocalThreshold;
    float GlobalThreshold;
    int MinS;
    Region BotReg;
    std::vector<Region> Regions;
    std::vector<Point> Points;

    RegionStrategy(float localThreshold, float globalThreshold, int minS):
    LocalThreshold(localThreshold), 
    GlobalThreshold(globalThreshold),
    MinS(minS){}

    Point FindNextPoint(const vector<string>& board, const Point& bot){
        Points = FindPointsInRegion(Region(0,0,board[0].length(), board.size()),board);
        int cnt = Points.size();
        if(cnt == 0) return Point();
        if(cnt == 1) return Points[0];
        if(FindCover(cnt, board[0].length(), board.size()) <= GlobalThreshold) return FindOptimalPointSparce(bot);
        Points.clear();
        DoPartitionOnBoard(board, bot);
        if(IsRegionActive(BotReg,board)){
            Points = FindPointsInRegion(BotReg,board);
            if(FindCover(Points.size(),  BotReg.h, BotReg.w) <= LocalThreshold) FindOptimalPointSparce(bot);    
            return FindOptimalPointDense(bot); 
        } else{
            Points = std::vector<Point>();
            Points.reserve(100);
            for(auto it = Regions.begin(); it != Regions.end(); ++ it){
                if(IsRegionActive(*it,board))
                    Points.push_back(Point(it->p.x + (it->w/2), it->p.y + (it->h/2)));
            }
            return FindOptimalPointSparce(bot); 
        }
    }

    float FindCover(int cnt, int w, int h){return ((float)cnt) / ((float) h * w);}

    std::vector<Point>& FindPointsInRegion(const Region& reg, const vector<string>& board){
        int w = (reg.p.x + reg.w > board[0].length()) ? board[0].length() : reg.p.x + reg.w;
        int h = (reg.p.y + reg.h > board.size()) ? board.size() : reg.p.y + reg.h;
        auto res = new vector<Point>();
        res->reserve(reg.h * reg.w);
        for(int y = reg.p.y; y < h ; y ++){
            for(int x = reg.p.x; x < w; ++ x) if(board[y][x] == 'd') res->push_back(Point(x,y));
        }
        return *res;
    }

    bool IsRegionActive(const Region& reg, const vector<string>& board){
        for(int i = reg.p.y; i < reg.p.y + reg.h; ++ i){
            for(int j = reg.p.x; j < reg.p.x + reg.w; ++ j){
                if(board[i][j] == 'd') return true;
            }
        }
        return false;
    }

    void DoPartitionOnBoard(const vector<string>& board, const Point& bot){
        int w = board[0].length();
        int h = board.size();
        int nx = w / MinS;
        int ny = h / MinS;
        auto Xpartition = std::vector<int>();
        auto Ypartition = std::vector<int>();
        if(nx == 0) Xpartition = std::vector<int>(1,w);
        else{
            Xpartition = std::vector<int>(nx,MinS);
            for(int rx = w % MinS, i = 0; rx > 0; -- rx, ++ i){
                Xpartition[i % nx] ++;
            }
        }
        if(ny == 0) Ypartition = std::vector<int>(1,h);
        else{
            Ypartition = std::vector<int>(ny,MinS);
            for(int ry = h % MinS, i = 0; ry > 0; -- ry, ++ i){
                Ypartition[i % ny] ++;
            }
        }
        int x,y = 0;
        for(int i = 0; i < Ypartition.size(); ++ i){
            x = 0;
            for(int j = 0; j < Xpartition.size(); ++ j){
                auto rreg = Region(x,y,Xpartition[j], Ypartition[i]);
                Regions.push_back(rreg);
                if(rreg.IsPointInside(bot)){
                    BotReg = rreg;
                }
                x += Xpartition[j];
            }
            y += Ypartition[i];
        }
    }

    Point FindOptimalPointDense(Point bot){
        int index,bx,by = -1;
        const int cnt = Points.size();
        bool isOnBorder = true;
        auto compX = [](const Point& p1, const Point& p2){return p1.x < p2.x;};
        int x_min = std::min_element(Points.begin(), Points.end(), compX)->x;
        if(bot.x == x_min) bx = x_min;
        else{
            int x_max = std::max_element(Points.begin(), Points.end(), compX)->x;
            if(bot.x == x_max) bx = x_max;
            else{
                auto compY = [](const Point& p1, const Point& p2){return p1.y < p2.y;};
                int y_min = std::min_element(Points.begin(), Points.end(), compY)->y;
                if(bot.y == y_min) by = y_min;
                else{
                    int y_max = std::max_element(Points.begin(), Points.end(), compY)->y;
                    if(bot.y == y_max) by = y_max;
                    else isOnBorder = false;
                }
            }
        }
        int min_dist = std::numeric_limits<int>::max();
        for(int i = 0; i < cnt; ++ i){
            int ddist = bot.Dist(Points[i]);
            if( isOnBorder ){
                if(Points[i].x == bot.x) ddist = std::abs(Points[i].y - bot.y);
                else ddist = std::abs(Points[i].x - bot.x);   
            }
            if((ddist < min_dist) || (ddist == min_dist && std::rand() % 2)){
                min_dist = ddist;
                index = i;
            }
        }
        return Points[index];  
    }
    Point FindOptimalPointSparce(Point bot){
        int cnt = Points.size();
        auto Dist = std::vector<std::vector<int>>(cnt);
        for(int i = 0; i < cnt; ++ i){
            Dist[i].resize(cnt);
            for(int j = 0; j < cnt; ++ j) Dist[i][j] = Points[i].Dist(Points[j]);
        }

        auto dp = vector<vector<DP>>();
        for(int i = 0; i < cnt; ++ i) dp.push_back(vector<DP>(cnt,DP(-1,std::numeric_limits<int>::max())));
        for(int i = 0; i < cnt; ++ i) dp[0][i].dist = bot.Dist(Points[i]);
        auto flags = vector<int>(cnt,-1);
        for(int i = 1; i < cnt; ++ i){
            std::fill(flags.begin(),flags.end(),-1);
            for(int j = 0; j < cnt; ++ j){
                if(i == 1 || dp[i - 1][j].prev >= 0){
                    int m = j;
                    for(int k = i - 1;k >= 0; -- k) {flags[m] = j; m = dp[k][m].prev;}
                    for(int k = 0; k < cnt; ++ k){
                        if(flags[k] != j){
                            int dd = dp[i-1][j].dist + Dist[j][k];
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
        for(int i = cnt - 1; i > 0; -- i) k_min = dp[i][k_min].prev; 
        return Points[k_min]; 
    }
};

inline string next_move(int posr, int posc, int dimh, int dimw, vector <string> board) {
    string res = "EMPTY";
    if(board[posr][posc] == 'd') res = "CLEAN";
    else{
        int minS = 5;
        float globalThreshold = 0.05f;
        float localThreshold = 0.5f;
        Point bot = Point(posc, posr);
        auto partition = RegionStrategy(localThreshold, globalThreshold, minS);
        Point p = partition.FindNextPoint(board,bot);
    
        if(bot.x == p.x){
            res = "UP";
            if(bot.y < p.y) res = "DOWN";
        }
        if(bot.y == p.y){
            res = "LEFT";
            if(bot.x < p.x) return "RIGHT";
        }
        if(std::rand() % 2){
            res = "LEFT";
            if(bot.x < p.x) return "RIGHT";
        } else{
            res = "UP";
            if(bot.y < p.y) return "DOWN";
        }
    }
    return res;
}