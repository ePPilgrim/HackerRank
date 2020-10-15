
// SubsetComponent.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <queue>
#include <fstream>

using namespace std;

#define LOCAL_TEST


static int buff[64];

struct SubGraph {
    int Id = 0;
    int Cnt = 64;
    unsigned char* Components;

    SubGraph(int id, unsigned char* components) {
        Id = id;
        Components = new unsigned char[64];
        std::memcpy(Components, components, 64);
    }

    SubGraph spawnSubgraph(int id, const vector<int>& single) {
        SubGraph newSubGraph(id, Components);
        unsigned char setId = Components[single[0]];
        std::memset((void*)buff, 0, 256);
        for (auto i : single) {
            buff[Components[i]] = 1;
        }
        for (int i = 0; i < 64; ++ i) {
            if (buff[Components[i]]) {
                newSubGraph.Components[i] = setId;
            }
        }
        int sum = 0;
        for (int i = 0; i < 64; ++i) sum += buff[i];
        newSubGraph.Cnt = Cnt + 1 - sum;
        return newSubGraph; 
    }
};


unsigned int findConnectedComponents() {
    vector<bool> flagv(1 << 21, true);
    queue<SubGraph> graphs;
    unsigned char* temp = new unsigned char[64];
    for (int i = 0; i < 64; ++i) {
        temp[i] = (unsigned char)i;
    }
    graphs.push(SubGraph(0,temp));
    vector<vector<int>> components;
    int ans = 64;
    int n = 0;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        uint64_t subgraph = 0;
        cin >> subgraph;
        vector<int> component;
        component.reserve(64);
        for (int j = 0; j < 64; ++j) {
            if (1 & (subgraph >> j)) {
                component.push_back(j);
            }
        }
        components.push_back(component);
    }
    for(; graphs.size() != 0;){
        SubGraph singleG = graphs.front();
        graphs.pop();
        for (int i = 0; i < components.size(); ++i) {
            int id = (1 << i) | singleG.Id;
            if (flagv[id]) {
                graphs.push(singleG.spawnSubgraph(id, components[i]));
                ans += graphs.back().Cnt;
                flagv[id] = false;
            }
        }
    }
    return ans;
}

int main()
{
#ifdef LOCAL_TEST
    std::ifstream in("input.txt");
    std::streambuf* cinbuf = std::cin.rdbuf(); //save old buf
    std::cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!
#endif
    unsigned int ans = findConnectedComponents();
    std::cout << ans << std::endl;
}


