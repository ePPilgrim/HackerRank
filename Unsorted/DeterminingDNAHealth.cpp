#include <bits/stdc++.h>
#include <chrono>

using namespace std;
typedef unsigned long long Long;
const int MAX_NUMBER_OF_NODES = 4000000;
const int MAX_NUMBER_OF_LETTERS = 26;
const int MAX_NUMBER_OF_WORDS = 100000;

struct Node {
    int LinkedNode;
    int TerminalNode;
    vector<int> Children;
    Node(int linkedNode) : LinkedNode(linkedNode), TerminalNode(-1), Children(vector<int>(MAX_NUMBER_OF_LETTERS, -1))
    {}
};

struct Tree {
    vector<Node> Nodes;
    vector<pair<int,int>> TerminalNodes;
    vector<int> Permutation;
    Tree()
    {
        Nodes.reserve(MAX_NUMBER_OF_NODES);
        TerminalNodes.reserve(MAX_NUMBER_OF_WORDS);
    }
};

class BfsTreeBuilder {
    struct NodeInfo {
        pair<int, int> Span;
        int Depth;
        NodeInfo(int from, int to, int depth) : Span(from,to), Depth(depth){}
    };
    vector<NodeInfo> NodeInfos;
    queue<int> Queue;
protected:
    void buildPermutation(Tree& tree, vector<string>& genes)
    {
        vector<int> permutation(genes.size());
        generate(permutation.begin(), permutation.end(), [n = 0]() mutable { return n++; });
        sort(permutation.begin(), permutation.end(), [&](int i, int j) { return (genes[i] == genes[j]) ? i < j : genes[i] < genes[j]; });
        vector<string> permutedGenes;
        permutedGenes.reserve(genes.size());
        tree.Permutation.resize(genes.size());
        for (int i = 0; i < permutation.size(); ++i) {
            permutedGenes.push_back(genes[permutation[i]]);
            tree.Permutation[permutation[i]] = i;
        }
        genes = move(permutedGenes);
    }

    void buildTreeStructure(Tree& tree, const vector<string>& genes)
    {
        Queue.push(0);
        NodeInfos.push_back(NodeInfo(0, genes.size(), 0));
        tree.Nodes.push_back(Node(0));
        for (; !Queue.empty(); Queue.pop()) {
            int state = Queue.front();
            int depth = NodeInfos[state].Depth;
            for (int i = NodeInfos[state].Span.first; i < NodeInfos[state].Span.second; ++i) {
                if (genes[i].length() == depth) {
                    if (tree.Nodes[state].TerminalNode < 0) {
                        tree.Nodes[state].TerminalNode = tree.TerminalNodes.size();
                        tree.TerminalNodes.push_back(pair<int, int>(i, i + 1));
                    }
                    else {
                        tree.TerminalNodes[tree.Nodes[state].TerminalNode].second++;
                    }
                    continue;
                }
                int index = int(genes[i][depth]) - int('a');
                int child_state = tree.Nodes[state].Children[index];
                if (child_state >= 0) {
                    NodeInfos[child_state].Span.second++;
                    continue;
                }
                Queue.push(tree.Nodes.size());
                NodeInfos.push_back(NodeInfo(i, i + 1, depth + 1));
                tree.Nodes[state].Children[index] = tree.Nodes.size();
                tree.Nodes.push_back(Node(tree.Nodes.size()));
            }
        }
    }

    void buildFailureFunction(Tree& tree) {
        tree.Nodes[0].LinkedNode = -1;
        Queue.push(0);
        for (; !Queue.empty(); Queue.pop()) {
            int state = Queue.front();
            for (int i = 0; i < MAX_NUMBER_OF_LETTERS; ++i) {
                int child_state = tree.Nodes[state].Children[i];
                if (child_state < 0) continue;
                Queue.push(child_state);
                int prefix = tree.Nodes[state].LinkedNode;
                for (; prefix >= 0 && tree.Nodes[prefix].Children[i] < 0; prefix = tree.Nodes[prefix].LinkedNode);
                tree.Nodes[child_state].LinkedNode = (prefix < 0) ? 0 : tree.Nodes[prefix].Children[i];
            }
        }
    }

    void releaseResources() {
        Queue = move(queue<int>());
        NodeInfos.clear();
    }
public:
    Tree BuildTree(vector<string>& genes) {
        Tree tree;
        buildPermutation(tree, genes);
        buildTreeStructure(tree, genes);
        buildFailureFunction(tree);
        releaseResources();
        return move(tree);
    }
};

class DeterminingDnaHealth
{
    Tree DnaTree;
    vector<Long> AccumulativeWeights;
    vector<int> WordHistogram;
    vector<pair<int,int>> MapToWords;

