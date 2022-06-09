#include <bits/stdc++.h>

using namespace std;
typedef unsigned long long Long;
const int MAX_NUMBER_OF_NODES = 2000000;
const int MAX_NUMBER_OF_LETTERS = 26;
const int MAX_NUMBER_OF_WORDS = 100000;

struct Node {
    int LinkedNode;
    int TerminalNode;
    vector<int> Children;
    Node() : LinkedNode(-1), TerminalNode(-1), Children(vector<int>(MAX_NUMBER_OF_LETTERS, -1))
    {}
};

struct Tree {
    vector<Node> Nodes;
    vector<vector<int>> TerminalNodes;
    Tree()
    {
        Nodes.reserve(MAX_NUMBER_OF_NODES);
        Nodes.push_back(Node());
        TerminalNodes.reserve(MAX_NUMBER_OF_WORDS);
    }
};

class BfsTreeBuilder {
    queue<int> Queue;
protected:

    void expandTree(Tree& tree, const string& gene, int position)
    {
        int root = 0;
        for (int i = 0; i < gene.length(); ++i) {
            int id = gene[i] - 'a';
            if (tree.Nodes[root].Children[id] < 0) {
                tree.Nodes[root].Children[id] = tree.Nodes.size();
                tree.Nodes.push_back(Node());    
            }
            root = tree.Nodes[root].Children[id];
        }
        if (tree.Nodes[root].TerminalNode < 0) {
            tree.Nodes[root].TerminalNode = tree.TerminalNodes.size();
            tree.TerminalNodes.push_back(vector<int>());
        }
        tree.TerminalNodes[tree.Nodes[root].TerminalNode].push_back(position);
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

public:
    Tree BuildTree(vector<string> genes) {
        Tree tree;
        for (int i = 0; i < genes.size(); ++i) {
            expandTree(tree, genes[i], i);
        }
        buildFailureFunction(tree);
        return move(tree);
    }
};

class DeterminingDnaHealth
{
    Tree DnaTree;
    vector<int> Weights;
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
        Weights.resize(n, 0);
        for (int i = 0; i < n; stream >> Weights[i++]);
    }

    Long findTotalHealthOfStrand(int left, int right, const string& dnaStrand) {
        vector<vector<int>> positions;
        for (int i = 0, state = 0; i < dnaStrand.size(); ++i) {
            int id = dnaStrand[i] - int('a');
            for (; state >= 0 && DnaTree.Nodes[state].Children[id] < 0;state = DnaTree.Nodes[state].LinkedNode);
            if (state < 0) {
                state = 0;
                continue;
            }
            state = DnaTree.Nodes[state].Children[id];
            for (int sufix_state = state; sufix_state > 0; sufix_state = DnaTree.Nodes[sufix_state].LinkedNode) {
                int idx = DnaTree.Nodes[sufix_state].TerminalNode;
                if ((idx >= 0) && (DnaTree.TerminalNodes[idx].front() <= right) && (DnaTree.TerminalNodes[idx].back()>=left))
                {
                    positions.push_back(DnaTree.TerminalNodes[idx]);
                }
            }
        }

        Long totalHealth = 0;
        for (int i = 0; i < positions.size(); ++i) {
           for (int j : positions[i]) {
               if ((j > right) || (j < left)) continue;
               totalHealth += (Long)Weights[j];
           }
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
    DeterminingDnaHealth determiningDnaHealth;
    determiningDnaHealth.FindSolution();
    return 0;
};
