#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <memory>
#include <map>
#include <tuple>
#include <sstream>
#include <unordered_map>
struct Node
{
    std::string id;
    std::vector<std::shared_ptr<Node>> children;
};


// Define a key for our cache: {Node ID, hitDAC, hitFFT}
typedef std::tuple<std::string, bool, bool> State;
/*
*
* Read tree structure from file
aaa: you hhh
you: bbb ccc
bbb: ddd eee
ccc: ddd eee fff
ddd: ggg'
"you"  id will always be starting node to seatrch from so we return it explicitly as parameter reference
*/
// Helper to get an existing node or create a new one
std::shared_ptr<Node> getOrCreate(std::string id, std::unordered_map<std::string, std::shared_ptr<Node>>& map) {
    if (map.find(id) == map.end()) {
        map[id] = std::make_shared<Node>(Node{id, {}});
    }
    return map[id];
}

void readFromFile(std::ifstream& inputFile, std::shared_ptr<Node>& rootP1, std::shared_ptr<Node>& rootP2) {
    std::string line;
    std::unordered_map<std::string, std::shared_ptr<Node>> nodesMap;

    while (std::getline(inputFile, line)) {
        // 1. Clean the line: replace ':' with ' ' to make it a space-separated list
        size_t colon = line.find(':');
        if (colon == std::string::npos) continue;
        line[colon] = ' ';

        // 2. Use stringstream to extract IDs
        std::stringstream ss(line);
        std::string parentId, childId;
        
        ss >> parentId;
        auto parentNode = getOrCreate(parentId, nodesMap);

        // Set entry points
        if (parentId == "you" && !rootP1) rootP1 = parentNode;
        if (parentId == "svr" && !rootP2) rootP2 = parentNode;

        // 3. Add all remaining IDs on the line as children
        while (ss >> childId) {
            parentNode->children.push_back(getOrCreate(childId, nodesMap));
        }
    }
}

unsigned long findNumOfPathsToOut(std::shared_ptr<Node> node, bool hitDAC, bool hitFFT, std::map<State, unsigned long>& memo)
{
    if (node->id == "out")
    {
        return (hitDAC && hitFFT) ? 1 : 0;
    }
    if (node->children.size() == 0)
    {
        return 0;
    }

    // Memoization check
    hitDAC = hitDAC || node->id == "dac";
    hitFFT = hitFFT || node->id == "fft";
    State currentState = {node->id, hitDAC, hitFFT};
    if (memo.count(currentState))
    {
        return memo[currentState];
    }

    unsigned long sumPaths = 0;
    for (const auto &child : node->children)
    {
        sumPaths += findNumOfPathsToOut(child, hitDAC, hitFFT, memo);
    }
    memo[currentState] = sumPaths;
    return sumPaths;
}

int main(int argc, char **argv)
{
    std::ifstream inputFile("input");

    std::shared_ptr<Node> rootP1 = nullptr;
    std::shared_ptr<Node> rootP2 = nullptr;
    readFromFile(inputFile, rootP1, rootP2);

    std::map<State, unsigned long> memo1;
    int numPaths = findNumOfPathsToOut(rootP1,true,true,memo1);
    std::cout << "Number of paths from 'you' to 'out': " << numPaths << "\n";
    
    std::map<State, unsigned long> memo;
    unsigned long numPathsP2 = findNumOfPathsToOut(rootP2, false, false,memo1);
    std::cout << "Number of paths from 'svr' to 'out' via 'dac' and 'fft': " << numPathsP2 << "\n";
    
    return 0;
}