#include <bits/stdc++.h>
#include <chrono>
using namespace std;
typedef unsigned long long Long;
const int MAX_NUMBER_OF_NODES = 4000000;

struct Node {
    int LinkedNode;
    int TerminalNode;
    pair<int, int> Span;
    vector<int> Children;
    Node(int linkedNode, int fromIndex, int toIndex) : LinkedNode(linkedNode), TerminalNode(-1), Span(fromIndex, toIndex), Children(vector<int>(26, -1))
    {}
};

struct Tree {
    vector<Node> Nodes;
    vector<Node> TerminalNodes;
    vector<int> Permutation;
    Tree()
    {
        Nodes.reserve(MAX_NUMBER_OF_NODES);
        TerminalNodes.reserve(MAX_NUMBER_OF_NODES);
    }
};

class BfsTreeBuilder {
    queue<int> Queue;

    void releaseResources() {
        Queue = move(queue<int>());
    }

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
        queue<int> depthQueue;
        Queue.push(0);
        depthQueue.push(0);
        tree.Nodes.push_back(Node(0, 0, genes.size()));
        for (; !Queue.empty(); Queue.pop(), depthQueue.pop()) {
            int state = Queue.front();
            int depth = depthQueue.front();
            for (int i = tree.Nodes[state].Span.first; i < tree.Nodes[state].Span.second; ++i) {
                if (genes[i].length() == depth) {
                    if (tree.Nodes[state].TerminalNode < 0) {
                        int termState = tree.TerminalNodes.size();
                        tree.TerminalNodes.push_back(Node(termState, i, i + 1));
                        tree.Nodes[state].TerminalNode = termState;
                    }
                    else {
                        tree.TerminalNodes[tree.Nodes[state].TerminalNode].Span.second++;
                    }
                    continue;
                }
                int index = int(genes[i][depth]) - int('a');
                int child_state = tree.Nodes[state].Children[index];
                if (child_state >= 0) {
                    tree.Nodes[child_state].Span.second++;
                    continue;
                }
                Queue.push(tree.Nodes.size());
                depthQueue.push(depth + 1);
                tree.Nodes[state].Children[index] = tree.Nodes.size();
                tree.Nodes.push_back(Node(tree.Nodes.size(), i, i + 1));
            }
        }
    }

    void buildFailureFunction(Tree& tree) {
        tree.Nodes[0].LinkedNode = -1;
        Queue.push(0);
        for (; !Queue.empty(); Queue.pop()) {
            int state = Queue.front();
            for (int i = 0; i < 26; ++i) {
                int child_state = tree.Nodes[state].Children[i];
                if (child_state < 0) continue;
                Queue.push(child_state);
                int prefix = tree.Nodes[state].LinkedNode;
                for (; prefix >= 0 && tree.Nodes[prefix].Children[i] < 0; prefix = tree.Nodes[prefix].LinkedNode);
                tree.Nodes[child_state].LinkedNode = (prefix < 0) ? 0 : tree.Nodes[prefix].Children[i];
            }
        }
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

class Diagnostic {
    vector<string> Paths;
    void fillPaths(int state, string prefix, const vector<Node>& nodes) {
        Paths[state] = prefix;
        for (int i = 0; i < 26; ++i) {
            int child_state = nodes[state].Children[i];
            if (child_state < 0) continue;
            fillPaths(child_state, prefix + char(int('a') + i), nodes);
        }
    }

    void getInfo(int state, const vector<Node>& nodes) {
        cout << "(" << Paths[state] << ")" << "   ";
        if (nodes[state].TerminalNode >= 0) cout << "(+)";
        else cout << "(-)";
        cout << "   ";
        if (nodes[state].LinkedNode >= 0) cout << "(" << Paths[nodes[state].LinkedNode] << ")";
        cout << endl;
        for (int i = 0; i < 26; ++i) {
            int child_state = nodes[state].Children[i];
            if (child_state < 0) continue;
            getInfo(child_state, nodes);
        }
    }
public:
    void DoDiagnostic(const vector<Node>& nodes) {
        Paths.resize(nodes.size());
        fillPaths(0, string(), nodes);
        getInfo(0, nodes);
    }
};

class DeterminingDnaHealth
{
    Tree DnaTree;
    vector<Long> AccumulativeWeights;
    vector<unsigned int> Weights;
    vector<double> Clocks;
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
        Weights.resize(n);
        for (int i = 0; i < n; stream >> Weights[DnaTree.Permutation[i++]]);
        AccumulativeWeights.resize(n, 0);
        AccumulativeWeights[0] = static_cast<Long>(Weights[0]);
        for (int i = 1; i < n; ++i) {
            if (genes[i] == genes[i - 1])  AccumulativeWeights[i] = Weights[i] + AccumulativeWeights[i - 1];
            else {
                AccumulativeWeights[i] = static_cast<Long>(Weights[i]);
            }
        }
    }

    bool isStrandHealthy(const Node& node, const set<int>& validSet) {
        if (*validSet.rbegin() < node.Span.first) return false;
        if (*validSet.begin() >= node.Span.second) return false;
        // auto it = validSet.lower_bound(node.Span.first);
        // if(it == validSet.end()) return false;
        // if(*it < node.Span.second) return true;
        return true;
    }

    Long calculateTotalHealth(const Node& node, const set<int>& validSet) {
        if (!isStrandHealthy(node, validSet)) return 0;
        int from = node.Span.first;
        int to = node.Span.second;
        auto it = validSet.lower_bound(from);
        if (*it >= to) return 0;
        from = *it;
        it = validSet.lower_bound(to);
        to = (it != validSet.end()) ? *--it : *validSet.rbegin();
        return AccumulativeWeights[to] - AccumulativeWeights[from] + static_cast<Long>(Weights[from]);
    }

    Long findTotalHealthOfStrand(int left, int right, const string& dnaStrand) {
        clock_t s1 = clock();
        vector<int> list(right - left + 1);
        generate(list.begin(), list.end(), [&, i = left]() mutable { return DnaTree.Permutation[i++]; });
        set<int> validSet(list.begin(), list.end());
        list = vector<int>(DnaTree.TerminalNodes.size(), 0);
        clock_t s2 = clock();
        for (int i = 0, state = 0; i < dnaStrand.size(); ++i) {
            int id = dnaStrand[i] - int('a');
            for (; state >= 0 && DnaTree.Nodes[state].Children[id] < 0 /*&& !isStrandHealthy(DnaTree.Nodes[state], validSet)*/;
                state = DnaTree.Nodes[state].LinkedNode);
            if (state < 0) {
                state = 0;
                continue;
            }

            state = DnaTree.Nodes[state].Children[id];
            for (int sufix_state = state; sufix_state > 0; sufix_state = DnaTree.Nodes[sufix_state].LinkedNode) {
                if (DnaTree.Nodes[sufix_state].TerminalNode >= 0) list[DnaTree.Nodes[sufix_state].TerminalNode] ++;
            }
        }
        clock_t s3 = clock();
        Long totalHealth = 0;
        for (int i = 0; i < list.size(); ++i) {
            totalHealth += list[i] * calculateTotalHealth(DnaTree.TerminalNodes[i], validSet);
        }
        clock_t s4 = clock();
        Clocks[0] += (double)s2 - s1;
        Clocks[1] += (double)s3 - s2;
        Clocks[2] += (double)s4 - s3;
        //
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
        Clocks = { 0.0, 0.0, 0.0, 0.0 };
        for (int i = 0; i < n; ++i) {
            string dnaStrand;
            *PtrToStream >> l >> r >> dnaStrand;
            Long res = findTotalHealthOfStrand(l, r, dnaStrand);
            if (res > maxHealth) maxHealth = res;
            if (res < minHealth) minHealth = res;
            //cout << ", ";
        }
        //cout << endl;
        cout << minHealth << ' ' << maxHealth << endl;
        for (int i = 0; i < Clocks.size(); ++i) {
            Clocks[i] /= CLOCKS_PER_SEC;
            cout << "S" << i << ":  " << Clocks[i] << endl;
        }
        cout << endl;
    }
};

int main()
{
    //  BfsTreeBuilder dna;
    //  vector<string> genes = {"a", "ab", "bab", "bc", "bca", "c", "caa"};
    //  Tree tree = dna.BuildTree(genes);
    //  Diagnostic obj;
    //  obj.DoDiagnostic(tree.Nodes);


    clock_t start, end;

    start = clock();
    DeterminingDnaHealth determiningDnaHealth("input.txt");
    determiningDnaHealth.FindSolution();
    end = clock();
    cout << "Time = " << ((float)end - start) / CLOCKS_PER_SEC << endl;
    //0 1970060

    cout << int('z') - int('a') + 2 << endl;
    return 0;
};