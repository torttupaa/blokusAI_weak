#pragma once
#include "pelaaja.h"


class peli
{
private:
	int PeliID;
	pelaaja *p1;
	pelaaja *p2;
	pelilauta lauta;
public:
	peli(int id, pelaaja * p1, pelaaja* p2);
	~peli();
	void PrintPeliInfo();
	void Pelaa();
	void PelaaAiHuman();
	void PelaaAiAi();
	std::vector<std::vector<std::string>> GetLauta();
	std::vector<std::string> GetP1Log();
	std::vector<std::string> GetP2Log();
	void ClearPeli();
};

