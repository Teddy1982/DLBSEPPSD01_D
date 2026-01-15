#include "Editor.h"

#include <fstream>

CEditor* CEditor::s_pInstance = nullptr;

CEditor::CEditor()
{

}

CEditor::~CEditor()
{

}

bool CEditor::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;

    // store the game width and height
    m_editorWidth = width;
    m_editorHeight = height;

    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    // attempt to initialise SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "SDL init success\n";
        // init the window
        m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

        if (m_pWindow != 0) // window init success
        {
            std::cout << "window creation success\n";
            m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);

            if (m_pRenderer != 0) // renderer init success
            {
                std::cout << "renderer creation success\n";
                SDL_SetRenderDrawColor(m_pRenderer, 255, 255, 255, 255);
            }
            else
            {
                std::cout << "renderer init fail\n";
                return false; // renderer init fail
            }
        }
        else
        {
            std::cout << "window init fail\n";
            return false; // window init fail
        }
    }
    else
    {
        std::cout << "SDL init fail\n";
        return false; // SDL init fail
    }

    TheTextureManager::Instance()->load("sprites/path_tiles.bmp", "path_tiles", getRenderer());
    TheTextureManager::Instance()->load("sprites/buttons.bmp", "buttons", getRenderer());
    TheTextureManager::Instance()->load("sprites/player.png", "player", getRenderer());
    TheTextureManager::Instance()->load("sprites/enemy.png", "enemy", getRenderer());

    m_btHindernis.init(10, 10, 10 + 192, 10 + 48);
    m_btStartpunkt.init(10, 60, 10 + 192, 60 + 48);
    m_btZielpunkt.init(10, 110, 10 + 192, 110 + 48);
    m_btStarten.init(10, 160, 10 + 192, 160 + 48);

    Level.init(210, 10, MAX_X * 48, MAX_Y * 48);

    m_bRunning = true;

    return true;

}

void CEditor::handleEvents()
{
    TheInputHandler::Instance()->update();
}

