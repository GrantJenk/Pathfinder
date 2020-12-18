#ifndef TYPES_H
#define TYPES_H

#include <cmath>

struct Location
{
    int col;
    int row;
};

struct Node
{
    Location loc;
    bool isWall = false;
    bool visited = false;
    bool inPath = false;
    float G = INFINITY;
    float F = INFINITY;
    Node* parent = nullptr;
};

#endif // TYPES_H
