#include "Commons.h"
class NextPointStrategy
{
    public:
    virtual list<int>& FindOptimalPath(const vector<Point>& points, const Point& bot) = 0;
};