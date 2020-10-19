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
    void drawPath();
    SDL_Color getNodeColor(Node* node);

private:
    bool m_running;
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    bool m_dragStartNode;
    bool m_dragEndNode;
    int m_SQDIM;
    Node* m_beginNode;
    Node* m_endNode;
    Pathfinder m_pathfinder;
};

#endif // APP_H

