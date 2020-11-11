#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <Windows.h>

#include "evoluutio.h"
#include "peli.h"


int main()
{
	/*
	std::cout << "player1 brain name?: " << std::endl;
	std::string inp1;
	std::getline(std::cin, inp1);
	std::cout << "player2 brain name?: " << std::endl;
	std::string inp2;
	std::getline(std::cin, inp2);
	*/
	long counter_evogen = 10000;

	std::cout << "HOLD SHIFT TO PAUSE THE LOOP (then q to breka from it, ws to save all brains)" << std::endl;
	const int SanmanaikasetPelit = 100;
	static const int threadAmo = 20; //samanaikasten pelien pitaa olla jaollinen talla!!!!!!!

	std::cout << "initializing AI" << std::endl;
	srand(time(NULL)); // tekee jokasesta randomista joka kerta erilaisen

	while (true)
	{

		evoluutio evo1(SanmanaikasetPelit, "QWE" + std::to_string(counter_evogen), "X", { 1,1 }, { 100,160,80,40,7 });
		evoluutio evo2(SanmanaikasetPelit, "ASD" + std::to_string(counter_evogen), "0", { 20,20 }, { 100,160,80,40,7 });
		evoluutio evo3(1, "asdaddaddadasasd", "0", { 20,20 }, { 100,160,80,40,7 });


		//create pelilista
		std::vector<peli> pelilista;
		for (int i = 0; i < SanmanaikasetPelit; i++)
		{
			peli thegame(i, &evo1.PelaajaStack[i], &evo2.PelaajaStack[i]);//muutettu evo2 etta 1 pelaa keskenaa PELI.cpp otettu pois 2 pelaajan vuoro
			pelilista.emplace_back(thegame);
		}

		std::thread langat[threadAmo];
		std::string user_input; //INTERRUPT inputti

		std::chrono::steady_clock::time_point begin;
		std::chrono::steady_clock::time_point end;

		std::cout << "games begin" << std::endl;
		unsigned int gencounter = 0;

		long long t1 = 0;
		long long t2 = 0;
		while (true)
		{
			begin = std::chrono::steady_clock::now();

			//pelataan pelit threadeissa

			for (int i = 0; i < SanmanaikasetPelit; i += threadAmo)
			{
				for (int j = 0; j < threadAmo; j++)
				{
					langat[j] = std::thread(&peli::Pelaa, &pelilista[i + j]);
				}
				for (int j = 0; j < threadAmo; j++)
				{
					langat[j].join();
				}
			}

			//pelilista[0].Pelaa();
			//lasketaan evoluution sisalla fitnessit vois laittaa 2 threadissa jos 2 evoo vastakkain
			for (int j = 0; j < SanmanaikasetPelit; j++)
			{
				evo1.ComputeBrainScore(j, pelilista[j].GetLauta());
				evo2.ComputeBrainScore(j, pelilista[j].GetLauta());
			}
			end = std::chrono::steady_clock::now();
			t1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

			//risteytykset ja mutaatiot
			begin = std::chrono::steady_clock::now();
			evo1.GeenienRisteytys();
			evo2.GeenienRisteytys();
			// evo2 jos 2 evoo
			end = std::chrono::steady_clock::now();
			t2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

			std::cout << "Generation " << gencounter << " " << SanmanaikasetPelit << " games " << t1 << "ms " << std::endl;
			std::cout << "Generation " << gencounter << " risteytysaika " << t2 << "ms " << std::endl;
			for (auto x : evo1.PelaajaStack[evo1.biggest_score_index].GetPelilogi())
			{
				std::cout << x << " ";
			}
			std::cout << evo1.gethighestscore() << std::endl;
			for (auto x : evo2.PelaajaStack[evo2.biggest_score_index].GetPelilogi())
			{
				std::cout << x << " ";
			}
			std::cout << evo2.gethighestscore() << std::endl;


			gencounter++;
			if (gencounter > 2000)
			{
				if (evo1.gethighestscore() > 3000)
				{
					evo1.WriteGenes(evo1.biggest_score_index);
				}
				if (evo2.gethighestscore() > 3000)
				{
					evo2.WriteGenes(evo2.biggest_score_index);
				}
				evo1.~evoluutio();
				evo2.~evoluutio();
				evo3.~evoluutio();
				break;
			}


			//user interruptit
			if (GetKeyState(VK_SHIFT) & 0x8000)
			{
				std::cout << "Nomitany?: ";
				std::getline(std::cin, user_input);

				if (user_input == "q")
				{
					break;
				}
				else if (user_input == "ws")
				{
					evo1.WriteAllGenes();
					evo2.WriteAllGenes();
				}
				else if (user_input == "wge1b")
				{
					evo1.WriteGenes(evo1.biggest_score_index);
				}
				else if (user_input == "bs")
				{
					evo1.printBrainScores();
					evo2.printBrainScores();
				}
				else if (user_input == "gl")
				{
					evo1.printGamelogs();
					evo2.printGamelogs();
				}
				else if (user_input == "bAI")
				{
					for (int j = 0; j < SanmanaikasetPelit; j++)
					{
						pelilista[j].ClearPeli();
					}
					peli uus(9999, &evo1.PelaajaStack[evo1.biggest_score_index], &evo3.PelaajaStack[0]);
					uus.PelaaAiHuman();

					uus.ClearPeli();
				}
				else if (user_input == "AIAI")
				{
					for (int j = 0; j < SanmanaikasetPelit; j++)
					{
						pelilista[j].ClearPeli();
					}
					peli uus(9999, &evo1.PelaajaStack[evo1.biggest_score_index], &evo2.PelaajaStack[evo2.biggest_score_index]);
					uus.PelaaAiAi();
				}


				else if (user_input == "?")
				{
					if (evo1.OnkoNappiAsetettu())
						std::cout << "evo1true" << std::endl;
					else
						std::cout << "evo1false" << std::endl;
					if (evo2.OnkoNappiAsetettu())
						std::cout << "evo2true" << std::endl;
					else
						std::cout << "evo2false" << std::endl;
				}
				std::cin.clear();
			}


			//pelien ja pelaajien resetoiti uutta generaatioo varten
			for (int j = 0; j < SanmanaikasetPelit; j++)
			{
				pelilista[j].ClearPeli();
			}
			std::cout << std::endl;
		}
		counter_evogen++;
	}
	return 0;
}