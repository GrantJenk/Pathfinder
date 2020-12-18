#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>

#include "types.h"

class Pathfinder
{
public:
    Pathfinder(int cols, int rows, bool useDiagonals);
    ~Pathfinder();

<<<<<<< Updated upstream
    std::vector<Node*> aStar(Location start, Location dest);
    std::vector<Node*> aStar(Node* start, Node* dest);

    float heuristic(Node* a, Node* b);
=======
    void aStar(const Location& start, const Location& dest);

    void toggleWall(const Location& loc);
>>>>>>> Stashed changes

    void randomizeWalls(int percentChance);

    void resetPath();

    void emptyGrid();

<<<<<<< Updated upstream
    Node* getGrid() const;

=======
>>>>>>> Stashed changes
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
