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
        auto points = FindPointInRegion(Region(0,0,Board[0].length(), Board.size()));
        std::cout<< points.size() << std::endl;
        int index = -1;
        if(points.size() == 0) return Point();
        if(points.size() == 1){
            index = 0;
        }
        else if(FindCover(points.size(), Board[0].length(), Board.size()) <= GlobalThreshold){
            index = *SparseExplorer->FindOptimalPath(points,Bot).begin();
        }else{
            DoPartitionOnBoard(minX, minY);
            if(Regions.size() == 0){
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
        int botix = -1;
        for(int i = 0; i < Ypartition.size(); ++ i){
            x = 0;
            for(int j = 0; j < Xpartition.size(); ++ j){
                auto rreg = Region(x,y,Xpartition[j], Ypartition[i]);
                auto cnt = FindPointInRegion(rreg).size();
                if(cnt != 0) Regions.push_back(rreg);
                if(rreg.IsPointInside(Bot)){
                    BotReg = rreg;
                    if(cnt != 0) botix = cnt - 1;
                }
                x += Xpartition[j];
            }
            y += Ypartition[i];
        }
        if(botix >= 0) Regions.clear();
    }



};

