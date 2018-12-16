#include "GameField.hpp"
#include "Randomizer.hpp"


GameField::GameField(unsigned int fieldWidth, unsigned int fieldHeight, const Vector2f &fieldPosition, unsigned long long randomChance, float cellSize, float cellGap, const Color &aliveCellColor, const Color &deadCellColor, const Color &hoveredCellColor)
	: position(fieldPosition), randomChance(randomChance), cellSize(cellSize), cellGap(cellGap), aliveCellColor(aliveCellColor), deadCellColor(deadCellColor)
{
	gameField = vector<vector<bool>>(fieldWidth, vector<bool>(fieldHeight));

	if (cellSize > 1.f)
		vertices = VertexArray(PrimitiveType::Quads, fieldWidth * fieldHeight * 4);
	else
		vertices = VertexArray(PrimitiveType::Points, fieldWidth * fieldHeight);

	cellSizeAndGap = cellSize + cellGap;
	hoveredCellRect = RectangleShape(Vector2f(cellSize, cellSize));
	generation = 0;
	stable = false;

	randomizer.Seed((unsigned long)time(nullptr));

	UpdateVertices();
}

void GameField::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(vertices, states);

	if (hoveredOnCell) {
		target.draw(hoveredCellRect, states);
	}
}

void GameField::SetPosition(const Vector2f& position)
{
	this->position = position;
}

const Vector2f& GameField::GetPosition() const
{
	return position;
}

const Vector2u GameField::GetSize() const
{
	if (gameField.size() > 0)
		return Vector2u((unsigned int)gameField.size(), (unsigned int)gameField[0].size());

	return Vector2u(0, 0);
}

void GameField::Randomize()
{
	Vector2u gameFieldSize = GetSize();

	for (unsigned int x = 0; x < gameFieldSize.x; x++)
		for (unsigned int y = 0; y < gameFieldSize.y; y++)
		{
			bool alive = randomizer.Random<unsigned long long>(1, randomChance) == 1;
			gameField[x][y] = alive;
		}

	generation = 0;
	stable = false;
	UpdateVertices();
}

void GameField::Clear()
{
	for (auto &column : gameField)
		fill(column.begin(), column.end(), false);

	generation = 0;
	stable = false;
	UpdateVertices();
}

void GameField::NextGeneration()
{
	if (!stable)
	{
		bool changed = false;
		vector<vector<bool>> tempField(gameField);

		for (unsigned int x = 0; x < tempField.size(); x++)
			for (unsigned int y = 0; y < tempField[0].size(); y++)
			{
				int aliveNeighboursCount = GetAliveNeighboursCount(tempField, x, y);

				if (gameField[x][y] && (aliveNeighboursCount < 2 || aliveNeighboursCount > 3))
				{
					gameField[x][y] = false;
					changed = true;
				}
				else if (!gameField[x][y] && aliveNeighboursCount == 3)
				{
					gameField[x][y] = true;
					changed = true;
				}
			}
		stable = !changed;
		if (!stable)
		{
			generation++;
			UpdateVertices();
		}
	}
}

void GameField::SetCellSize(float cellSize)
{
	this->cellSize = cellSize;
	cellSizeAndGap = cellSize + cellGap;
	hoveredCellRect.setSize(Vector2f(cellSize, cellSize));
	UpdateVertices();
}

float GameField::GetCellSize() const
{
	return cellSize;
}

void GameField::SetCellGap(float cellGap)
{
	this->cellGap = cellGap;
	cellSizeAndGap = cellGap + cellSize;
	UpdateVertices();
}

float GameField::GetCellGap() const
{
	return cellGap;
}

