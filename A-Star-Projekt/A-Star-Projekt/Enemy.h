#pragma once

#include "Vector2D.h"
#include "ConstValues.h"

#include <vector>

class CEnemy {
public:
    
	CEnemy()
	{
		m_pAStarNode = new sAStarNode[MAX_Y*MAX_X];
	}

    ~CEnemy()
	{
		if (m_pAStarNode != NULL)
		{
			delete[] m_pAStarNode;
			m_pAStarNode = NULL;
		}
	}

	CVector2D m_Position;
	CVector2D m_OldPosition;
	CVector2D m_Velocity;
	
	int m_XPos, m_YPos;
	
	int m_ID;
	
	void init(int xPos, int yPos);

    void render();
    void update();

private:

    int m_XFrame;
    int m_YFrame;

    int m_Direction;

    float m_Speed;

	struct sAStarNode
	{
	public:
		sAStarNode() {}
		~sAStarNode() { parent = NULL; }
		
		bool bObstacle = false;
		bool bVisited = false;
		float fGlobalGoal;
		float fLocalGoal;
		int x;
		int y;
		std::vector<sAStarNode*> vecNeighbours;
		sAStarNode* parent;
	};

	sAStarNode* m_pAStarNode = nullptr;

	std::vector<sAStarNode> m_SearchedFields;

	bool m_bDirectionMap[MAX_X][MAX_Y];

	void AStarNodeInit();
	void searchWayToPlayer();

	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
};
