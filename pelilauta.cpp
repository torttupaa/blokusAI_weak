#include "pelilauta.h"
#include <vector>
#include <iostream>
#include <Windows.h>

pelilauta::pelilauta(int k, std::string mark)
{
	this->koko = k;
	this->emptymark = mark;
	std::vector<std::string> vektori(koko);
	std::vector<std::string> vektoriTOP(koko);
	for (int x = 0; x < koko; x++)
	{
		if (x == 0 || x == koko - 1)
		{
			vektori[x] = "#";
		}
		else
		{
			vektori[x] = emptymark;
		}
	}
	for (int x = 0; x < koko; x++)
	{
		vektoriTOP[x] = "#";
	}
	for (int x = 0; x < koko; x++)
	{
		if (x == 0 || x == koko - 1)
		{
			Lauta.emplace_back(vektoriTOP);
		}
		else
		{
			Lauta.emplace_back(vektori);
		}
	}
}
pelilauta::pelilauta()
{

}
pelilauta::~pelilauta()
{
	Lauta.clear();
}
void pelilauta::GetState()
{
	for (auto x : Lauta)
	{
		for (auto y : x)
		{
			std::cout << y << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
void pelilauta::ClearMarks()
{
	Lauta.clear();
	std::vector<std::string> vektori(koko);
	std::vector<std::string> vektoriTOP(koko);
	for (int x = 0; x < koko; x++)
	{
		if (x == 0 || x == koko - 1)
		{
			vektori[x] = "#";
		}
		else
		{
			vektori[x] = emptymark;
		}
	}
	for (int x = 0; x < koko; x++)
	{
		vektoriTOP[x] = "#";
	}
	for (int x = 0; x < koko; x++)
	{
		if (x == 0 || x == koko - 1)
		{
			Lauta.emplace_back(vektoriTOP);
		}
		else
		{
			Lauta.emplace_back(vektori);
		}
	}
}
void pelilauta::Clear()
{
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;
	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	SetConsoleCursorPosition(console, topLeft);
}
int pelilauta::GetSize()
{
	return koko;
}