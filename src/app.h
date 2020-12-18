#ifndef APP_H
#define APP_H

#include <SDL2/SDL.h>

#include "pathfinder.h"
#include "types.h"

class App
{
public:
    App(int numOfCols, int numOfRows, bool useDiagonals);
    ~App();

    void run();

    void handleInput();

    void draw();

private:
    SDL_Color getNodeColor(const Location& loc);

private:
    int m_SQDIM;
    bool m_running;
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    bool m_isDraggingWall;
    bool m_dragStartNode;
    bool m_dragDestNode;
    const Node* m_prevDragWallToggle;
    const Node* m_startNode;
    const Node* m_destNode;
    Pathfinder m_pathfinder;
};

#endif // APP_H
