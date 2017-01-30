#include "VictoryCondition.h"

Victory::Victory()
{
	_gotTheTreasure = false;
	Grid gettingTheGrid;
	exit = gettingTheGrid.getData(exiting).xz;

}

Victory::~Victory()
{

}

void Victory::checkifPlayerWon(glm::vec2 playerpos)
{
	glm::vec2 fixedPlayerPos;
	exit.x = (int)glm::floor(exit.x / GRIDSPACE);
	exit.y = (int)glm::floor(exit.y / GRIDSPACE);
	fixedPlayerPos.x = (int)glm::floor(playerpos.x / GRIDSPACE);
	fixedPlayerPos.y = (int)glm::floor(playerpos.y / GRIDSPACE);
	if (fixedPlayerPos == exit && _gotTheTreasure == true)
	{
		std::cout << "you won congratulations" << std::endl;
	}
	
}
