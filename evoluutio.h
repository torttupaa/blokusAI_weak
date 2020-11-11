#pragma once
#include "aivot.h"
#include "pelaaja.h"
#include<vector>
#include <thread>


class evoluutio
{
private:
	std::string BrainName;
	static const int ThreadAmo = 10;
	std::thread langat[ThreadAmo];
	std::vector<int> BrainScores;
	std::string pelimerkki; //tarvii scorejen laskentaan
	bool nappi_laudassa = false;
	std::vector<int> koot;
public:
	//std::vector<aivot> AivoStack;
	std::vector<pelaaja> PelaajaStack;
	int biggest_score_index;

	evoluutio(int PelaajaStackSize, std::string BrainName, std::string pelinappula, std::vector<int> alkupiste, std::vector<int> BrainComplexity);
	~evoluutio();
	void WriteAllGenes();
	void WriteGenes(int id);
	void ComputeBrainScore(int id, std::vector<std::vector<std::string>> lauda);
	void GeenienRisteytys();
	void GeenienRisteytys2();
	void printBrainScores();
	void printGamelogs();
	void mutaatio_thread(std::vector<int> koot, int aivoindex);
	void mutaatio_thread2(std::vector<int> koot, int aivoindex);
	void mutaatio_thread3(std::vector<int> koot, int aivoindex);
	void risteytys_thread(std::vector<int> koot, int alkuindex, std::vector<int> suurimmat, std::vector<int> loput);
	bool OnkoNappiAsetettu();
	int gethighestscore();
};