#pragma once

#include <iostream>
#include <string>
#include <map>

#include "SDL.h"
#include "SDL_image.h"

class CTextureManager {

public:

    ~CTextureManager() {};

    static CTextureManager* Instance()
    {
        if (s_pInstance == nullptr)
        {
            s_pInstance = new CTextureManager();
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

    bool load(std::string fileName, std::string id, SDL_Renderer* pRenderer);

    void clearTextureMap();
    void clearFromTextureMap(std::string id);

    void draw(std::string id, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);
    void drawFrame(std::string id, int sourceX, int sourceY, int x, int y, int width, int height, SDL_Renderer* pRenderer, SDL_RendererFlip flip = SDL_FLIP_NONE);

    std::map<std::string, SDL_Texture*> getTextureMap() { return m_textureMap; }

    static CTextureManager* s_pInstance;

    int m_tilemapWidth;
    int m_tilemapHeight;

private:

    CTextureManager() {};

    std::map<std::string, SDL_Texture*> m_textureMap;
};

typedef CTextureManager TheTextureManager;