    istream* PtrToStream;

    istream* getStream(string filePath = "") {
        PtrToStream = nullptr;
        if (filePath.empty()) {
            PtrToStream = &cin;
        }
        else {
            ifstream* ptr_fstream = new ifstream();
            ptr_fstream->open(filePath);
            PtrToStream = ptr_fstream;
        }
        return PtrToStream;
    }

    void initialize(istream& stream) {
        int n;
        stream >> n;
        vector<string> genes(n);
        for (int i = 0; i < n; stream >> genes[i++]);
        BfsTreeBuilder builder;
        DnaTree = builder.BuildTree(genes);
        AccumulativeWeights = vector<Long>(n + 1, 0);
        for (int i = 0; i < n; stream >> AccumulativeWeights[DnaTree.Permutation[i++] + 1]);
        for (int i = 1; i <= n; ++i) AccumulativeWeights[i] += AccumulativeWeights[i - 1];
        vector<int> subMap(n);
        for (int i = 0; i < DnaTree.TerminalNodes.size(); ++i) {
            for (int j = DnaTree.TerminalNodes[i].first; j < DnaTree.TerminalNodes[i].second; subMap[j++] = i);
            DnaTree.TerminalNodes[i] = pair<int, int>(numeric_limits<int>::max(), numeric_limits<int>::min());
        }
        MapToWords.resize(n);
        for (int i = 0; i < n; ++i) 
            MapToWords[i] = pair<int,int>(subMap[DnaTree.Permutation[i]], DnaTree.Permutation[i]);
        WordHistogram = vector<int>(DnaTree.TerminalNodes.size(), 0);
    }

    Long findTotalHealthOfStrand(int left, int right, const string& dnaStrand) {
        for (int i = left; i <= right; ++i) {
            int j = MapToWords[i].first;
            int val = MapToWords[i].second;
            if (DnaTree.TerminalNodes[j].first > val) DnaTree.TerminalNodes[j].first = val;
            if (DnaTree.TerminalNodes[j].second < val) DnaTree.TerminalNodes[j].second = val;
        }

        for (int i = 0, state = 0; i < dnaStrand.size(); ++i) {
            int id = dnaStrand[i] - int('a');
            for (; state >= 0 && DnaTree.Nodes[state].Children[id] < 0;state = DnaTree.Nodes[state].LinkedNode);
            if (state < 0) {
                state = 0;
                continue;
            }
            state = DnaTree.Nodes[state].Children[id];
            for (int sufix_state = state; sufix_state > 0; sufix_state = DnaTree.Nodes[sufix_state].LinkedNode) {
                if (DnaTree.Nodes[sufix_state].TerminalNode >= 0) WordHistogram[DnaTree.Nodes[sufix_state].TerminalNode] ++;
            }
        }
        
        Long totalHealth = 0;
        for (int i = 0; i < DnaTree.TerminalNodes.size(); ++i) {
            if((WordHistogram[i] != 0) && (DnaTree.TerminalNodes[i].first <= DnaTree.TerminalNodes[i].second)) {
                totalHealth += WordHistogram[i] * (AccumulativeWeights[DnaTree.TerminalNodes[i].second + 1] - AccumulativeWeights[DnaTree.TerminalNodes[i].first]);
            }
        }

        for (int i = 0; i < WordHistogram.size(); ++i) {
            DnaTree.TerminalNodes[i] = pair<int, int>(numeric_limits<int>::max(), numeric_limits<int>::min());
            WordHistogram[i] = 0;
        }
        return totalHealth;
    }

public:
    DeterminingDnaHealth(string filePath = "")
    {
        initialize(*getStream(filePath));
    }

    void FindSolution() {
        Long minHealth = numeric_limits<Long>::max();
        Long maxHealth = numeric_limits<Long>::min();
        int n, l, r;
        *PtrToStream >> n;
        for (int i = 0; i < n; ++i) {
            string dnaStrand;
            *PtrToStream >> l >> r >> dnaStrand;
            Long res = findTotalHealthOfStrand(l, r, dnaStrand);
            if (res > maxHealth) maxHealth = res;
            if (res < minHealth) minHealth = res;
        }
        cout << minHealth << ' ' << maxHealth << endl;
    }
};

int main()
{
    clock_t start, end;

    start = clock();
    DeterminingDnaHealth determiningDnaHealth("input.txt");
    determiningDnaHealth.FindSolution();
    end = clock();
    cout << "Time = " << ((float)end - start) / CLOCKS_PER_SEC << endl;
    //0 1970060
    return 0;
};
