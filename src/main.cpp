#include "app.h"

int main(int argc, char* argv[])
{
    int cols = 50;
    int rows = 50;
    bool useDiagonals = false;

    App app(cols, rows, useDiagonals);
    app.run();

    return 0;
}
