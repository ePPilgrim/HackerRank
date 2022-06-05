#include<iostream>
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
    {} 
};

class BfsTreeBuilder{
    struct NodeBuildInfo{
        int Depth;
        int ParentState; 
        char NodeSymbol;
        NodeBuildInfo( int depth, int parentState, char nodeSymbol) : Depth(depth), ParentState(parentState), NodeSymbol(nodeSymbol)
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
        TreeBuildInfo.push_back(NodeBuildInfo(0, 0, tree.StartSymbol));
        tree.Nodes.push_back(Node(0, 0, genes.size()));
          
        for(;!Queue.empty();Queue.pop()){
            int state = Queue.front();
            if(TreeBuildInfo[state].NodeSymbol == tree.TerminalSymbol) continue;
            Node& node = tree.Nodes[state];
            int depth = TreeBuildInfo[state].Depth;
            for(int i = node.WordRange.first; i < node.WordRange.second; ++ i){
                char ch = (genes[i].length() == depth + 1) ? tree.TerminalSymbol : genes[i][depth];
                auto it = node.Children.find(ch);
                if(it != node.Children.end()) tree.Nodes[it->second].WordRange.second ++;
                else{
                    Queue.push(tree.Nodes.size());
                    TreeBuildInfo.push_back(NodeBuildInfo(depth + 1, state, ch));
                    node.Children[ch] = tree.Nodes.size();
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



// class DeterminingDnaHealth
// {
//     std::vector<Node> mTree;
//     std::vector<int> mWeights;
//     void buildTree(std::vector<string> genes){
//         std::sort(genes.begin(), genes.end());
//         std::queue<Node> queue;
//         queue.push(new Node())

        


//     }
//     void buildFailureFunction();

// public:
//     DeterminingDnaHealth()
//     {}
// };

int main()
{

   // auto ff = new DeterminingDnaHealth();
    return 0;
};