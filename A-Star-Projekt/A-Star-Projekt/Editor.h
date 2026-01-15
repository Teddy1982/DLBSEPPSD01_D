#pragma once

#include <SDL.h>
#include "InputHandler.h"
#include "TextureManager.h"
#include "Level.h"
#include "Buttons.h"
#include "Player.h"
#include "Enemy.h"

class CEditor {

public:

    ~CEditor();

    static CEditor* Instance()
    {
        if (s_pInstance == nullptr)
        {
            s_pInstance = new CEditor();
        }

        return s_pInstance;
    }

    static void Release()
    {
        if (s_pInstance != nullptr)
        {
            delete (s_pInstance);
            s_pInstance = nullptr;
        }
    }

    bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

    SDL_Window* getWindow() const { return m_pWindow; }
    SDL_Renderer* getRenderer() const { return m_pRenderer; }

    bool running() { return m_bRunning; }

    void handleEvents();
    void update();
    void render();

    void clean();

    void quit() { m_bRunning = false; }

    void changeUniqueValue(int xPos, int yPos, int value);

    static CEditor* s_pInstance;
    CLevel Level;

    int m_Player_xPos = 0;
    int m_Player_yPos = 0;
    int m_Enemy_xPos = 0;
    int m_Enemy_yPos = 0;

private:

    CEditor();

    SDL_Window* m_pWindow;
    SDL_Renderer* m_pRenderer;

    int m_editorWidth;
    int m_editorHeight;

    bool m_bRunning;

    CButton m_btHindernis;
    CButton m_btStartpunkt;
    CButton m_btZielpunkt;
    CButton m_btStarten;

    CPlayer m_Player;
    CEnemy m_Enemy;

    void drawLevel();
    void drawButtons();
};

typedef CEditor TheEditor;
