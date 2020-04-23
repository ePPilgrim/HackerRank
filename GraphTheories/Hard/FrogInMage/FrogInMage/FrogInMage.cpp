// FrogInMage.cpp : This file contains the 'main' function. Program execution begins and ends there.
// https://www.hackerrank.com/challenges/frog-in-maze/problem

#include<iostream>
#include<set>
#include<vector>
#include<algorithm>
#include<string>
#include <fstream>

#define LOCAL_TEST

const double eps = 1e-10;

inline bool isZero(double val) {
	return std::abs(val) < eps;
}

struct Node {
	int id;
	char type;
	std::vector<std::pair<int,int>> neighbors;
	std::pair<int, int> cell;
	Node() : id(-1),type('O'),cell(std::make_pair(-1, -1)) { neighbors.reserve(4);}
};

std::vector<std::vector<double>> ReadAndModifyFromStream(int& inId, std::vector<double>& outTr) {
	int n, m, k;
	std::cin >> n >> m >> k;
	auto graph = std::vector<std::vector<Node>>(n);
	for (int i = 0; i < n; ++ i) {
		graph[i] = std::vector<Node>(m);
		std::string str;
		std::cin >> str;
		for (int j = 0; j < m; ++j) {
			graph[i][j].type = str[j];
			graph[i][j].cell = std::make_pair(i, j);
		}
	}
	for (int i = 0; i < k; ++i) {
		int i1, j1, i2, j2;
		std::cin >> i1 >> j1 >> i2 >> j2;
		graph[i1 - 1][j1 - 1].cell = std::make_pair(i2 - 1, j2 - 1);	
		graph[i2 - 1][j2 - 1].cell = std::make_pair(i1 - 1, j1 - 1);
	}
	int id = 0;
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			if (graph[i][j].type == '#' || graph[i][j].type == '*' || graph[i][j].type == '%'  ) continue;
			for (int i1 = -1; i1 < 2; ++ i1) {
				for (int j1 = -1; j1 < 2; ++j1) {
					if (i1 * i1 + j1 * j1 == 1) {
						int k = i + i1;
						int l = j + j1;
						if (k >= 0 && k < n && l >= 0 && l < m && graph[k][l].type != '#')	graph[i][j].neighbors.push_back(graph[k][l].cell);
					}
				}
			}
			if (graph[i][j].neighbors.size() != 0) graph[i][j].id = id++;
			if (graph[i][j].type == 'A') inId = graph[i][j].id;
		}
	}
	auto mat = std::vector<std::vector<double>>();
	if (inId < 0) return std::move(mat);
	mat.resize(id);
	outTr.resize(id,0.0);
	for (int i = 0; i < id; ++i) mat[i] = std::vector<double>(id, 0.0);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) {
			int k = graph[i][j].id;
			if ( k >= 0) {
				mat[k][k] = 1.0;
				double p = -1.0 / graph[i][j].neighbors.size();
				for (auto iter = graph[i][j].neighbors.begin(); iter != graph[i][j].neighbors.end(); ++iter) {
					int r = iter->first;
					int c = iter->second;
					if (graph[r][c].id >= 0) mat[graph[r][c].id][k] = p;
					else if (graph[r][c].type == '%') outTr[k] += -p;
				}
			}
		}
	}
	return std::move(mat);
}

class AbsorbingMarkovChain{
	std::vector<std::vector<double>> mMatrix;
	std::set<int> mNonZeroElements;
	std::vector<int> mRowPermutation;
	std::vector<double> mOutTrans;
	int mSz;

	void LUDecomposition() {
		for (int r = 0; r < mSz; ++r) {
			updateNoneZero(r);
			if (mNonZeroElements.size() == 0) continue;
			permute(r);
			for (auto iter = mNonZeroElements.begin(); iter != mNonZeroElements.end(); ++iter) {
				mMatrix[*iter][r] /= mMatrix[r][r];
				for (int c = r + 1; c < mSz; ++c) {
					mMatrix[*iter][c] -= mMatrix[*iter][r] * mMatrix[r][c];
				}
			}	
		}	
	}

	std::vector<double> findSingleColumnOfInvertMatrix(int col) {
		auto res = std::vector<double>(mSz,0.0);
		res[col] = 1.0;
		for (int r = 1; r < mSz; ++ r) {
			if (!isZero(mMatrix[r][r])) {
				for (int c = 0; c < r; ++ c) {
					res[r] -= res[c] * mMatrix[r][c];
				}
			}
		}
		for (int r = mSz - 1; r >= 0; --r) {
			if (!isZero(mMatrix[r][r])) {
				for (int c = r + 1; c < mSz; ++ c) {
					res[r] -= res[c] * mMatrix[r][c];
				}
				res[r] /= mMatrix[r][r];
			}
		}
		return res;
	}

	void permute(int start) {
		int idx = start;
		for (int r = start + 1; r < mSz; ++r) {
			if (std::abs(mMatrix[r][start]) > std::abs(mMatrix[idx][start])) {
				idx = r;
			}
		}
		if (idx != start) {
			std::swap(mMatrix[idx], mMatrix[start]);
			std::swap(mRowPermutation[idx], mRowPermutation[start]);
		}		
	}

	void updateNoneZero(int start) {
		mNonZeroElements.clear();
		for (int r = start + 1; r < mSz; ++r) {
			if (!isZero(mMatrix[r][start])){
				mNonZeroElements.insert(r);
			}
		}
	}
public:
	AbsorbingMarkovChain(std::vector<std::vector<double>>& inputMatrix, std::vector<double>& outTr) {
		mSz = inputMatrix.size();
		mMatrix = std::move(inputMatrix);
		mOutTrans = std::move(outTr);
		mRowPermutation = std::vector<int>(mSz);
		for (int i = 0; i < mSz; ++i) mRowPermutation[i] = i;
	}

	double FindSolution(int enterCell) {
		LUDecomposition();
		for (int i = 0; i < mSz; ++i) {
			if (mRowPermutation[i] == enterCell) {
				enterCell = i;
				break;
			}
		}
		auto vec = findSingleColumnOfInvertMatrix(enterCell);
		double ans = 0.0;
		if (!isZero(vec[enterCell])) {
			for (int i = 0; i < mSz; ++i) ans += vec[i] * mOutTrans[i];
		}
		return ans;
	}
};

int main()
{
#ifdef LOCAL_TEST
	std::ifstream in("../in39.txt");
	std::streambuf* cinbuf = std::cin.rdbuf(); //save old buf
	std::cin.rdbuf(in.rdbuf()); //redirect std::cin to in.txt!
#endif
	int inId;
	std::vector<double> outTr;
	std::vector<std::vector<double>> matrix = ReadAndModifyFromStream(inId, outTr);
	double ans = 0.0;
	if (inId >= 0) {
		AbsorbingMarkovChain solution(matrix, outTr);
		ans = solution.FindSolution(inId);
	}
	std::cout << ans << std::endl;
	return 0;
}



