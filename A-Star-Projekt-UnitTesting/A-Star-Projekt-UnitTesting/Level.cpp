#include "Level.h"
#include <iostream>

CLevel::~CLevel()
{

}

void CLevel::init(int x, int y, int width, int height)
{
	m_X = x;
	m_Y = y;
	m_Width = width;
	m_Height = height;

	m_Indices.assign(MAX_Y * MAX_X, 0);
	m_IndexX = 0;
	m_IndexY = 0;
}

bool CLevel::collision(int xPos, int yPos)
{
	if ((xPos < 0) || (xPos >= MAX_X) || (yPos < 0) || (yPos >= MAX_Y)) {
		return true;
	}

	const int idx = yPos * MAX_X + xPos;
	if (m_Indices[idx] != 0) {
		return true;
	}

	return false;
}