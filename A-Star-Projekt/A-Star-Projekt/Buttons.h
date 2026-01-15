#pragma once

class CButton
{
public:
    CButton() {}
    ~CButton() {}
    
    int m_X;
    int m_Y;
    int m_Width;
    int m_Height;

    void init(int x, int y, int width, int height)
    {
        m_X = x;
        m_Y = y;
        m_Width = width;
        m_Height = height;
    }
};