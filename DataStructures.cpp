#include "DataStructures.h"

Forest::Forest(const int& count)
{
    nodes.reserve(count);
    sets = count;
    for (int i = 0; i < sets; i++) {
        nodes.push_back(Node());
        nodes[i].rank = 0;
        nodes[i].index = i;
        nodes[i].size = 1;
    }
}

int Forest::find(int i)
{
    int j = i;
    while (j != nodes[j].index) j = nodes[j].index;
    nodes[i].index = j;
    
    return j;
}

void Forest::join(int i, int j)
{
    if (nodes[i].rank > nodes[j].rank) {
        nodes[j].index = i;
        nodes[i].size += nodes[j].size;
        
    } else {
        nodes[i].index = j;
        nodes[j].size += nodes[i].size;
        
        if (nodes[i].rank == nodes[j].rank) nodes[j].rank++;
    }
    
    sets--;
}

int Forest::nodeSize(int i) const
{
    return nodes[i].size;
}

int Forest::numSets() const
{
    return sets;
}