#define CATCH_CONFIG_MAIN
#include <catch_amalgamated.hpp>
#include "Level.h"
#include "ConstValues.h"

TEST_CASE("hitUpBorder") {
	CLevel level;
	level.init(210, 10, 15 * 48, 15 * 48);
	
	bool isCollide = level.collision(0, 1 - 1);

	REQUIRE(isCollide == false);

	isCollide = level.collision(0, 0 - 1);

	REQUIRE(isCollide == true);
}

TEST_CASE("hitBottomBorder") {
	CLevel level;
	level.init(210, 10, 15 * 48, 15 * 48);

	bool isCollide = level.collision(0, (MAX_Y - 2) + 1);

	REQUIRE(isCollide == false);

	isCollide = level.collision(0, (MAX_Y - 1) + 1);

	REQUIRE(isCollide == true);
}

TEST_CASE("hitLeftBorder") {
	CLevel level;
	level.init(210, 10, 15 * 48, 15 * 48);

	bool isCollide = level.collision(1 - 1, 0);

	REQUIRE(isCollide == false);

	isCollide = level.collision(0 - 1, 0);

	REQUIRE(isCollide == true);
}

TEST_CASE("hitRightBorder") {
	CLevel level;
	level.init(210, 10, 15 * 48, 15 * 48);

	bool isCollide = level.collision((MAX_X - 2) + 1, 0);

	REQUIRE(isCollide == false);

	isCollide = level.collision((MAX_X - 1) + 1, 0);

	REQUIRE(isCollide == true);
}

TEST_CASE("collideWithObstacle") {
	CLevel level;
	level.init(210, 10, 15 * 48, 15 * 48);

	level.m_Indices[15 * MAX_X + 15] = 1;

	bool isCollide = level.collision(15, 14);

	REQUIRE(isCollide == false);

	isCollide = level.collision(15, 14 + 1);

	REQUIRE(isCollide == true);
}

TEST_CASE("collideWithEnemy") {
	CLevel level;
	level.init(210, 10, 15 * 48, 15 * 48);

	level.m_Indices[15 * MAX_X + 10] = ENEMY_ID;

	bool isCollide = level.collision(9, 15);

	REQUIRE(isCollide == false);

	isCollide = level.collision(9 + 1, 15);

	REQUIRE(isCollide == true);
}
