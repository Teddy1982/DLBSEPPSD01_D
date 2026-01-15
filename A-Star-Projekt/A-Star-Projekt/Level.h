#pragma once

#include <vector>
#include "ConstValues.h"

class CLevel
{
public:
	CLevel() {}
	~CLevel();

	int m_X;
	int m_Y;
	int m_Width;
	int m_Height;

	int m_IndexX;
	int m_IndexY;

	bool m_isHindernis = false;
	bool m_isComputer = false;
	bool m_isSpieler = false;
	bool m_isStarten = false;

	std::vector<int> m_Indices;

	int m_PlayerXPos;
	int m_PlayerYPos;

	void init(int x, int y, int width, int height);
	bool collision(int xPos, int yPos);
};