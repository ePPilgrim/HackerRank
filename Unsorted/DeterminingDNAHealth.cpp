#include <bits/stdc++.h>

using namespace std;
typedef unsigned long long Long;
const int MAX_NUMBER_OF_NODES = 4000000; 

struct Node{
    int LinkedNode;
    pair<int,int> Span;
    map<char,int> Children; 
    Node(int linkedNode, int fromIndex, int toIndex) : LinkedNode(linkedNode), Span(fromIndex, toIndex)
    {}
};

struct Tree{
    vector<Node> Nodes;
    vector<int> Permutation;
    char StartSymbol;
    char TerminalSymbol;
    Tree() : StartSymbol('{'), TerminalSymbol('}')
    {
        Nodes.reserve(MAX_NUMBER_OF_NODES);
    } 
};

class BfsTreeBuilder{
    struct NodeBuildInfo{
        int Depth;
        char NodeSymbol;
        NodeBuildInfo( int depth, char nodeSymbol) : Depth(depth), NodeSymbol(nodeSymbol)
        {}
    };
    queue<int> Queue;
    vector<NodeBuildInfo> TreeBuildInfo;

    void releaseResources(){
        Queue = move(queue<int>());
        TreeBuildInfo.clear();
        TreeBuildInfo.reserve(MAX_NUMBER_OF_NODES);
    }

    void touchChildrenWithPrefixPosition(Tree& tree, int state, int prefixState){
        prefixState = (prefixState < 0) ? 0 : prefixState;
        for(auto it = tree.Nodes[state].Children.begin(); it != tree.Nodes[state].Children.end(); Queue.push(it++->second)){
            tree.Nodes[it->second].LinkedNode = prefixState;
        }
    }
    
protected:
    void buildPermutation(Tree& tree, vector<string>& genes)
    {
        vector<int> permutation(genes.size());
        generate(permutation.begin(), permutation.end(), [n = 0]() mutable { return n++; });
        sort(permutation.begin(), permutation.end(), [&](int i, int j) {return genes[i] < genes[j];});
        vector<string> permutedGenes;
        permutedGenes.reserve(genes.size());
        tree.Permutation.resize(genes.size());
        for(int i = 0; i < permutation.size(); ++ i){
            permutedGenes.push_back(genes[permutation[i]]);
            tree.Permutation[permutation[i]] = i;
        }
        genes = move(permutedGenes);
    }

    void buildTreeStructure(Tree& tree, const vector<string>& genes)
    {
        Queue.push(0);
        TreeBuildInfo.push_back(NodeBuildInfo(0, tree.StartSymbol));
        tree.Nodes.push_back(Node(0, 0, genes.size()));  
        for(;!Queue.empty();Queue.pop()){
            int state = Queue.front();
            if(TreeBuildInfo[state].NodeSymbol == tree.TerminalSymbol) continue;
            int depth = TreeBuildInfo[state].Depth;
            for(int i = tree.Nodes[state].Span.first; i < tree.Nodes[state].Span.second; ++ i){
                char ch = (genes[i].length() == depth) ? tree.TerminalSymbol : genes[i][depth];
                auto it = tree.Nodes[state].Children.find(ch);
                if(it != tree.Nodes[state].Children.end()) tree.Nodes[it->second].Span.second ++;
                else{
                    Queue.push(tree.Nodes.size());
                    TreeBuildInfo.push_back(NodeBuildInfo(depth + 1, ch));
                    tree.Nodes[state].Children[ch] = tree.Nodes.size();
                    tree.Nodes.push_back(Node(tree.Nodes.size(), i, i + 1));        
                }    
            }    
        }
    } 

     void buildFailureFunction(Tree& tree){
        tree.Nodes[0].LinkedNode = -1;
        touchChildrenWithPrefixPosition(tree, 0, 0);
        for(;!Queue.empty(); Queue.pop()){
            int state = Queue.front();
            int prefixState = tree.Nodes[state].LinkedNode;
            for(;prefixState >= 0;prefixState = tree.Nodes[prefixState].LinkedNode){
                auto it = tree.Nodes[prefixState].Children.find(TreeBuildInfo[state].NodeSymbol);
                if(it != tree.Nodes[prefixState].Children.end()){
                    prefixState = it->second;
                    break;
                }  
            }
            touchChildrenWithPrefixPosition(tree, state, prefixState);
        }
    } 
public:
    Tree BuildTree(vector<string>& genes){
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
    vector<int> Weights;
    istream* PtrToStream;

    istream* getStream(string filePath=""){
        PtrToStream = nullptr;
        if(filePath.empty()){
            PtrToStream = &cin;
        }else{
            ifstream* ptr_fstream = new ifstream();
            ptr_fstream->open(filePath);
            PtrToStream = ptr_fstream;
        }
        return PtrToStream;
    }

    void initialize(istream& stream){
        int n;
        stream >> n;
        vector<string> genes(n);
        for(int i = 0; i < n; stream >> genes[i++]);
        BfsTreeBuilder builder;
        DnaTree = builder.BuildTree(genes);
        Weights.resize(n);
        for(int i = 0; i < n; stream >> Weights[DnaTree.Permutation[i++]]);
    }

    bool isStrandHealthy(const Node& node, const set<int>& validSet){
        auto it = validSet.lower_bound(node.Span.first);
        if(it == validSet.end()) return false;
        if(*it < node.Span.second) return true;
        return false;
    }

    Long calculateTotalHealth(const Node& node, const set<int>& validSet){
        if(!isStrandHealthy(node, validSet)) return 0;
        Long totalHealth = 0;
        for(auto it = validSet.lower_bound(node.Span.first); it != validSet.lower_bound(node.Span.second); totalHealth += (Long) Weights[*it++]);
        return totalHealth;
    }

    Long findTotalHealthOfStrand(int left, int right, const string& dnaStrand){
        Long totalHealth = 0;
        vector<int> validList(right-left+1);
        generate(validList.begin(), validList.end(), [&,i = left]() mutable { return DnaTree.Permutation[i++]; });
        set<int> validSet(validList.begin(), validList.end());
        for(int i = 0, state = 0; i < dnaStrand.length(); ++ i){
            Node& node = DnaTree.Nodes[(state < 0) ? 0 : state];
            if(isStrandHealthy(node, validSet)){
                auto it = node.Children.find(DnaTree.TerminalSymbol);
                if (it != node.Children.end()) totalHealth += calculateTotalHealth(DnaTree.Nodes[it->second],validSet);
                it = node.Children.find(dnaStrand[i]);
                if(it != node.Children.end()){
                    state = it->second;
                    continue;
                }
            }
            state = node.LinkedNode;
            i += (state < 0) ? 0 : -1;
        }
        return totalHealth;
    }

public:
    DeterminingDnaHealth(string filePath = "")
    {
        initialize(*getStream(filePath));
    }

    void FindSolution(){

        int n, l,r;
        *PtrToStream>>n;
        for(int i = 0; i < n; ++ i){
            string dnaStrand;
            *PtrToStream>>l>>r>>dnaStrand;
            Long res = findTotalHealthOfStrand(l, r, dnaStrand + DnaTree.TerminalSymbol);
            if(res > maxHealth) maxHealth = res;
            if(res < minHealth) minHealth = res;
            cout << res << endl;
        }
        cout << minHealth <<' '<< maxHealth << endl;
    }
};

int main()
{
    DeterminingDnaHealth determiningDnaHealth("input.txt");
    determiningDnaHealth.FindSolution();
    //0 1970060
    return 0;
};