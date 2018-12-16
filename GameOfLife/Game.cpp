#include "Game.hpp"


Game::Game(unsigned int resX, unsigned int resY, unsigned int maxFPS, unsigned long long randomChance, float cellSize, float cellGap,
	const Color &aliveCellColor, const Color &deadCellColor, const Color &hoveredCellColor, const Color &bckgrndColor) : bckgrndColor(bckgrndColor)
{
	gameWindow = make_unique<RenderWindow>(VideoMode(resX, resY), GAME_TITLE, Style::Default);
	SetMaxFPS(maxFPS);

	simulationDelay = 0;
	paused = true;

	gameField = make_unique<GameField>((unsigned int)(gameWindow->getSize().x / (cellSize + cellGap)),
									   (unsigned int)((gameWindow->getSize().y - GAMEFIELD_SIZE_OFFSET) / (cellSize + cellGap)),
										Vector2f(0, (float)GAMEFIELD_SIZE_OFFSET), randomChance, cellSize, cellGap, aliveCellColor, deadCellColor, hoveredCellColor);

	if (gameField->IsHoveredOnCell())
	{
		Vector2u hoveredCellCoords = gameField->GetHoveredCellCoords();
	}
}

Game::~Game()
{
	paused = true;
	sleep(milliseconds(1));
	simulationThread.detach();

	if (gameWindow != nullptr)
	{
		if (gameWindow->isOpen())
			gameWindow->close();
	}
}

void Game::Run()
{
	RandomizeField();
	simulationThread = thread(&Game::simulationTask, this);
	while (gameWindow->isOpen())
	{
		HandleInput();
		Tick();
		Render();
		sleep(milliseconds(1));
	}
}

void Game::HandleInput()
{
	Event event;

	while (gameWindow->pollEvent(event))
	{
		if (event.type == Event::MouseMoved)
			gameField->SetLocalMousePosition(Vector2u(event.mouseMove.x, event.mouseMove.y));
		else if (event.type == Event::MouseButtonPressed)
			gameField->MouseClicked();
		else if (event.type == Event::Closed)
			gameWindow->close();
		else if (event.type == Event::KeyPressed)
		{
			switch (event.key.code)
			{
				case Keyboard::Escape:
					gameWindow->close();
					break;
				case Keyboard::P:
					ToggleGameState();
					break;
				case Keyboard::L:
					RandomizeField();
					break;
				case Keyboard::N:
					if (paused)
						NextGeneration();
					break;
				case Keyboard::Up:
					IncreaseDelay();
					break;
				case Keyboard::Down:
					DecreaseDelay();
					break;
				case Keyboard::Left:
					DecreaseRandomChance();
					break;
				case Keyboard::Right:
					IncreaseRandomChance();
					break;
				case Keyboard::C:
					ClearField();
					break;
			}
		}  
	}
}
void Game::Tick()
{
	if (gameField->IsHoveredOnCell()) Vector2u hoveredCellCoords = gameField->GetHoveredCellCoords();
}
void Game::Render()
{
	gameWindow->clear(bckgrndColor);
	gameWindow->draw(*gameField);
	gameWindow->display();
}

void Game::ToggleGameState()
{
	paused = !paused;
}

void Game::RandomizeField()
{
	gameField->Randomize();
}

void Game::IncreaseDelay()
{
	simulationDelay++;
}

void Game::DecreaseDelay()
{
	if (simulationDelay > 0)
		simulationDelay--;
}

void Game::IncreaseRandomChance()
{
	gameField->SetRandomChance(gameField->GetRandomChance() + 1);
}

void Game::DecreaseRandomChance()
{
	if (gameField->GetRandomChance() > 1)
		gameField->SetRandomChance(gameField->GetRandomChance() - 1);
}

const string Game::GetRandomChancePercentage() const
{
	float percentage = 100.f / gameField->GetRandomChance();
	stringstream ss;
	ss << fixed << setprecision(2) << percentage;
	return ss.str();
}

void Game::SetMaxFPS(unsigned int maxFPS)
{
	gameWindow->setFramerateLimit(maxFPS);
}

void Game::NextGeneration()
{
	if (!gameField->isStable())
	{
		gameField->NextGeneration();
	}
}

void Game::ClearField()
{
	gameField->Clear();
}

void Game::simulationTask()
{
	while (true)
	{
		while (!paused)
		{
			lockMutex.lock();
			sleep(milliseconds(simulationDelay));
			NextGeneration();
			lockMutex.unlock();
		}
		sleep(milliseconds(10));
	}
}