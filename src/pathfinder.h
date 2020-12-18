#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>

#include "types.h"

class Pathfinder
{
public:
    Pathfinder(int cols, int rows, bool useDiagonals);
    ~Pathfinder();

    void aStar(const Location& start, const Location& dest);

    void toggleWall(const Location& loc);

    void randomizeWalls(int percentChance);

    void resetPath();

    void emptyGrid();

    int getCols() const;
    int getRows() const;
    const Node& getNode(const Location& loc) const;

private:
    float heuristic(const Node& a, const Node& b) const;

private:
    int m_cols;
    int m_rows;
    bool m_useDiagonals;
    std::vector<Node> m_grid;
};

#endif // PATHFINDER_H
