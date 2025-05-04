#ifndef NODE_H
#define NODE_H

enum NodeType
{
    WATER,
    GROUND,
    SOURCE
};

struct Node
{
    enum NodeType type;
    float concentration;
};

#endif