void GameField::SetLocalMousePosition(const Vector2u &localMousePosition)
{
	unsigned int relX = (unsigned int)(localMousePosition.x - position.x);
	unsigned int relY = (unsigned int)(localMousePosition.y - position.y);

	unsigned int fieldSizeX = (unsigned int)(gameField.size() * cellSizeAndGap);
	unsigned int fieldSizeY = (unsigned int)(gameField[0].size() * cellSizeAndGap);

	hoveredOnCell = false;
	if (relX < fieldSizeX && relX >= 0 && relY < fieldSizeY && relY >= 0)
	{
		if (relX % (unsigned int)cellSizeAndGap < cellSize && relY % (unsigned int)cellSizeAndGap < cellSize)
		{
			hoveredCellCoords = Vector2u((unsigned int)(relX / cellSizeAndGap), (unsigned int)(relY / cellSizeAndGap));
			hoveredCellRect.setPosition(hoveredCellCoords.x * cellSizeAndGap + position.x, hoveredCellCoords.y * cellSizeAndGap + position.y);
			hoveredOnCell = true;
		}
	}
}
void GameField::MouseClicked()
{
	if (hoveredOnCell)
	{
		gameField[hoveredCellCoords.x][hoveredCellCoords.y] = !gameField[hoveredCellCoords.x][hoveredCellCoords.y];
		stable = false;
		UpdateVertices();
	}
}

unsigned long long GameField::GetGeneration() const 
{ 
	return generation; 
}

void GameField::SetRandomChance(unsigned long long randomChance)
{
	this->randomChance = randomChance;
}
unsigned long long GameField::GetRandomChance() const
{
	return randomChance;
}

void GameField::SetAliveCellColor(const Color &aliveCellColor)
{
	this->aliveCellColor = aliveCellColor;
	UpdateVertices();
}

const Color &GameField::GetAliveCellColor() const
{
	return aliveCellColor;
}

void GameField::SetDeadCellColor(const Color &deadCellColor)
{
	this->deadCellColor = deadCellColor;
	UpdateVertices();
}

const Color &GameField::GetDeadCellColor() const
{
	return deadCellColor;
}

void GameField::SetHoveredCellColor(const Color &hoveredCellColor)
{
	hoveredCellRect.setFillColor(hoveredCellColor);
}

const Color &GameField::GetHoveredCellColor() const
{
	return hoveredCellRect.getFillColor();
}

bool GameField::IsHoveredOnCell() const
{
	return hoveredOnCell;
}

const Vector2u &GameField::GetHoveredCellCoords() const
{
	return hoveredCellCoords;
}

bool GameField::isStable() const
{
	return stable;
}

unsigned int GameField::GetAliveNeighboursCount(const vector<vector<bool>>& field, unsigned int x, unsigned int y) const
{
	int aliveNeighbours = 0;
	int neighbourOffsets[8][2] = { {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1} };

	for (const auto& currentOffset : neighbourOffsets)
	{
		int xToCheck = x + currentOffset[0];
		int yToCheck = y + currentOffset[1];

		if (xToCheck >= 0 && yToCheck >= 0 && xToCheck < field.size() && yToCheck < field[0].size())
		{
			if (field[xToCheck][yToCheck])
				aliveNeighbours++;
		}
	}
	return aliveNeighbours;
}

void GameField::UpdateVertices()
{
	size_t i = 0;

	for (unsigned int x = 0; x < gameField.size(); x++)
	{
		for (unsigned int y = 0; y < gameField[0].size(); y++)
		{
			float curPosX = position.x + x * cellSizeAndGap;
			float curPosY = position.y + y * cellSizeAndGap;

			if (cellSize > 1.f)
			{
				Vertex *quadOffset = &vertices[i * 4];

				for (unsigned int j = 0; j < 4; j++)
				{
					if (gameField[x][y])
						quadOffset[j].color = aliveCellColor;
					else
						quadOffset[j].color = deadCellColor;
				}

				quadOffset[0].position = Vector2f(curPosX, curPosY);
				quadOffset[1].position = Vector2f(curPosX + cellSize, curPosY);
				quadOffset[2].position = Vector2f(curPosX + cellSize, curPosY + cellSize);
				quadOffset[3].position = Vector2f(curPosX, curPosY + cellSize);
			}
			else {
				Vertex *vertex = &vertices[i];

				if (gameField[x][y])
					vertex->color = aliveCellColor;
				else
					vertex->color = deadCellColor;

				vertex->position = Vector2f(curPosX, curPosY);
			}
			i++;
		}
	}
}