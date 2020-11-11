#pragma once
#include "nappula.h"
#include "pelilauta.h"
#include "aivot.h"

class pelaaja
{
private:
	int PlayerID;
	std::string symbol;
	std::vector<nappula> napit;
	std::vector<std::vector<int>> current_kulmat;
	std::vector<std::vector<int>> alkuAvail;
	std::vector<std::vector<int>> AvailableCorners = { {3,3} };
	std::vector<std::vector<int>> Unavailable = { {0,0} };
	std::vector<std::string> pelilogi;
	std::vector<float> InputLayer;
	bool elossa = true;
	bool OnkoVektoriVektorissa(std::vector<std::vector<int>> v1, std::vector<int> v2);
public:
	aivot aivo;
	pelaaja(bool tuplanappulat , std::string sym, std::vector<int> vekki, std::string BrainName, int ID, std::vector<int> BrainComplexity);
	pelaaja();
	~pelaaja();
	void PrintNapit();
	std::vector<std::vector<int>> ManipNappula(int nappula_index, bool mirror, int nextcorner_hops, int rotations);
	bool PlaceNappula(pelilauta* pelilauta, std::vector<int> Corner, std::vector<std::vector<int>> napin_kulmat);
	bool vuoro(pelilauta* pelilauta);
	std::vector<std::string> GetPelilogi();
	void Reset();
	bool AIvuoro(pelilauta* pelilauta);
	bool AIvuoroVisible(pelilauta* pelilauta);
};