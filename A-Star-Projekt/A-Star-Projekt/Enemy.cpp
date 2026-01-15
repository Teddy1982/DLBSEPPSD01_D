#include "Enemy.h"

#include "Editor.h"
#include "TextureManager.h"

#include <list>

void CEnemy::init(int xPos, int yPos)
{
	m_XPos = xPos;
	m_YPos = yPos;
	m_Direction = 2;
	m_ID = ENEMY_ID;

	m_Position.m_x = m_XPos * TILE;
	m_Position.m_y = m_YPos * TILE;

	m_OldPosition = m_Position;

	m_Speed = 1.5f;

	m_XFrame = 0;
	switch (m_Direction)
	{
	case 1:
		m_YFrame = 0;
		break;
	case 2:
		m_YFrame = 1;
		break;
	case 3:
		m_YFrame = 2;
		break;
	case 4:
		m_YFrame = 3;
		break;
	default:
		break;
	}

	for (int i = 0; i < MAX_X; i++)
	{
		for (int j = 0; j < MAX_Y; j++)
		{
			m_bDirectionMap[i][j] = false;
		}
	}
	
	AStarNodeInit();
}

void CEnemy::render()
{
    int sourceX = m_XFrame * TILE;
    int sourceY = m_YFrame * TILE;

	for (sAStarNode nodeNeighbour :m_SearchedFields) {
		if ((!nodeNeighbour.bVisited && nodeNeighbour.bObstacle == 0) && TheEditor::Instance()->Level.m_Indices[nodeNeighbour.y * MAX_X + nodeNeighbour.x] != PLAYER_ID) {
			TheTextureManager::Instance()->drawFrame("path_tiles", 0 * TILE, 0, nodeNeighbour.x * TILE + OFFSET_X, nodeNeighbour.y * TILE + OFFSET_Y, TILE, TILE, TheEditor::Instance()->getRenderer());
		}
		else if ((nodeNeighbour.bVisited && nodeNeighbour.bObstacle == 0) && TheEditor::Instance()->Level.m_Indices[nodeNeighbour.y * MAX_X + nodeNeighbour.x] != PLAYER_ID) {
			TheTextureManager::Instance()->drawFrame("path_tiles", 1 * TILE, 0, nodeNeighbour.x * TILE + OFFSET_X, nodeNeighbour.y * TILE + OFFSET_Y, TILE, TILE, TheEditor::Instance()->getRenderer());
		}
	}

	m_SearchedFields.clear();


	//  Weg von nodeStart zum nodeEnd darstellen:

	for (int i = 0; i < MAX_X; i++) {
		for (int j = 0; j < MAX_Y; j++) {
			if (m_bDirectionMap[i][j] == true) {
				TheTextureManager::Instance()->drawFrame("path_tiles", 2 * TILE, 0, i * TILE + OFFSET_X, j * TILE + OFFSET_Y, TILE, TILE, TheEditor::Instance()->getRenderer());
			}
		}
	}
	
	TheTextureManager::Instance()->drawFrame("enemy", sourceX, sourceY, m_Position.m_x + OFFSET_X, m_Position.m_y + OFFSET_Y, TILE, TILE, TheEditor::Instance()->getRenderer());
}

void CEnemy::update()
{
	searchWayToPlayer();
	
	switch (m_Direction)
	{
	case 1:
		m_YFrame = 0;
		break;
	case 2:
		m_YFrame = 1;
		break;
	case 3:
		m_YFrame = 2;
		break;
	case 4:
		m_YFrame = 3;
		break;
	default:
		break;
	}

	if (m_YPos > 0 && m_bDirectionMap[m_XPos][m_YPos - 1] == true && TheEditor::Instance()->Level.m_Indices[(m_YPos-1) * MAX_X + m_XPos] != PLAYER_ID)
	{
		moveUp();
		return;
	}
	if (m_YPos < MAX_Y - 1 && m_bDirectionMap[m_XPos][m_YPos + 1] == true && TheEditor::Instance()->Level.m_Indices[(m_YPos + 1) * MAX_X + m_XPos] != PLAYER_ID)
	{
		moveDown();
		return;
	}
	if (m_XPos > 0 && m_bDirectionMap[m_XPos - 1][m_YPos] == true && TheEditor::Instance()->Level.m_Indices[m_YPos * MAX_X + (m_XPos - 1)] != PLAYER_ID)
	{
		moveLeft();
		return;
	}
	if (m_XPos < MAX_X - 1 && m_bDirectionMap[m_XPos + 1][m_YPos] == true && TheEditor::Instance()->Level.m_Indices[m_YPos * MAX_X + (m_XPos + 1)] != PLAYER_ID)
	{
		moveRight();
		return;
	}
}

