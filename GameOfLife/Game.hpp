#pragma once

#include "GameField.hpp"

#include <memory>
#include <sstream>
#include <iomanip>
#include <thread>
#include <mutex>

using namespace std;
using namespace sf;

class Game
{
public:
	Game(unsigned int resX, unsigned int resY, unsigned int maxFPS, unsigned long long randomChance,
		float cellSize, float cellGap, const Color &aliveCellColor, const Color &deadCellColor,
		const Color &hoveredCellColor, const Color &bckgrndColor);

	Game(Game const &) = delete;	
	void operator = (Game) = delete;
	~Game();

	void Run();
protected:
	void HandleInput();
	void Tick();
	void Render();
	void ToggleGameState();
	void RandomizeField();
	void IncreaseDelay();
	void DecreaseDelay();
	void IncreaseRandomChance();
	void DecreaseRandomChance();
	const string GetRandomChancePercentage() const;
	void SetMaxFPS(unsigned int maxFPS);
	void NextGeneration();
	void ClearField();
private:
	const String GAME_TITLE = "Gra w Zycie - Konrad Wenc";
	const unsigned int GAMEFIELD_SIZE_OFFSET = 80u;

	unique_ptr<RenderWindow> gameWindow;
	unique_ptr<GameField> gameField;

	bool paused;
	unsigned int simulationDelay;

	Color bckgrndColor;

	thread simulationThread;
	mutex lockMutex;
	void simulationTask();
};