#pragma once

#include "SFML.hpp"
#include "Randomizer.hpp"

using namespace sf;
using namespace std;

class GameField : public Drawable
{
public:
	GameField(unsigned int fieldWidth, unsigned int fieldHeight, const Vector2f &fieldPosition, unsigned long long randomChance, float cellSize, float cellGap, const Color &aliveCellColor, const Color &deadCellColor, const Color &hoveredCellColor);

	void draw(RenderTarget& target, RenderStates = RenderStates::Default) const;

	void SetPosition(const Vector2f& position);
	const Vector2f& GetPosition() const;

	const Vector2u GetSize() const;

	void Randomize();
	void Clear();
	void NextGeneration();

	void SetCellSize(float cellSize);
	float GetCellSize() const;

	void SetCellGap(float cellGap);
	float GetCellGap() const;

	void SetLocalMousePosition(const Vector2u &localMousePosition);
	void MouseClicked();

	unsigned long long GetGeneration() const;

	void SetRandomChance(unsigned long long randomChance);
	unsigned long long GetRandomChance() const;

	void SetAliveCellColor(const Color &aliveCellColor);
	const Color &GetAliveCellColor() const;

	void SetDeadCellColor(const Color &deadCellColor);
	const Color &GetDeadCellColor() const;

	void SetHoveredCellColor(const Color &hoveredCellColor);
	const Color &GetHoveredCellColor() const;

	bool IsHoveredOnCell() const;
	const Vector2u &GetHoveredCellCoords() const;

	bool isStable() const;

protected:
	unsigned int GetAliveNeighboursCount(const vector<vector<bool>>& field, unsigned int x, unsigned int y) const;
	void UpdateVertices();

private:
	vector<vector<bool>> gameField;
	RectangleShape hoveredCellRect;
	VertexArray vertices;
	Vector2f position;

	bool hoveredOnCell, stable;
	Randomizer randomizer;
	unsigned long long generation, randomChance;
	float cellSize, cellGap, cellSizeAndGap;
	Color aliveCellColor, deadCellColor;
	Vector2u hoveredCellCoords;
};