void CEnemy::moveUp()
{
	m_Direction = 1;

	m_Velocity.m_x = 0;
	m_Velocity.m_y = -1 * m_Speed;

	static int animChange = 0;
	animChange++;
	if (animChange % 5 == 0)
	{
		m_XFrame++;

		if (m_XFrame == 3)
			m_XFrame = 0;
	}

	m_Position += m_Velocity;

	if (m_Position.m_y <= m_OldPosition.m_y - TILE)
	{
		m_Position.m_y = m_OldPosition.m_y - TILE;
		m_OldPosition.m_y -= TILE;
		m_Velocity.m_y = 0;

		m_YPos -= 1;
		TheEditor::Instance()->changeUniqueValue(m_XPos, m_YPos, m_ID);
		m_XFrame = 0;
	}
}

void CEnemy::moveDown()
{
	m_Direction = 2;

	m_Velocity.m_x = 0;
	m_Velocity.m_y = m_Speed;

	static int animChange = 0;
	animChange++;
	if (animChange % 5 == 0)
	{
		m_XFrame++;

		if (m_XFrame == 3)
			m_XFrame = 0;
	}

	m_Position += m_Velocity;

	if (m_Position.m_y >= m_OldPosition.m_y + TILE)
	{
		m_Position.m_y = m_OldPosition.m_y + TILE;
		m_OldPosition.m_y += TILE;
		m_Velocity.m_y = 0;

		m_YPos += 1;
		TheEditor::Instance()->changeUniqueValue(m_XPos, m_YPos, m_ID);
		m_XFrame = 0;
	}
}

void CEnemy::moveLeft()
{
	m_Direction = 3;

	m_Velocity.m_x = -1 * m_Speed;
	m_Velocity.m_y = 0;

	static int animChange = 0;
	animChange++;
	if (animChange % 5 == 0)
	{
		m_XFrame++;

		if (m_XFrame == 3)
			m_XFrame = 0;
	}

	m_Position += m_Velocity;

	if (m_Position.m_x <= m_OldPosition.m_x - TILE)
	{
		m_Position.m_x = m_OldPosition.m_x - TILE;
		m_OldPosition.m_x -= TILE;
		m_Velocity.m_x = 0;

		m_XPos -= 1;
		TheEditor::Instance()->changeUniqueValue(m_XPos, m_YPos, m_ID);
		m_XFrame = 0;
	}
}

void CEnemy::moveRight()
{
	m_Direction = 4;

	m_Velocity.m_x = m_Speed;
	m_Velocity.m_y =0;

	static int animChange = 0;
	animChange++;
	if (animChange % 5 == 0)
	{
		m_XFrame++;

		if (m_XFrame == 3)
			m_XFrame = 0;
	}

	m_Position += m_Velocity;

	if (m_Position.m_x >= m_OldPosition.m_x + TILE)
	{
		m_Position.m_x = m_OldPosition.m_x + TILE;
		m_OldPosition.m_x += TILE;
		m_Velocity.m_x = 0;

		m_XPos += 1;
		TheEditor::Instance()->changeUniqueValue(m_XPos, m_YPos, m_ID);
		m_XFrame = 0;
	}
}

void CEnemy::AStarNodeInit()
{
	CEditor* Editor = TheEditor::Instance();
	
	for (int x = 0; x < MAX_X; x++)
		for (int y = 0; y < MAX_Y; y++)
		{
			if (Editor->Level.m_Indices[y * MAX_X + x] == 1)
				m_pAStarNode[(y * MAX_X) + x].bObstacle = true;
			else
				m_pAStarNode[(y * MAX_X) + x].bObstacle = false;

			m_pAStarNode[(y * MAX_X) + x].x = x;
			m_pAStarNode[(y * MAX_X) + x].y = y;
//			m_pAStarNode[(y * MAX_X) + x].bObstacle = false;
			m_pAStarNode[(y * MAX_X) + x].parent = nullptr;
			m_pAStarNode[(y * MAX_X) + x].bVisited = false;
		}

	// Create connections - in this case nodes are on a regular grid
	for (int x = 0; x < MAX_X; x++)
		for (int y = 0; y < MAX_Y; y++)
		{
			if (y > 0)
				m_pAStarNode[(y * MAX_X) + x].vecNeighbours.push_back(&m_pAStarNode[((y - 1) * MAX_X) + (x + 0)]);
			if (y < (MAX_Y - 1))
				m_pAStarNode[(y * MAX_X) + x].vecNeighbours.push_back(&m_pAStarNode[((y + 1) * MAX_X) + (x + 0)]);
			if (x > 0)
				m_pAStarNode[(y * MAX_X) + x].vecNeighbours.push_back(&m_pAStarNode[((y + 0) * MAX_X) + (x - 1)]);
			if (x < (MAX_X - 1))
				m_pAStarNode[(y * MAX_X) + x].vecNeighbours.push_back(&m_pAStarNode[((y + 0) * MAX_X) + (x + 1)]);
		}
}

