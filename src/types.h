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
    int col;
    int row;
    bool isWall = false;
    bool visited = false;
    float G = INFINITY;
    float F = INFINITY;
    Node* parent = nullptr;
};

#endif // TYPES_H
