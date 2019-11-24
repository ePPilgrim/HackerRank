#include "NextPointStrategy.h"

class RegionStrategy
{
    public:
    float LocalThreshold;
    float GlobalThreshold;
    NextPointStrategy* SparseExplorer;
    NextPointStrategy* DensityExplorer;
    std::vector<string> Board;
    Point Bot;
    Region BotReg;
    std::vector<Region> Regions;

    RegionStrategy(float localThreshold, float globalThreshold, 
                    NextPointStrategy* sparseExplorer, NextPointStrategy* densityExplorer,
                    const std::vector<string>& board, const Point& bot):
    LocalThreshold(localThreshold), 
    GlobalThreshold(globalThreshold),
    SparseExplorer(sparseExplorer),
    DensityExplorer(densityExplorer),
    Board(board),
    Bot(bot){}

    Point FindNextPoint(int minX, int minY){
        int cnt = FindCount(Region(0,0,Board[0].length(), Board.size()));
        std::vector<Point> points = std::vector<Point>();
        int index = -1;
        std::cout<< cnt << std::endl;
        if(cnt == 0) return Point();
        if(cnt == 1){
            points = FindPointInRegion(Region(0,0,Board[0].length(), Board.size()));
            index = 0; 
        }
        else if(FindCover(cnt, Board[0].length(), Board.size()) <= GlobalThreshold){
            points = FindPointInRegion(Region(0,0,Board[0].length(), Board.size()));
            index = *SparseExplorer->FindOptimalPath(points,Bot).begin();
        }else{
            DoPartitionOnBoard(minX, minY);
            if(IsRegionActive(BotReg)){
                points = FindPointInRegion(BotReg);
                if(FindCover(points.size(),  BotReg.h, BotReg.w) <= LocalThreshold){
                    index = *SparseExplorer->FindOptimalPath(points,Bot).begin();    
                }
                else{
                    index = *DensityExplorer->FindOptimalPath(points,Bot).begin(); 
                }
            } else{
                points = std::vector<Point>();
                points.reserve(100);
                for(auto it = Regions.begin(); it != Regions.end(); ++ it){
                    if(IsRegionActive(*it))
                        points.push_back(Point(it->p.x + (it->w/2), it->p.y + (it->h/2)));
                }
                index = *SparseExplorer->FindOptimalPath(points,Bot).begin(); 
            }
        }
        return points[index];
    }

    float FindCover(int cnt, int w, int h){
        return ((float)cnt) / ((float) h * w);
    }

    std::vector<Point>& FindPointInRegion(const Region& reg){
        int w = (reg.p.x + reg.w > Board[0].length()) ? Board[0].length() : reg.p.x + reg.w;
        int h = (reg.p.y + reg.h > Board.size()) ? Board.size() : reg.p.y + reg.h;
        auto res = new vector<Point>();
        for(int y = reg.p.y; y < h ; y ++){
            for(int x = reg.p.x; x < w; ++ x) if(Board[y][x] == 'd') res->push_back(Point(x,y));
        }
        return *res;
    }

    int FindCount(const Region& reg){
        int cnt = 0;
        for(int i = reg.p.y; i < reg.p.y + reg.h; ++ i){
            for(int j = reg.p.x; j < reg.p.x + reg.w; ++ j){
                if(Board[i][j] == 'd') cnt ++;
            }
        }
        return cnt;
    }

    bool IsRegionActive(const Region& reg){
        for(int i = reg.p.y; i < reg.p.y + reg.h; ++ i){
            for(int j = reg.p.x; j < reg.p.x + reg.w; ++ j){
                if(Board[i][j] == 'd') return true;
            }
        }
        return false;
    }

    void DoPartitionOnBoard(int minX, int minY){
        int w = Board[0].length();
        int h = Board.size();
        int nx = w / minX;
        int ny = h / minY;
        auto Xpartition = std::vector<int>();
        auto Ypartition = std::vector<int>();
        if(nx == 0) Xpartition = std::vector<int>(1,w);
        else{
            Xpartition = std::vector<int>(nx,minX);
            for(int rx = w % minX, i = 0; rx > 0; -- rx, ++ i){
                Xpartition[i % nx] ++;
            }
        }
        if(ny == 0) Ypartition = std::vector<int>(1,h);
        else{
            Ypartition = std::vector<int>(ny,minY);
            for(int ry = h % minY, i = 0; ry > 0; -- ry, ++ i){
                Ypartition[i % ny] ++;
            }
        }
        int x,y = 0;
        for(int i = 0; i < Ypartition.size(); ++ i){
            x = 0;
            for(int j = 0; j < Xpartition.size(); ++ j){
                auto rreg = Region(x,y,Xpartition[j], Ypartition[i]);
                Regions.push_back(rreg);
                if(rreg.IsPointInside(Bot)){
                    BotReg = rreg;
                }
                x += Xpartition[j];
            }
            y += Ypartition[i];
        }
    }



};