void CEnemy::searchWayToPlayer()
{
	int playerXPos = TheEditor::Instance()->Level.m_PlayerXPos;
	int playerYPos = TheEditor::Instance()->Level.m_PlayerYPos;

	sAStarNode* nodeStart = &m_pAStarNode[(m_YPos * MAX_X) + m_XPos];
	sAStarNode* nodeEnd = &m_pAStarNode[(playerYPos * MAX_X) + playerXPos];

	// Reset Navigation Graph - default all node states
	for (int x = 0; x < MAX_X; x++)
		for (int y = 0; y < MAX_Y; y++)
		{
			m_pAStarNode[(y * MAX_X) + x].bVisited = false;
			m_pAStarNode[(y * MAX_X) + x].fGlobalGoal = INFINITY;
			m_pAStarNode[(y * MAX_X) + x].fLocalGoal = INFINITY;
			m_pAStarNode[(y * MAX_X) + x].parent = nullptr;
		}

	auto distance = [](sAStarNode* a, sAStarNode* b) // For convenience
	{
		return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
	};

	auto heuristic = [distance](sAStarNode* a, sAStarNode* b)
	{
		return distance(a, b);
	};

	// Setup starting conditions
	sAStarNode* nodeCurrent = nodeStart;
	nodeStart->fLocalGoal = 0.0f;
	nodeStart->fGlobalGoal = heuristic(nodeStart, nodeEnd);

	std::list<sAStarNode*> listNotTestedNodes;
	listNotTestedNodes.push_back(nodeStart);

	while (!listNotTestedNodes.empty() && nodeCurrent != nodeEnd)
	{
		listNotTestedNodes.sort([](const sAStarNode* lhs, const sAStarNode* rhs) { return lhs->fGlobalGoal < rhs->fGlobalGoal; });

		while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
			listNotTestedNodes.pop_front();

		//... or abort because there are no valid nodes left to test
		if (listNotTestedNodes.empty())
			break;

		nodeCurrent = listNotTestedNodes.front();
		nodeCurrent->bVisited = true;	// We only explore a node once

		// Check each of this node's neighbours...
		for (auto nodeNeighbour : nodeCurrent->vecNeighbours)
		{
			// and only if the neighbour is not visited and is
			// not an obstacle, add it to NotTested List
			if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == 0) {
				listNotTestedNodes.push_back(nodeNeighbour);
			}

			m_SearchedFields.push_back(*nodeNeighbour);

			// Calculate the neighbours potential lowest parent distance
			float fPossiblyLowerGoal = nodeCurrent->fLocalGoal + distance(nodeCurrent, nodeNeighbour);

			// if choosing to path through this node is a lower distance than what
			// the neighbour currently has set, update the neighbour to use this node
			// as the path source, and set its distance scores as necessary
			if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
			{
				nodeNeighbour->parent = nodeCurrent;
				nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;

				// The best path length to the neighbour being tested has changed, so
				// update the neighbour's score. The heuristic is used to globally bias
				// the path algorithm, so it knows if its getting better or worse. At some
				// point the algo will realise this path is worse and abandon it, and then go
				// and search along the next best path.
				nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, nodeEnd);
			}
		}
	}

	if (nodeEnd != nullptr)
	{
		for (int i = 0; i < MAX_X; i++)
			for (int j = 0; j < MAX_Y; j++)
				m_bDirectionMap[i][j] = false;
		
		sAStarNode* p = nodeEnd;
		while (p->parent != nullptr)
		{
			m_bDirectionMap[p->x][p->y] = true;
			// Set next node to this node's parent
			p = p->parent;

		}
	}

	m_bDirectionMap[m_XPos][m_YPos] = false;
}