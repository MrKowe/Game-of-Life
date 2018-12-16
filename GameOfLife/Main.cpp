/*
	TODO:
	
	Instrukcja sterowania:
	---UP/DOWN aby zwi�kszy�/zmniejszy� op�nienie
	---LEFT/RIGHT aby zwiekszyc/zmniejszyc losowosc zywych komorek
	---C Wyczyszczenie planszy
	---L Wylosowanie komorek
	---P by zapauzowac/grac
	---ESC Wyj�cie
	---N Awansowanie generacji manualnie


	Pokazanie statystyk symulacji:
	---Opoznienie symulacji
	---Szansa na losowosc
	---Status symulacja/pauza
	---kt�ra generacja
*/
#include "Game.hpp"
#include "Settings.hpp"

int main()
{
	setlocale(LC_ALL, "pl-PL");

	unique_ptr<Game> GoL = make_unique<Game>(RES_X, RES_Y, MAX_FPS, RANDOM_CHANCE, CELL_SIZE, CELL_GAP, CELL_ALIVE_COLOR, CELL_DEAD_COLOR, CELL_HOVERED_COLOR, BACKGROUND_COLOR);
	GoL->Run();
	return 0;
}