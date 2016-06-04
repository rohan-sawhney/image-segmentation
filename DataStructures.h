#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <iostream>
#include <math.h>
#include <vector>

struct Edge {
    int a, b;
    double weight;
};

struct Node {
    int rank, index, size;
};

class Forest {
public:
    // constructor
    Forest(const int& count);
    
    // find
    int find(int i);
    
    // join
    void join(int i, int j);
    
    // return node size
    int nodeSize(int i) const;
    
    // num sets
    int numSets() const;
    
private:
    // member variables
    std::vector<Node> nodes;
    int sets;
};

#endif
