// Ustawienia planszy i okienka
#pragma once

using namespace sf;

// Rozmiary okienka
const unsigned int RES_X = 1080;
const unsigned int RES_Y = 1080;

// Klatki na sekunde
const unsigned int MAX_FPS = 60;

// Rozmiary komorki
const float CELL_SIZE = 5.f;
const float CELL_GAP = 2.f;

// Losowosc mnozenia
const unsigned long long RANDOM_CHANCE = 10ull;

// Kolory komorek w poszczegolnych stanach istnienia
const Color CELL_ALIVE_COLOR = Color::Magenta;
const Color CELL_DEAD_COLOR = Color(30, 30, 30);
const Color CELL_HOVERED_COLOR = Color(84, 148, 250, 200);
const Color BACKGROUND_COLOR = Color::Black;