#include "InputHandler.h"

CInputHandler* CInputHandler::s_pInstance = nullptr;

CInputHandler::CInputHandler() : m_keystates(0),
m_mousePosition(new CVector2D(0, 0))
{
    // create button states for the mouse
    for (int i = 0; i < 3; i++)
    {
        m_mouseButtonStates.push_back(false);
    }
}

CInputHandler::~CInputHandler()
{
    delete m_mousePosition;

    m_mouseButtonStates.clear();
}

void CInputHandler::reset()
{
    m_mouseButtonStates[LEFT] = false;
    m_mouseButtonStates[RIGHT] = false;
    m_mouseButtonStates[MIDDLE] = false;
}

void CInputHandler::update()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            TheEditor::Instance()->quit();
            break;

        case SDL_KEYDOWN:
            onKeyDown();
            break;

        case SDL_KEYUP:
            onKeyUp();
            break;
        case SDL_MOUSEBUTTONDOWN:
            onMouseButtonDown(event);
            break;
        case SDL_MOUSEBUTTONUP:
            onMouseButtonUp(event);
            break;
        case SDL_MOUSEMOTION:
            onMouseMove(event);
            break;

        default:
            break;
        }
    }
}

void CInputHandler::onMouseMove(SDL_Event& event)
{
    m_mousePosition->setX(event.motion.x);
    m_mousePosition->setY(event.motion.y);
}

void CInputHandler::onMouseButtonDown(SDL_Event& event)
{
    if (event.button.button == SDL_BUTTON_LEFT)
    {
        m_mouseButtonStates[LEFT] = true;
    }

    if (event.button.button == SDL_BUTTON_MIDDLE)
    {
        m_mouseButtonStates[MIDDLE] = true;
    }

    if (event.button.button == SDL_BUTTON_RIGHT)
    {
        m_mouseButtonStates[RIGHT] = true;
    }
}

void CInputHandler::onMouseButtonUp(SDL_Event& event)
{
    if (event.button.button == SDL_BUTTON_LEFT)
    {
        m_mouseButtonStates[LEFT] = false;
    }

    if (event.button.button == SDL_BUTTON_MIDDLE)
    {
        m_mouseButtonStates[MIDDLE] = false;
    }

    if (event.button.button == SDL_BUTTON_RIGHT)
    {
        m_mouseButtonStates[RIGHT] = false;
    }
}

bool CInputHandler::isKeyDown(SDL_Scancode key) const
{
    if (m_keystates != 0)
    {
        if (m_keystates[key] == 1)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    return false;
}

void CInputHandler::onKeyDown()
{
    m_keystates = SDL_GetKeyboardState(0);
}

void CInputHandler::onKeyUp()
{
    m_keystates = SDL_GetKeyboardState(0);
}