void CEditor::update()
{
    if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_ESCAPE))
        quit();

    if (Level.m_isStarten) {
        m_Player.update();
        m_Enemy.update();

        m_Player_xPos = m_Player.m_XPos;
        m_Player_yPos = m_Player.m_YPos;

        m_Enemy_xPos = m_Enemy.m_XPos;
        m_Enemy_yPos = m_Enemy.m_YPos;
    }

    if ((TheInputHandler::Instance()->getMouseButtonState(LEFT)) && (TheInputHandler::Instance()->getMousePosition()->getX() >= m_btHindernis.m_X) && (TheInputHandler::Instance()->getMousePosition()->getX() < m_btHindernis.m_Width + m_btHindernis.m_X) &&
        (TheInputHandler::Instance()->getMousePosition()->getY() >= m_btHindernis.m_Y) && (TheInputHandler::Instance()->getMousePosition()->getY() < m_btHindernis.m_Height + m_btHindernis.m_Y))
    {
        Level.m_isHindernis = true;
        Level.m_isComputer = false;
        Level.m_isSpieler = false;
        Level.m_isStarten = false;
    }

    if ((TheInputHandler::Instance()->getMouseButtonState(LEFT)) && (TheInputHandler::Instance()->getMousePosition()->getX() >= m_btStartpunkt.m_X) && (TheInputHandler::Instance()->getMousePosition()->getX() < m_btStartpunkt.m_Width + m_btStartpunkt.m_X) &&
        (TheInputHandler::Instance()->getMousePosition()->getY() >= m_btStartpunkt.m_Y) && (TheInputHandler::Instance()->getMousePosition()->getY() < m_btStartpunkt.m_Height + m_btStartpunkt.m_Y))
    {
        Level.m_isHindernis = false;
        Level.m_isComputer = true;
        Level.m_isSpieler = false;
        Level.m_isStarten = false;
    }

    if ((TheInputHandler::Instance()->getMouseButtonState(LEFT)) && (TheInputHandler::Instance()->getMousePosition()->getX() >= m_btZielpunkt.m_X) && (TheInputHandler::Instance()->getMousePosition()->getX() < m_btZielpunkt.m_Width + m_btZielpunkt.m_X) &&
        (TheInputHandler::Instance()->getMousePosition()->getY() >= m_btZielpunkt.m_Y) && (TheInputHandler::Instance()->getMousePosition()->getY() < m_btZielpunkt.m_Height + m_btZielpunkt.m_Y))
    {
        Level.m_isHindernis = false;
        Level.m_isComputer = false;
        Level.m_isSpieler = true;
        Level.m_isStarten = false;
    }

    if ((TheInputHandler::Instance()->getMouseButtonState(LEFT)) && (TheInputHandler::Instance()->getMousePosition()->getX() >= m_btStarten.m_X) && (TheInputHandler::Instance()->getMousePosition()->getX() < m_btStarten.m_Width + m_btStarten.m_X) &&
        (TheInputHandler::Instance()->getMousePosition()->getY() >= m_btStarten.m_Y) && (TheInputHandler::Instance()->getMousePosition()->getY() < m_btStarten.m_Height + m_btStarten.m_Y))
    {
        bool isPlayer = false;
        bool isEnemy = false;
        for (int i = 0; i < Level.m_Indices.size(); i++) {
            if (Level.m_Indices[i] == 2) {
                isEnemy = true;
            }
            if (Level.m_Indices[i] == 3) {
                isPlayer = true;
            }
        }
        if (isEnemy && isPlayer) {
            
            m_Player.init(m_Player_xPos, m_Player_yPos);
            m_Enemy.init(m_Enemy_xPos, m_Enemy_yPos);

            Level.m_isHindernis = false;
            Level.m_isComputer = false;
            Level.m_isSpieler = false;
            Level.m_isStarten = true;
        }
    }

    if ((TheInputHandler::Instance()->getMousePosition()->getX() >= Level.m_X) && (TheInputHandler::Instance()->getMousePosition()->getX() < Level.m_Width + Level.m_X) &&
        (TheInputHandler::Instance()->getMousePosition()->getY() >= Level.m_Y) && (TheInputHandler::Instance()->getMousePosition()->getY() < Level.m_Height + Level.m_Y))
    {
        if (!Level.m_isStarten) {
            int x = TheInputHandler::Instance()->getMousePosition()->getX();
            int y = TheInputHandler::Instance()->getMousePosition()->getY();

            Level.m_IndexX = (x - Level.m_X) / 48;
            Level.m_IndexY = (y - Level.m_Y) / 48;
        }
    }

    if ((TheInputHandler::Instance()->getMouseButtonState(LEFT)) && (TheInputHandler::Instance()->getMousePosition()->getX() >= Level.m_X) && (TheInputHandler::Instance()->getMousePosition()->getX() < Level.m_Width + Level.m_X) &&
        (TheInputHandler::Instance()->getMousePosition()->getY() >= Level.m_Y) && (TheInputHandler::Instance()->getMousePosition()->getY() < Level.m_Height + Level.m_Y))
    {     
        if (Level.m_isHindernis)
        {
            Level.m_Indices[(Level.m_IndexY) * MAX_X + Level.m_IndexX] = 1;
        }
        if (Level.m_isComputer)
        {
            for (int i = 0; i < Level.m_Indices.size(); i++) {
                if (Level.m_Indices[i] == 2) {
                    Level.m_Indices[i] = 0;
                }
            }

            Level.m_Indices[(Level.m_IndexY) * MAX_X + Level.m_IndexX] = 2;
            m_Enemy_xPos = Level.m_IndexX;
            m_Enemy_yPos = Level.m_IndexY;
        }
        if (Level.m_isSpieler)
        {
            for (int i = 0; i < Level.m_Indices.size(); i++) {
                if (Level.m_Indices[i] == 3) {
                    Level.m_Indices[i] = 0;
                }
            }

            Level.m_Indices[(Level.m_IndexY) * MAX_X + Level.m_IndexX] = 3;
            m_Player_xPos = Level.m_IndexX;
            m_Player_yPos = Level.m_IndexY;
        }
    }
    if ((TheInputHandler::Instance()->getMouseButtonState(RIGHT)) && (TheInputHandler::Instance()->getMousePosition()->getX() >= Level.m_X) && (TheInputHandler::Instance()->getMousePosition()->getX() < Level.m_Width + Level.m_X) &&
        (TheInputHandler::Instance()->getMousePosition()->getY() >= Level.m_Y) && (TheInputHandler::Instance()->getMousePosition()->getY() < Level.m_Height + Level.m_Y))
    {
        Level.m_Indices[(Level.m_IndexY) * MAX_X + Level.m_IndexX] = 0;
        
        TheInputHandler::Instance()->m_mouseButtonStates[RIGHT] = false;
    }

    TheInputHandler::Instance()->m_mouseButtonStates[LEFT] = false;
}

