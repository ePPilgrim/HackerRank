#include<iostream>
#include<fstream>
#include<map>
#include<vector>
#include<queue>
#include<algorithm>
#include<map>

const int MAX_NUMBER_OF_NODES = 2000000; 

struct Node{
    int NodePosition;
    std::pair<int,int> WordRange;
    std::map<char,int> Children; 
    Node(int nodePosition, int from, int to) : NodePosition(nodePosition), WordRange(from, to)
    {}
};

struct Tree{
    std::vector<Node> Nodes;
    std::vector<int> Permutation;
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
    std::queue<int> Queue;
    std::vector<NodeBuildInfo> TreeBuildInfo;

    void releaseResources(){
        Queue = std::move(std::queue<int>());
        TreeBuildInfo.clear();
        TreeBuildInfo.reserve(MAX_NUMBER_OF_NODES);
    }

    void touchChildrenWithPrefixPosition(Tree& tree, int state, int prefixState){
        prefixState = (prefixState < 0) ? 0 : prefixState;
        for(auto it = tree.Nodes[state].Children.begin(); it != tree.Nodes[state].Children.end(); Queue.push(it++->second)){
            tree.Nodes[it->second].NodePosition = prefixState;
        }
    }
    
protected:
    void buildPermutation(Tree& tree, std::vector<std::string>& genes)
    {
        tree.Permutation = std::vector<int>(genes.size());
        std::generate(tree.Permutation.begin(), tree.Permutation.end(), [n = 0]() mutable { return n++; });
        std::sort(tree.Permutation.begin(), tree.Permutation.end(), [&](int i, int j) {return genes[i] < genes[j];});
        std::vector<std::string> permutedGenes;
        permutedGenes.reserve(genes.size());
        for(auto it = tree.Permutation.begin(); it != tree.Permutation.end(); permutedGenes.push_back(genes[*it++]));
        genes = std::move(permutedGenes);
    }

    void buildTreeStructure(Tree& tree, const std::vector<std::string>& genes)
    {
        Queue.push(0);
        TreeBuildInfo.push_back(NodeBuildInfo(0, tree.StartSymbol));
        tree.Nodes.push_back(Node(0, 0, genes.size()));
          
        for(;!Queue.empty();Queue.pop()){
            int state = Queue.front();
            if(TreeBuildInfo[state].NodeSymbol == tree.TerminalSymbol) continue;
            int depth = TreeBuildInfo[state].Depth;
            for(int i = tree.Nodes[state].WordRange.first; i < tree.Nodes[state].WordRange.second; ++ i){
                char ch = (genes[i].length() == depth) ? tree.TerminalSymbol : genes[i][depth];
                auto it = tree.Nodes[state].Children.find(ch);
                if(it != tree.Nodes[state].Children.end()) tree.Nodes[it->second].WordRange.second ++;
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
        tree.Nodes[0].NodePosition = -1;
        touchChildrenWithPrefixPosition(tree, 0, 0);
        for(;!Queue.empty(); Queue.pop()){
            int state = Queue.front();
            int prefixState = tree.Nodes[state].NodePosition;
            for(;prefixState >= 0;prefixState = tree.Nodes[prefixState].NodePosition){
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
    Tree BuildTree(std::vector<std::string>& genes){
        Tree tree;
        this->buildPermutation(tree, genes);
        this->buildTreeStructure(tree, genes);
        this->buildFailureFunction(tree);
        this->releaseResources();
        return std::move(tree);
    }
};

class DeterminingDnaHealth
{
    Tree DnaTree;
    std::vector<int> Weights;

    std::istream* getFileStream(std::string filePath){
        std::ifstream* ptr_fstream = new std::ifstream();
        ptr_fstream->open(filePath);
        return ptr_fstream;
    }

    void initialize(std::istream& stream){
        int n;
        stream >> n;
        std::vector<std::string> genes(n);
        Weights.resize(n);
        for(int i = 0; i < n; stream >> genes[i++]);
        for(int i = 0; i < n; stream >> Weights[i++]);
        BfsTreeBuilder builder;
        DnaTree = builder.BuildTree(genes);
    }

public:
    DeterminingDnaHealth()
    {
        initialize(std::cin);
    }
    DeterminingDnaHealth(std::string filePath)
    {
        std::ifstream* ptr_fstream = new std::ifstream();
        ptr_fstream->open(filePath);
        initialize(*ptr_fstream);
    }
};

int main()
{
    DeterminingDnaHealth determiningDnaHealth("input.txt");
    return 0;
};