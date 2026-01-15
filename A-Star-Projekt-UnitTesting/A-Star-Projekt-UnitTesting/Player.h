#pragma once

#include "Vector2D.h"
#include "ConstValues.h"

class CPlayer
{
public:

	CPlayer() {};
	~CPlayer() {};

	CVector2D m_Position;
	CVector2D m_Velocity;

	void init(int xPos, int yPos);
	
	void update();
	void render();

	void move();
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

	int m_XPos = 0;
	int m_YPos = 0;

private:

	CVector2D m_OldPosition;
	int m_XFrame, m_YFrame;
	
	int m_Direction;
	float m_Speed;
	bool m_isMoving;

	int m_DirectionMap[MAX_X][MAX_Y] = { 0 };
};