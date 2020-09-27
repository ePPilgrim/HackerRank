// Synchronous Shopping.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <queue>
#include <fstream>

using namespace std;

#define LOCAL_TEST

struct Node {
    int ftype = 0;
    vector<pair<int,int>> neighbors;
};

vector<vector<long>> timem;
vector<vector<int>> ftypem;
vector<Node> graph;
int K = 0;

void init();
vector<pair<int,int>> doStep(int id, int ftype);

long shop() {
    init();
    std::queue<pair<int,int>> nodev;
    nodev.push(make_pair(0,graph[0].ftype));

    for (; !nodev.empty();) {
        auto node = nodev.front();
        nodev.pop();
        auto nextNodev = doStep(node.first, node.second);
        for(auto val : nextNodev) {
            nodev.push(val);
        }
    }

    int n = timem.size() - 1;
    long res = std::numeric_limits<long>().max();
    for (int i = 0; i < K; ++i) {
        for (int j = 0; j < K; ++j) {
            if ((i | j) == K - 1) {
                res = std::min(res, std::max(timem[n][i], timem[n][j]));
            }
        }
    }
    
    return res;
}


int main()
{
#ifdef LOCAL_TEST
    std::ifstream in("input.txt");
    std::streambuf* cinbuf = std::cin.rdbuf(); //save old buf
    std::cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!
#endif
    long res = shop();
    std::cout << res << std::endl;
}

vector<pair<int,int>> doStep(int id, int ftype) {
    int time = timem[id][ftype];
    int sz = graph[id].neighbors.size();
    vector<pair<int, int>> res;
    res.reserve(sz);
    for (int i = 0; i < sz; ++i) {
        int cId = graph[id].neighbors[i].first;
        int cTime = time + graph[id].neighbors[i].second;
        int cftype = ftype | graph[cId].ftype;
        if (cTime < timem[cId][cftype]) {
            timem[cId][cftype] = cTime;
            res.push_back(make_pair(cId, cftype));
        }
    }
    return res;
}

void init(){
    int n, k, m, fcnt, ftype, s1, s2, t;
    std::cin >> n >> m >> k;
    timem.resize(n);
    graph.resize(n);
    K = 1 << k;
    for (int i = 0; i < n; ++i) timem[i].resize(K, std::numeric_limits<long>().max());
    
    for (int i = 0; i < n; ++i) {
        cin >> fcnt;
        for (int j = 0; j < fcnt; ++j) {
            cin >> ftype;
            graph[i].ftype |= 1 << ftype - 1;
        }
    }
    timem[0][graph[0].ftype] = 0;

    for (int i = 0; i < m; ++i) {
        cin >> s1 >> s2 >> t;
        graph[s1 - 1].neighbors.push_back(make_pair(s2 - 1, t));
        graph[s2 - 1].neighbors.push_back(make_pair(s1 - 1, t));
    }
}
