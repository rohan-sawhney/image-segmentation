#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include <iostream>
#include <math.h>
#include <vector>

struct Edge {
    Edge(const int& a0, const int& b0, const double& weight0): a(a0), b(b0), weight(weight0) {};
    int a, b;
    double weight;
};

struct Node {
    Node(const int& rank0, const int& index0, const int& size0): rank(rank0), index(index0), size(size0) {};
    int rank, index, size;
};

class Forest {
public:
    // constructor
    Forest(const int& count);
    
    // find
    int find(const int& i);
    
    // join
    void join(const int& i, const int& j);
    
    // return node size
    int nodeSize(const int& i) const;
    
    // num sets
    int numSets() const;
    
private:
    // member variables
    std::vector<Node> nodes;
    int sets;
};

#endif
