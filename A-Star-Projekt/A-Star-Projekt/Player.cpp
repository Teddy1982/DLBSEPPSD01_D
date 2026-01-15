#include "Player.h"

#include "Editor.h"
#include "TextureManager.h"
#include "InputHandler.h"


void CPlayer::init(int xPos, int yPos)
{
	m_XPos = xPos;
	m_YPos = yPos;

	TheEditor::Instance()->Level.m_PlayerXPos = m_XPos;
	TheEditor::Instance()->Level.m_PlayerYPos = m_YPos;

	m_OldPosition.m_x = m_XPos * TILE;
	m_OldPosition.m_y = m_YPos * TILE;

	m_Position = m_OldPosition;
	
	m_XFrame = 0;
	m_YFrame = 1;

	m_Direction = 2;
	m_Speed = 3.0f;

	m_isMoving = false;
}

void CPlayer::update()
{	
	if (m_isMoving)
	{
		move();
		return;
	}

	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_W))
	{
		m_Direction = 1;
		m_YFrame = 0;
		m_isMoving = true;
		return;
	}
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_S))
	{
		m_Direction = 2;
		m_YFrame = 1;
		m_isMoving = true;
		return;
	}
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_A))
	{
		m_Direction = 3;
		m_YFrame = 2;
		m_isMoving = true;
		return;
	}
	if (TheInputHandler::Instance()->isKeyDown(SDL_SCANCODE_D))
	{
		m_Direction = 4;
		m_YFrame = 3;
		m_isMoving = true;
		return;
	}
}

void CPlayer::render()
{
	TheTextureManager::Instance()->drawFrame("player", TILE * m_XFrame, TILE * m_YFrame, m_Position.m_x + OFFSET_X, m_Position.m_y + OFFSET_Y, TILE, TILE, TheEditor::Instance()->getRenderer());
}

void CPlayer::move()
{
	switch (m_Direction)
	{
	case 1:
		moveUp();
		break;
	case 2:
		moveDown();
		break;
	case 3:
		moveLeft();
		break;
	case 4:
		moveRight();
		break;
	default:
		break;
	}

	TheEditor::Instance()->Level.m_PlayerXPos = m_XPos;
	TheEditor::Instance()->Level.m_PlayerYPos = m_YPos;
}

void CPlayer::moveUp()
{
	if (TheEditor::Instance()->Level.collision(m_XPos, m_YPos - 1))
	{
		m_isMoving = false;
		return;
	}

	m_isMoving = true;
	m_Direction = 1;
	
	m_Velocity.m_x = 0;
	m_Velocity.m_y = -1 * m_Speed;

	m_YFrame = 0;

	static int animChange = 0;
	animChange++;
	if (animChange % 5 == 0)
	{
		m_XFrame++;

		if (m_XFrame == 3)
			m_XFrame = 0;
	}

	m_Position += m_Velocity;

	if (m_Position.m_y < m_OldPosition.m_y - TILE)
	{
		m_Position.m_y = m_OldPosition.m_y - TILE;
		m_OldPosition.m_y -= TILE;
		m_Velocity.m_y = 0;
		
		m_YPos -= 1;
		TheEditor::Instance()->changeUniqueValue(m_XPos, m_YPos, PLAYER_ID);
		m_XFrame = 0;
		m_isMoving = false;
	}
}

void CPlayer::moveDown()
{
	if (TheEditor::Instance()->Level.collision(m_XPos, m_YPos + 1))
	{
		m_isMoving = false;
		return;
	}

	m_isMoving = true;
	m_Direction = 2;

	m_Velocity.m_x = 0;
	m_Velocity.m_y = 1 * m_Speed;

	m_YFrame = 1;

	static int animChange = 0;
	animChange++;
	if (animChange % 5 == 0)
	{
		m_XFrame++;

		if (m_XFrame == 3)
			m_XFrame = 0;
	}

	m_Position += m_Velocity;

	if (m_Position.m_y > m_OldPosition.m_y + TILE)
	{
		m_Position.m_y = m_OldPosition.m_y + TILE;
		m_OldPosition.m_y += TILE;
		m_Velocity.m_y = 0;
		
		m_YPos += 1;
		TheEditor::Instance()->changeUniqueValue(m_XPos, m_YPos, PLAYER_ID);
		m_XFrame = 0;
		m_isMoving = false;
	}
}

void CPlayer::moveLeft()
{
	if (TheEditor::Instance()->Level.collision(m_XPos - 1, m_YPos))
	{
		m_isMoving = false;
		return;
	}

	m_isMoving = true;
	m_Direction = 3;

	m_Velocity.m_x = -1 * m_Speed;
	m_Velocity.m_y = 0;

	m_YFrame = 2;

	static int animChange = 0;
	animChange++;
	if (animChange % 5 == 0)
	{
		m_XFrame++;

		if (m_XFrame == 3)
			m_XFrame = 0;
	}

	m_Position += m_Velocity;

	if (m_Position.m_x < m_OldPosition.m_x - TILE)
	{
		m_Position.m_x = m_OldPosition.m_x - TILE;
		m_OldPosition.m_x -= TILE;
		m_Velocity.m_x = 0;
		
		m_XPos -= 1;
		TheEditor::Instance()->changeUniqueValue(m_XPos, m_YPos, PLAYER_ID);
		m_XFrame = 0;
		m_isMoving = false;
	}
}

void CPlayer::moveRight()
{
	if (TheEditor::Instance()->Level.collision(m_XPos + 1, m_YPos))
	{
		m_isMoving = false;
		return;
	}

	m_isMoving = true;
	m_Direction = 4;

	m_Velocity.m_x = 1 * m_Speed;
	m_Velocity.m_y = 0;

	m_YFrame = 3;

	static int animChange = 0;
	animChange++;
	if (animChange % 5 == 0)
	{
		m_XFrame++;

		if (m_XFrame == 3)
			m_XFrame = 0;
	}

	m_Position += m_Velocity;

	if (m_Position.m_x > m_OldPosition.m_x + TILE)
	{
		m_Position.m_x = m_OldPosition.m_x + TILE;
		m_OldPosition.m_x += TILE;
		m_Velocity.m_x = 0;
		
		m_XPos += 1;
		TheEditor::Instance()->changeUniqueValue(m_XPos, m_YPos, PLAYER_ID);
		m_XFrame = 0;
		m_isMoving = false;
	}
}

