#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>

#include "types.h"

class Pathfinder
{
public:
    Pathfinder(int cols, int rows, bool useDiagonals);
    ~Pathfinder();

    std::vector<Node*> aStar(Location start, Location dest);
    std::vector<Node*> aStar(Node* start, Node* dest);

    float heuristic(Node* a, Node* b);

    void randomizeWalls(int percentChance);

    void resetPath();

    void emptyGrid();

    Node* getGrid() const;

    int getCols() const;
    int getRows() const;

private:
    int m_cols;
    int m_rows;
    bool m_useDiagonals;
    Node* m_grid;
};

#endif // PATHFINDER_H
