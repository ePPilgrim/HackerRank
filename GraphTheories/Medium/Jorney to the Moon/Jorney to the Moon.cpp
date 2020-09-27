// Jorney to the Moon.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>


#define LOCAL_TEST

std::vector<int> forest;

void assignValue(int id, int value) {
    for (; forest[id] >= 0;) {
        int k = forest[id];
        forest[id] = value;
        id = k;
    }
}

int findRoot(int id) {
    for (; forest[id] >= 0; id = forest[id]);
    return id;
}

void merge(int id1, int id2) {
    int root1 = findRoot(id1);
    int root2 = findRoot(id2);
    if (root1 == root2) return;
    assignValue(id1, root1);
    assignValue(id2, root1);
    forest[root2] = root1;
}


unsigned long long int journeyToMoon(int n, std::vector<std::vector<int>> astronaut) {
    if (n == 1) return 0;
    forest.resize(n);
    for (int i = 0; i < n; ++i) forest[i] = -1 - i;
    for (int i = 0; i < astronaut.size(); ++i) {
        merge(astronaut[i][0], astronaut[i][1]);
    }

    for (int i = 0; i < n; ++i) {
        if (forest[i] < 0) continue;
        int root = findRoot(i);
        assignValue(i, forest[root]);
    }

    std::vector<unsigned long long int> cntv(n, 0);
    for (int i = 0; i < n; ++i) {
        cntv[-1 - forest[i]] ++;
    }

    unsigned long long int sum = 0;
    unsigned long long int res = 0;
    for (int i = 0; i < n; ++ i) {
        res += cntv[i] * sum;
        sum += (unsigned long long int)cntv[i];
    }
    return res;
}


int main()
{
#ifdef LOCAL_TEST
    std::ifstream in("./input.txt");
    std::streambuf* cinbuf = std::cin.rdbuf(); //save old buf
    std::cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!
#endif
    int n = 0, p = 0;
    std::cin >> n >> p;
    std::vector<std::vector<int>> astronauts(p, std::vector<int>(2));
    for (int i = 0; i < p; ++i) {
        std::cin >> astronauts[i][0] >> astronauts[i][1];
    }
    unsigned long long int ans = journeyToMoon(n, astronauts);
    std::cout << ans << std::endl;
    return 0;
}

