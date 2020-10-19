#include "app.h"

App::App(int numOfCols, int numOfRows, bool useDiagonals) :
    m_running(true), m_window(nullptr), m_renderer(nullptr), m_pathfinder(numOfCols, numOfRows, useDiagonals), m_beginNode(nullptr), m_endNode(nullptr)
{
    SDL_Init(SDL_INIT_VIDEO);
    m_window = SDL_CreateWindow("A* Pathfinder", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_RESIZABLE);
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    m_SQDIM = 800/numOfCols;
    m_pathfinder.randomizeWalls(30);
}

App::~App()
{
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void App::run()
{
    while(m_running)
    {
        handleInput();
        draw();
    }
}

void App::handleInput()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            m_running = false;
            return;
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            int col = e.button.x/m_SQDIM;
            int row = e.button.y/m_SQDIM;
            int index = row * m_pathfinder.getCols() + col;
            Node* grid = m_pathfinder.getGrid();
            Node* clickedNode = &grid[index];

            if (e.button.button == SDL_BUTTON_LEFT)
            {
                if (grid[index].isWall)
                    return;

                if (m_beginNode == nullptr)
                {
                    m_beginNode = clickedNode;
                }
                else if (m_endNode == nullptr)
                {
                    m_endNode = clickedNode;
                    m_pathfinder.aStar(m_beginNode, m_endNode);
                }
                else if (m_beginNode != nullptr && m_endNode != nullptr)
                {
                    m_pathfinder.resetPath();
                    m_beginNode = clickedNode;
                    m_endNode = nullptr;
                }
            }
            else if (e.button.button == SDL_BUTTON_RIGHT)
            {
                if (m_endNode) {
                    m_pathfinder.resetPath();
                }
                m_beginNode = nullptr;
                m_endNode = nullptr;
                clickedNode->isWall = !clickedNode->isWall;
            }
        }
    }
}

void App::draw()
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    for (int row = 0; row < m_pathfinder.getRows(); row++)
    {
        for (int col = 0; col < m_pathfinder.getCols(); col++)
        {
            Node* curNode = &m_pathfinder.getGrid()[row * m_pathfinder.getCols() + col];
            SDL_Color color = getNodeColor(curNode);

            if ( !m_pathfinder.getPath().empty() ) {
                for (auto el: m_pathfinder.getPath())
                {
                    if (el == curNode)
                    {
                        color = {0, 255, 0, 255};
                    }
                }
            }

            SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);

            SDL_Rect rect;
            rect.h = rect.w = m_SQDIM;
            rect.x = col * m_SQDIM;
            rect.y = row * m_SQDIM;
            SDL_RenderFillRect(m_renderer, &rect);
        }
    }

    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    for (int i = 0; i < m_pathfinder.getCols(); i++)
    {
        SDL_RenderDrawLine(m_renderer, i*m_SQDIM, 0, i*m_SQDIM, 800);
        SDL_RenderDrawLine(m_renderer, 0, i*m_SQDIM, 800, i*m_SQDIM);
    }

    SDL_RenderPresent(m_renderer);
}

SDL_Color App::getNodeColor(Node* node)
{
    if (node == m_beginNode || node == m_endNode)
        return {255, 125, 0, 255};
    if (node->isWall)
        return {0, 0, 0, 255};
    if (node->visited)
        return {128, 150, 255, 255};

    return {255, 255, 255, 255};
}
