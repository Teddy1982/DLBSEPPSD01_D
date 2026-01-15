#pragma once
#pragma once

#include "SDL.h"
#include "Editor.h"
#include "Vector2D.h"

#include <vector>

enum mouse_buttons
{
    LEFT = 0,
    MIDDLE = 1,
    RIGHT = 2
};

class CInputHandler {

public:

    ~CInputHandler();

    static CInputHandler* Instance()
    {
        if (s_pInstance == nullptr)
        {
            s_pInstance = new CInputHandler();
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

    void update();

    void reset();

    bool isKeyDown(SDL_Scancode key) const;

    bool getMouseButtonState(int buttonNumber) const {
        return m_mouseButtonStates[buttonNumber];
    }
    CVector2D* getMousePosition() const {
        return m_mousePosition;
    }

    static CInputHandler* s_pInstance;

    std::vector<bool> m_mouseButtonStates;

private:

    CInputHandler();

    const Uint8* m_keystates;

    void onKeyDown();
    void onKeyUp();

    void onMouseMove(SDL_Event& event);
    void onMouseButtonDown(SDL_Event& event);
    void onMouseButtonUp(SDL_Event& event);

    CVector2D* m_mousePosition;

};

typedef CInputHandler TheInputHandler;
