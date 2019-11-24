#ifndef NEXTPOINTSTRATEGY_H
#define NEXTPOINTSTRATEGY_H

#include "Commons.h"

class NextPointStrategy
{
    public:
    virtual list<int>& FindOptimalPath(const vector<Point>& points, const Point& bot) = 0;
};

#endif