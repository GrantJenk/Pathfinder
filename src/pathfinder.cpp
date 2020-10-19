#include "pathfinder.h"

#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <time.h>

Pathfinder::Pathfinder(int cols, int rows, bool useDiagonals) : m_cols(cols), m_rows(rows), m_grid(nullptr), m_useDiagonals(useDiagonals)
{
    emptyGrid();
}

Pathfinder::~Pathfinder()
{
    delete[] m_grid;
    m_grid = nullptr;
}

std::vector<Node*> Pathfinder::aStar(Location start, Location dest)
{
    std::vector<Node*> result;
    Node* startNode = &m_grid[start.row * m_cols + start.col];
    Node* destNode = &m_grid[dest.row * m_cols + dest.col];
    std::vector<Node*> openSet;

    startNode->G = 0.0f;
    startNode->F = heuristic(startNode, destNode);
    openSet.push_back(startNode);

    while( !openSet.empty() )
    {
        Node* curNode = ( *std::min_element(openSet.begin(), openSet.end(), [](Node* a, Node* b) { return a->F < b->F; }) );
        if (curNode == destNode)
        {
            Node* backtrack = curNode;
            while (backtrack != nullptr)
            {
                result.push_back(backtrack);
                backtrack = backtrack->parent;
            }
            m_path = result;
            return result;
        }

        auto curNodePos = std::find(openSet.begin(), openSet.end(), curNode);
        openSet.erase(curNodePos);

        std::vector<Node*> neighbors;
        int row = curNode->row;
        int col = curNode->col;
        if (col > 0)
            neighbors.push_back(&m_grid[row * m_cols + (col - 1)]);
        if (row > 0)
            neighbors.push_back(&m_grid[(row - 1) * m_cols + col]);
        if (row < m_rows - 1)
            neighbors.push_back(&m_grid[(row + 1) * m_cols + col]);
        if (col < m_cols - 1)
            neighbors.push_back(&m_grid[row * m_cols + (col + 1)]);

        if (m_useDiagonals)
        {
            if (col > 0 && row > 0)
                neighbors.push_back(&m_grid[(row - 1) * m_cols + (col - 1)]);
            if (row < m_rows - 1 && col > 0)
                neighbors.push_back(&m_grid[(row + 1) * m_cols + (col - 1)]);
            if (row > 0 && col < m_cols - 1)
                neighbors.push_back(&m_grid[(row - 1) * m_cols + (col + 1)]);
            if (row < m_rows - 1 && col < m_cols - 1)
                neighbors.push_back(&m_grid[(row + 1) * m_cols + (col + 1)]);
        }

        curNode->visited = true;

        for (auto neighbor: neighbors)
        {
            float tentativeG = curNode->G + heuristic(curNode, neighbor); // 1 distance when no diags
            if (tentativeG < neighbor->G)
            {
                neighbor->parent = curNode;
                neighbor->G = tentativeG;
                neighbor->F = neighbor->G + heuristic(neighbor, destNode);
                if ( !(std::find(openSet.begin(), openSet.end(), neighbor) != openSet.end()) &&
                    !neighbor->isWall &&
                    !neighbor->visited)
                {
                    openSet.push_back(neighbor);
                }
            }
        }
    }

    m_path = result;
    return result;
}

std::vector<Node*> Pathfinder::aStar(Node* start, Node* dest)
{
    Location startLoc = {start->col, start->row};
    Location destLoc = {dest->col, dest->row};
    return aStar(startLoc, destLoc);
}

float Pathfinder::heuristic(Node* a, Node* b)
{
    return sqrt( pow(abs(a->col - b->col), 2) + pow(abs(a->row - b->row), 2) );
}

void Pathfinder::randomizeWalls(int percentChance)
{
    srand(time(0));
    for (int i = 0; i < m_cols*m_rows; i++)
    {
        if ((rand() % 100) < percentChance) {
            m_grid[i].isWall = true;
        }
    }
}

void Pathfinder::resetPath()
{
    for (int row = 0; row < m_rows; row++)
    {
        for (int col = 0; col < m_cols; col++)
        {
            Node* cur = &m_grid[row * m_cols + col];
            cur->visited = false;
            cur->F = INFINITY;
            cur->G = INFINITY;
            cur->parent = nullptr;
        }
    }
    m_path.clear();
}

void Pathfinder::emptyGrid()
{
    delete[] m_grid;
    m_grid = new Node[m_cols * m_rows];

    for (int c = 0; c < m_cols; c++)
    {
        for (int r = 0; r < m_rows; r++)
        {
            m_grid[r * m_cols + c] = {c, r};
        }
    }
}

Node* Pathfinder::getGrid() const
{
    return m_grid;
}

std::vector<Node*> Pathfinder::getPath() const
{
    return m_path;
}

int Pathfinder::getCols() const
{
    return m_cols;
}

int Pathfinder::getRows() const
{
    return m_rows;
}