void CEditor::render()
{
    SDL_RenderClear(m_pRenderer);

    drawLevel();
    drawButtons();

    SDL_RenderPresent(m_pRenderer);
}

void CEditor::drawLevel()
{    
    for (int x = 0; x < MAX_X; x++)
    {
        for (int y = 0; y < MAX_Y; y++)
        {
            int index = Level.m_Indices[(y) * MAX_X + (x)];
            if (index == 0)
                continue;

            if (index == 1) {
                TheTextureManager::Instance()->drawFrame("path_tiles", 3 * 48, 0 * 48, x * 48 + 210, y * 48 + 10, 48, 48, getRenderer());
            }
            else if (!Level.m_isStarten && index == 2) {
                TheTextureManager::Instance()->drawFrame("enemy", 0 * 48, 1 * 48, x * 48 + 210, y * 48 + 10, 48, 48, getRenderer());
            }
            else if (!Level.m_isStarten && index == 3) {
                TheTextureManager::Instance()->drawFrame("player", 0 * 48, 1 * 48, x * 48 + 210, y * 48 + 10, 48, 48, getRenderer());
            }
        }
    }

    SDL_Rect rectangle;
    rectangle.x = Level.m_X;
    rectangle.y = Level.m_Y;
    rectangle.w = Level.m_Width;
    rectangle.h = Level.m_Height;

    SDL_SetRenderDrawColor(getRenderer(), 0, 0, 0, 255);
    SDL_RenderDrawRect(getRenderer(), &rectangle);
    SDL_SetRenderDrawColor(getRenderer(), 255, 255, 255, 255);

    if (Level.m_isStarten) {
        m_Enemy.render();
        m_Player.render();
    }

}

void CEditor::drawButtons()
{
    TheTextureManager::Instance()->drawFrame("buttons", 0, 0, 10, 10, 192, 48, getRenderer());
    TheTextureManager::Instance()->drawFrame("buttons", 0, 48, 10, 60, 192, 48, getRenderer());
    TheTextureManager::Instance()->drawFrame("buttons", 0, 96, 10, 110, 192, 48, getRenderer());
    TheTextureManager::Instance()->drawFrame("buttons", 0, 144, 10, 160, 192, 48, getRenderer());
}

void CEditor::changeUniqueValue(int xPos, int yPos, int value)
{
    for (int x = 0; x < MAX_X; x++)
    {
        for (int y = 0; y < MAX_Y; y++)
        {
            if (Level.m_Indices[y * MAX_X + x] == value)
            {
                Level.m_Indices[y * MAX_X + x] = 0;
            }
        }
    }

    Level.m_Indices[yPos * MAX_X + xPos] = value;
}

void CEditor::clean()
{
    std::cout << "cleaning editor\n";

    TheTextureManager::Release();
    TheInputHandler::Release();

    SDL_DestroyRenderer(m_pRenderer);
    SDL_DestroyWindow(m_pWindow);

    m_pRenderer = nullptr;
    m_pWindow = nullptr;

    SDL_Quit();
}
