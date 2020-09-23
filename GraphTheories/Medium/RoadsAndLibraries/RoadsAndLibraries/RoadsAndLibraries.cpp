// RoadsAndLibraries.cpp : This file contains the 'main' function. Program execution begins and ends there.
// https://www.hackerrank.com/challenges/torque-and-development/problem

#include <iostream>
#include <vector>
#include <fstream>

#define LOCAL_TEST

class RoadAndLibrary {

    std::vector<int> mStack;
    std::vector<std::vector<int>> mGraph;
    std::vector<int> mStateOfNode;
    int mClib;
    int mCroad;
public:
    RoadAndLibrary(int n, int clib, int croad, std::vector<std::vector<int>>&& graph) : mClib(clib), mCroad(croad) {
        mGraph = std::move(graph);
        mStack.clear();
        mStack.reserve(n);
        mStateOfNode.resize(n);
        for (int i = 0; i < n; ++i) {
            mStateOfNode[i] = mGraph[i].size();
        }        
    }

    long FindMinCost() {
        long x1 = mClib - mCroad;
        if (x1 < 0) return (long)mGraph.size() * mClib;
        if (x1 == 0) return (long)mGraph.size() * mCroad;
        return (long)mGraph.size() * mCroad + x1 * findNumberOfTrees();
    }

private:
    long findNumberOfTrees() {
        long res = 0;
        for (int i = 0; i < mStateOfNode.size(); ++i) {
            if (mStateOfNode[i] >= 0) {
                res++;
                deepSearch(i);
            }
        }
        return res;
    }

    void deepSearch(int nodeId) {
        if (mStateOfNode[nodeId] < 0) return;
        mStack.push_back(nodeId);
        for (; mStack.size() != 0;) {
            int id = mStack.back();
            int curPos = -- mStateOfNode[id];
            if (curPos < 0) mStack.pop_back();
            else mStack.push_back(mGraph[id][curPos]);   
        }
    }
};


int main()
{
#ifdef LOCAL_TEST
    std::ifstream in("../input.txt");
    std::streambuf* cinbuf = std::cin.rdbuf(); //save old buf
    std::cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt
#endif
    int q;
    std::cin >> q;
    for (int j = 0; j < q; ++j) {
        int n, m, clib, croad;
        std::cin >> n >> m >> clib >> croad;
        std::vector<std::vector<int>> graph(n);
        for (int i = 0; i < m; ++i) {
            int v1, v2;
            std::cin >> v1 >> v2;
            v1--;
            v2--;
            graph[v1].push_back(v2);
            graph[v2].push_back(v1);
        }
        RoadAndLibrary res(n, clib, croad, std::move(graph));
        std::cout << res.FindMinCost() << std::endl;
    }
    return 0;
}
