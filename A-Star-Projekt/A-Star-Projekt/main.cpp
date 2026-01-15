#include <iostream>

#include "Editor.h"

const int FPS = 60;
const int DELAY_TIME = 1000.0f / FPS;

int main(int argc, char* argv[])
{
    Uint32 frameStart, frameTime;

    if (TheEditor::Instance()->init("A-Star-Pathfinding", 0, 35, 1920, 1080, false))
    {
        while (TheEditor::Instance()->running())
        {
            frameStart = SDL_GetTicks();

            TheEditor::Instance()->handleEvents();
            TheEditor::Instance()->update();
            TheEditor::Instance()->render();
            frameTime = SDL_GetTicks() - frameStart;

            if (frameTime < DELAY_TIME)
            {
                SDL_Delay((int)(DELAY_TIME - frameTime));
            }
        }
    }
    else
    {
        std::cout << "editor init failure - " << SDL_GetError() << "\n";
        return -1;
    }

    TheEditor::Instance()->clean();
    TheEditor::Release();

    std::cout << "editor closing...\n";

    return 0;
}
