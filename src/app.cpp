#include "app.h"

App::App(int numOfCols, int numOfRows, bool useDiagonals) : m_pathfinder(numOfCols, numOfRows, useDiagonals)
{
    m_SQDIM = 800/numOfCols;
    m_running = true;
    m_window = nullptr;
    m_renderer = nullptr;
    m_isDraggingWall = false;
    m_dragStartNode = false;
    m_dragDestNode = false;
    m_prevDragWallToggle = nullptr;
    m_startNode = nullptr;
    m_destNode = nullptr;

    SDL_Init(SDL_INIT_VIDEO);
    m_window = SDL_CreateWindow("A* Pathfinder", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, SDL_WINDOW_RESIZABLE);
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

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
        switch (e.type)
        {
        case SDL_QUIT:
            m_running = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
        {
            int col = e.button.x/m_SQDIM;
            int row = e.button.y/m_SQDIM;
            const Node* clickedNode = &m_pathfinder.getNode({col, row});

            if (e.button.button == SDL_BUTTON_LEFT)
            {
                if (clickedNode->isWall)
                    return;

                if (clickedNode == m_startNode && m_destNode != nullptr)
                {
                    m_dragStartNode = true;
                }
                else if (clickedNode == m_destNode)
                {
                    m_dragDestNode = true;
                }
                else if (m_startNode == nullptr)
                {
                    m_startNode = clickedNode;
                }
                else if (m_destNode == nullptr)
                {
                    m_destNode = clickedNode;
                    m_pathfinder.aStar(m_startNode->loc, m_destNode->loc);
                }
                else if (m_startNode != nullptr && m_destNode != nullptr)
                {
                    m_pathfinder.resetPath();
                    m_startNode = clickedNode;
                    m_destNode = nullptr;
                }
            }
            else if (e.button.button == SDL_BUTTON_RIGHT)
            {
                m_isDraggingWall = true;
                if (m_destNode) {
                    m_pathfinder.resetPath();
                }
                m_startNode = nullptr;
                m_destNode = nullptr;
                m_prevDragWallToggle = clickedNode;
                m_pathfinder.toggleWall(clickedNode->loc);
            }
            break;
        }

        case SDL_MOUSEBUTTONUP:
            m_isDraggingWall = false;
            m_dragStartNode = false;
            m_dragDestNode = false;
            m_prevDragWallToggle = nullptr;
            break;

        case SDL_MOUSEMOTION:
            int col = e.motion.x/m_SQDIM;
            int row = e.motion.y/m_SQDIM;
            const Node* hoverNode = &m_pathfinder.getNode({col, row});

            if (m_dragStartNode || m_dragDestNode)
            {
                if (!hoverNode->isWall && hoverNode != m_startNode && hoverNode != m_destNode)
                {
                    if (m_dragDestNode) { m_destNode = hoverNode; };
                    if (m_dragStartNode) { m_startNode = hoverNode; };
                    m_pathfinder.resetPath();
                    m_pathfinder.aStar(m_startNode->loc, m_destNode->loc);
                }
            }
            else if (m_isDraggingWall && m_prevDragWallToggle != hoverNode)
            {
                m_prevDragWallToggle = hoverNode;
                m_pathfinder.toggleWall(hoverNode->loc);
            }
            break;
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
            const SDL_Color& color = getNodeColor({col, row});

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

SDL_Color App::getNodeColor(const Location& loc)
{
    const Node& node = m_pathfinder.getNode(loc);

    if (&node == m_startNode)
        return {0, 255, 0, 255};
    else if (&node == m_destNode)
        return {255, 0, 0, 255};

    if (node.inPath)
        return {0, 200, 255, 255};
    else if (node.isWall)
        return {0, 0, 0, 255};
    else if (node.visited)
        return {200, 200, 255, 255};

    return {255, 255, 255, 255};
}
