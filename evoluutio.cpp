#include "evoluutio.h"
#include "aivot.h"
#include <ctime>
#include <string>
#include <vector>
#include "pelaaja.h"
#include <algorithm>

evoluutio::evoluutio(int PelaajaStackSize,std::string BrainName, std::string pelinappula, std::vector<int> alkupiste, std::vector<int> BrainComplexity)
{
	this->BrainName = BrainName;
	this->BrainScores.assign(PelaajaStackSize, 0);
	this->pelimerkki = pelinappula;

	//srand(time(NULL));
	for (int i = 0; i < PelaajaStackSize; i++)
	{
		pelaaja player(false, pelinappula, alkupiste, BrainName, i, BrainComplexity);
		PelaajaStack.emplace_back(player);
	}

	for (auto x : this->PelaajaStack[0].aivo.GetSynapsit())
	{
		this->koot.emplace_back(x.size());
	}
	for (auto joku : koot)
	{
		std::cout << joku << " ";
	}
	std::cout << "\n";
}
evoluutio::~evoluutio()
{
	PelaajaStack.clear();
	PelaajaStack.shrink_to_fit();
}
void evoluutio::WriteAllGenes()
{
	for (int i = 0; i < this->PelaajaStack.size(); i += ThreadAmo)
	{
		for (int j = 0; j < ThreadAmo; j++)
		{
			langat[j] = std::thread(&aivot::WriteSynapseFile, &PelaajaStack[i + j].aivo);
		}
		for (int j = 0; j < ThreadAmo; j++)
		{
			langat[j].join();
		}
	}
}
void evoluutio::WriteGenes(int id)
{
	PelaajaStack[id].aivo.WriteSynapseFile();
}
void evoluutio::ComputeBrainScore(int id, std::vector<std::vector<std::string>> lauda)
{
	std::vector<std::string> logi =  PelaajaStack[id].GetPelilogi();
	int pisteet = 0;
	std::vector<std::string> edelliset;
	edelliset.assign(4, "#");

	for (auto z : lauda)
	{
		for (auto w : z)
		{
			if (w == this->pelimerkki)
			{
				pisteet += 100;
			}
		}
	}


	//pistetys tosta logista
	int counter = 0;
	int c_count = 0;
	int q_count = 0;
	for (auto x : logi)
	{
		if (x == "CM")
			pisteet -= 1;
		else if (x == "CC")
			pisteet -= 2;
		else if (x == "CR")
			pisteet -= 2;
		else if (x == "MC")
			pisteet -= 20;
		else if (x == "c")
			c_count++;
		else if (x == "q")
			q_count++;
		//else if (x == "p")
			//pisteet += 1;
		else if (x == "TurnSuccess")
		{
			pisteet += 100;
			q_count = 0;
			c_count = 0;
		}
			

		if (counter >= edelliset.size()) //jos edellisten liikkeitten lista on tarpeeks pitka analysoitavaks
		{
			std::rotate(edelliset.begin(), edelliset.begin() + 1, edelliset.end());
			edelliset.back() = x;

			if (edelliset == std::vector<std::string>{"d", "u", "d", "u"})
				pisteet -= 2;
			else if (edelliset == std::vector<std::string>{"u", "d", "u", "d"})
				pisteet -= 2;
			else if (edelliset == std::vector<std::string>{"q", "q", "q", "q"})
				pisteet -= 2;
			else if (edelliset == std::vector<std::string>{"c", "c", "c", "c"})
				pisteet -= 2;
			else if (edelliset == std::vector<std::string>{"l", "r", "l", "r"})
				pisteet -= 2;
			else if (edelliset == std::vector<std::string>{"r", "l", "r", "l"})
				pisteet -= 2;
			else if (edelliset == std::vector<std::string>{"p", "p", "p", "p"})
				pisteet -= 2;
			//circle clockwise
			else if (edelliset == std::vector<std::string>{"d", "l", "u", "r"})
				pisteet -= 1;
			else if (edelliset == std::vector<std::string>{"l", "u", "r", "d"})
				pisteet -= 1;
			else if (edelliset == std::vector<std::string>{"u", "r", "d", "l"})
				pisteet -= 1;
			else if (edelliset == std::vector<std::string>{"r", "d", "l", "u"})
				pisteet -= 1;
			//circle unclockwise
			else if (edelliset == std::vector<std::string>{"l", "d", "r", "u"})
				pisteet -= 1;
			else if (edelliset == std::vector<std::string>{"u", "l", "d", "r"})
				pisteet -= 1;
			else if (edelliset == std::vector<std::string>{"r", "u", "l", "d"})
				pisteet -= 1;
			else if (edelliset == std::vector<std::string>{"d", "r", "u", "l"})
				pisteet -= 1;

		}
		else
		{ 
			edelliset[counter] = x;
			counter++;
		}
	}
	//std::cout << pisteet;
	//std::cout << std::endl;
	if (logi[logi.size()-4] == "p")
	{
		pisteet += 5;
	}

	if (c_count > 3)
		pisteet -= 5;
	if (q_count > c_count * 3)
		pisteet -= 5;

	pisteet += stoi(logi[logi.size()-2]);
	pisteet += stoi(logi[logi.size()-1]);


	if (pisteet > 0)
		this->nappi_laudassa = true;
	

	if (logi != PelaajaStack[biggest_score_index].GetPelilogi())
		pisteet += 3;

	this->BrainScores[id] = pisteet;
}
void evoluutio::GeenienRisteytys()
{
	//etitaan THE suurin
	int suurimman_index = 0;

	// etitaan suurimmat joista poistetaan suurin
	std::vector<int> suurimmat(17);
	std::partial_sort_copy(std::begin(this->BrainScores), std::end(this->BrainScores),std::begin(suurimmat), std::end(suurimmat),std::greater<int>());

	//etitaan suurimpien indexit
	std::vector<int> suurimmat_ind;
	int Lcounter = 0;
	int edellinen = -1000;
	for (int i = 0; i < suurimmat.size(); i++)
	{
		if (i > 0)
			edellinen = suurimmat[i - 1];
		for (int j = 0; j < this->BrainScores.size(); j++)
		{
			if (Lcounter == suurimmat.size()|| edellinen == suurimmat[i])
				break;
			if (suurimmat[i] == this->BrainScores[j])
			{
				suurimmat_ind.emplace_back(j);
				Lcounter++;
			}
		}
		if (Lcounter == suurimmat.size())
			break;
	}
	/*
	for (auto x : suurimmat)
	{
		std::cout << x << " ";
	}
	std::cout << "\n";
	
	for (auto x : suurimmat_ind)
	{
		std::cout << x << " ";
	}
	std::cout << "\n";*/

	suurimman_index = suurimmat_ind[0];
	this->biggest_score_index = suurimman_index;
	suurimmat_ind.erase(suurimmat_ind.begin());
	suurimmat_ind.shrink_to_fit();
	//std::cout << suurimman_index << "\n";

	//etitaan pienimmat tuplamaara suurimpiin-1 nahden!
	std::vector<int> pienimmat(32);
	std::partial_sort_copy(std::begin(this->BrainScores), std::end(this->BrainScores), std::begin(pienimmat), std::end(pienimmat), std::less<int>());


	//pienimpien indexit
	std::vector<int> pienimmat_ind;
	Lcounter = 0;
	for (int i = 0; i < pienimmat.size(); i++)
	{
		if (i > 0)
			edellinen = pienimmat[i - 1];
		for (int j = 0; j < this->BrainScores.size(); j++)
		{
			if (Lcounter == pienimmat.size() || edellinen == pienimmat[i])
				break;
			if (pienimmat[i] == this->BrainScores[j] && j != suurimman_index)
			{
				if (!(std::find(suurimmat_ind.begin(), suurimmat_ind.end(), j) != suurimmat_ind.end()))
				{
					pienimmat_ind.emplace_back(j);
					Lcounter++;
				}
			}
		}
		if (Lcounter == pienimmat.size())
			break;
	}
	
	//loppujen indexit
	std::vector<int> loput_ind;
	bool FinS = false;
	bool FinP = false;
	for (int i = 0; i < this->BrainScores.size(); i++)
	{
		FinS = false;
		FinP = false;
		for (auto x : suurimmat_ind)
		{
			if(i == x)
			{ 
				FinS = true;
				break;
			}
		}
		for (auto y : pienimmat_ind)
		{
			if (i == y)
			{
				FinP = true;
				break;
			}
		}
		if (!FinS && !FinP && i != suurimman_index)
			loput_ind.emplace_back(i);
	}

	//varmistelut koska jostain syysta voi olla vituillaan? wtf
	/*
	while (suurimmat_ind.size() < 16) //varmistelun varmistelut LOL
	{
		std::cout << "bigsize 16 ERR LOL" << std::endl;
		suurimmat_ind.emplace_back(loput_ind[0]);
		loput_ind.erase(loput_ind.begin());
		loput_ind.shrink_to_fit();
	}
	while (pienimmat_ind.size() < 32)
	{
		std::cout << "lesssize 32 ERR LOL" << std::endl;
		pienimmat_ind.emplace_back(loput_ind[0]);
		loput_ind.erase(loput_ind.begin());
		loput_ind.shrink_to_fit();
	}
	*/



	//std::cout << suurimmat_ind.size() << " ";
	//std::cout << pienimmat_ind.size() << " ";
	//std::cout << loput_ind.size() << std::endl;

	//debuggitarkastelua
	/*
	for (int i = 0; i < suurimmat_ind.size();i++)
	{
		for (int j = 0; j < loput_ind.size(); j++)
		{
			if (suurimmat_ind[i] == loput_ind[j] || suurimman_index == loput_ind[j])
			{
				std::cout << "loput in suurimmat" << std::endl;
				loput_ind.erase(loput_ind.begin()+j);
				loput_ind.shrink_to_fit();
			}
		}
	}*/

	for (auto x : suurimmat_ind)
	{
		for (auto y : loput_ind)
		{
			if (x == y || suurimman_index == y)
			{
				std::cout << "suurimmat in loput" << std::endl;
				std::cin.get();
				std::cin.get();
				std::cin.get();
			}
		}
	}
	/*
	for (int i = 0; i < suurimmat_ind.size(); i++)
	{
		for (int j = 0; j < pienimmat_ind.size(); j++)
		{
			if (suurimmat_ind[i] == pienimmat_ind[j] || suurimman_index == pienimmat_ind[j])
			{
				std::cout << "suurimmat in pienimmat" << std::endl;
				pienimmat_ind.erase(pienimmat_ind.begin() + j);
				pienimmat_ind.shrink_to_fit();
			}
		}
	}*/
	
	for (auto x : suurimmat_ind)
	{
		for (auto y : pienimmat_ind)
		{
			if (x == y || suurimman_index == y)
			{
				std::cout << "suurimmat in pienimmat" << std::endl;
				std::cin.get();
				std::cin.get();
				std::cin.get();
			}
		}
	}

	for (auto x : pienimmat_ind)
	{
		for (auto y : loput_ind)
		{
			if (x == y)
			{
				std::cout << "loput in pienimmat" << std::endl;
				std::cin.get();
				std::cin.get();
				std::cin.get();
			}
		}
	}
	
	for (auto x : suurimmat_ind)
	{
		if (x == suurimman_index)
		{
			std::cout << "suurin in suurimmat" << std::endl;
			std::cin.get();
			std::cin.get();
			std::cin.get();
		}
	}
	

	//vain paras pariutuu n maaran suurimpien scorejen kanssa ylikirjoittaen huonoimmat  //huutista 20 prosentin incel generaatiolla
	if ((rand() % 100 + 1) >= 94)
	{
		//std::cout << "INCEL GENERATION" << std::endl;
		for (int i = 0; i < suurimmat_ind.size(); i++)
		{
			for (int j = 0; j < koot.size(); j++)
			{
				for (int k = 0; k < koot[j]; k++)
				{
					if ((rand() % 100 + 1) >= 50)
					{
						this->PelaajaStack[pienimmat_ind[i * 2]].aivo.SetSynapsi(j, k, this->PelaajaStack[suurimmat_ind[i]].aivo.GetSynapsi(j, k));
						this->PelaajaStack[pienimmat_ind[(i * 2) + 1]].aivo.SetSynapsi(j, k, this->PelaajaStack[loput_ind[i]].aivo.GetSynapsi(j, k));
					}
					else
					{
						this->PelaajaStack[pienimmat_ind[(i * 2) + 1]].aivo.SetSynapsi(j, k, this->PelaajaStack[suurimmat_ind[i]].aivo.GetSynapsi(j, k));
						this->PelaajaStack[pienimmat_ind[i * 2]].aivo.SetSynapsi(j, k, this->PelaajaStack[loput_ind[i]].aivo.GetSynapsi(j, k));
					}
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < suurimmat_ind.size(); i++)
		{
			for (int j = 0; j < koot.size(); j++)
			{
				for (int k = 0; k < koot[j]; k++)
				{
					if ((rand() % 100 + 1) >= 80)
					{
						this->PelaajaStack[pienimmat_ind[i * 2]].aivo.SetSynapsi(j, k, this->PelaajaStack[suurimmat_ind[i]].aivo.GetSynapsi(j, k));
						this->PelaajaStack[pienimmat_ind[(i * 2) + 1]].aivo.SetSynapsi(j, k, this->PelaajaStack[suurimman_index].aivo.GetSynapsi(j, k));
					}
					else
					{
						this->PelaajaStack[pienimmat_ind[(i * 2) + 1]].aivo.SetSynapsi(j, k, this->PelaajaStack[suurimmat_ind[i]].aivo.GetSynapsi(j, k));
						this->PelaajaStack[pienimmat_ind[i * 2]].aivo.SetSynapsi(j, k, this->PelaajaStack[suurimman_index].aivo.GetSynapsi(j, k));
					}
				}
			}
		}
	}


	//suurimmat mutta ei paras mutatoituu hieman
	std::thread mutThreads_isoimmat[16]; /////HARDCODED NIIN VITUSTI LOLL
	//for (int l = 0; l < suurimmat_ind.size(); l+=10)
	//{
	int asteroidiko = rand() % 100 + 1;
	
	if (asteroidiko > 98)
	{
		//std::cout << "ASTEROIDI :D" << std::endl;
		for (int i = 0; i < 16; i++)
		{
			mutThreads_isoimmat[i] = std::thread(&evoluutio::mutaatio_thread3, this, koot, suurimmat_ind[i]);
		}
		for (int i = 0; i < 16; i++)
		{
			mutThreads_isoimmat[i].join();
		}
	}

	if (asteroidiko > 98)
	{
		for (int t = 0; t < pienimmat_ind.size(); t += 16)
		{
			for (int i = 0; i < 16; i++)
			{
				mutThreads_isoimmat[i] = std::thread(&evoluutio::mutaatio_thread3, this, koot, pienimmat_ind[t + i]);
			}
			for (int i = 0; i < 16; i++)
			{
				mutThreads_isoimmat[i].join();
			}
		}
	}
	else
	{
		for (int t = 0; t < pienimmat_ind.size(); t += 16)
		{
			for (int i = 0; i < 16; i++)
			{
				mutThreads_isoimmat[i] = std::thread(&evoluutio::mutaatio_thread, this, koot, pienimmat_ind[t + i]);
			}
			for (int i = 0; i < 16; i++)
			{
				mutThreads_isoimmat[i].join();
			}
		}
	}

	//}

	//kaikki loput mutatoituu enemman
	std::thread mutThreads_pianemmat[10];//HARDSTYLEVITT


	if (asteroidiko > 98)
	{
		for (int i = 0; i < loput_ind.size() - 1; i += 10)
		{
			for (int j = 0; j < 10; j++)
			{
				mutThreads_pianemmat[j] = std::thread(&evoluutio::mutaatio_thread3, this, koot, loput_ind[i + j]);
			}
			for (int j = 0; j < 10; j++)
			{
				mutThreads_pianemmat[j].join();
			}
		}
	}
	else
	{
		for (int i = 0; i < loput_ind.size() - 1; i += 10)
		{
			for (int j = 0; j < 10; j++)
			{
				mutThreads_pianemmat[j] = std::thread(&evoluutio::mutaatio_thread2, this, koot, loput_ind[i + j]);
			}
			for (int j = 0; j < 10; j++)
			{
				mutThreads_pianemmat[j].join();
			}
		}
	}
}
void evoluutio::printBrainScores()
{
	for (int i = 0; i < this->BrainScores.size(); i++)
	{
		std::cout << this->BrainScores[i] << " ";
	}
	std::cout << "\n";
}
void evoluutio::printGamelogs()
{
	std::vector<std::string > pelilogi;
	for (int i = 0; i < this->PelaajaStack.size(); i++)
	{
		pelilogi = this->PelaajaStack[i].GetPelilogi();
		for (auto x : pelilogi)
		{
			std::cout << x << " ";
		}
		std::cout << this->BrainScores[i] <<"\n";
	}
	std::cout << "\n";
}
void evoluutio::mutaatio_thread(std::vector<int> koot, int aivoindex)
{
	for (int j = 0; j < koot.size(); j++)
	{
		for (int k = 0; k < koot[j]; k++)
		{
			if ((rand() % 100 + 1) >= 94)
			{
				this->PelaajaStack[aivoindex].aivo.SetSynapsi(j, k, ((rand() % 2000 - 1000) / 1000.0f));
			}
		}
	}
}
void evoluutio::mutaatio_thread2(std::vector<int> koot, int aivoindex)
{
	for (int j = 0; j < koot.size(); j++)
	{
		for (int k = 0; k < koot[j]; k++)
		{
			if ((rand() % 100 + 1) >= 70)
			{
				this->PelaajaStack[aivoindex].aivo.SetSynapsi(j, k, ((rand() % 2000 - 1000) / 1000.0f));
			}
		}
	}
}
void evoluutio::mutaatio_thread3(std::vector<int> koot, int aivoindex)
{
	for (int j = 0; j < koot.size(); j++)
	{
		for (int k = 0; k < koot[j]; k++)
		{
			if ((rand() % 100 + 1) >= 0)
			{
				this->PelaajaStack[aivoindex].aivo.SetSynapsi(j, k, ((rand() % 2000 - 1000) / 1000.0f));
			}
		}
	}
}
bool evoluutio::OnkoNappiAsetettu()
{
	return this->nappi_laudassa;
}
void evoluutio::GeenienRisteytys2()
{
	//parhaat
	static int bestAmo = 40;
	std::vector<int> suurimmat(bestAmo);
	std::partial_sort_copy(std::begin(this->BrainScores), std::end(this->BrainScores), std::begin(suurimmat), std::end(suurimmat), std::greater<int>());

	//parhaiden indexit
	std::vector<int> suurimmat_ind;
	int Lcounter = 0;
	int edellinen = -1000;
	for (int i = 0; i < suurimmat.size(); i++)
	{
		if (i > 0)
			edellinen = suurimmat[i - 1];
		for (int j = 0; j < this->BrainScores.size(); j++)
		{
			if (Lcounter == suurimmat.size() || edellinen == suurimmat[i])
				break;
			if (suurimmat[i] == this->BrainScores[j])
			{
				suurimmat_ind.emplace_back(j);
				Lcounter++;
			}
		}
		if (Lcounter == suurimmat.size())
			break;
	}

	//the paras
	this->biggest_score_index = suurimmat_ind[0];

	//ei parhaiden indexit
	std::vector<int> loput_ind;
	bool FinS = false;
	for (int i = 0; i < this->BrainScores.size(); i++)
	{
		FinS = false;
		for (auto x : suurimmat_ind)
		{
			if (i == x)
			{
				FinS = true;
				break;
			}
		}
		if (!FinS)
			loput_ind.emplace_back(i);
	}


	//varmistelu
	while (suurimmat_ind.size() < bestAmo)
	{
		std::cout << "ind make ERR LOL" << std::endl;
		suurimmat_ind.emplace_back(loput_ind[0]);
		loput_ind.erase(loput_ind.begin());
		loput_ind.shrink_to_fit();
	}
	// KYSYMPAHAN VAAN etta jaako noihin josku samoja indexeja mika sitten threadeissa aiheuttaa MEM ACCES VIOLATIONIN!?? kysympahan vaan...
	//no katotaanpa
	/*
	for (auto x : suurimmat_ind)
	{
		for (auto y : loput_ind)
		{
			if (x == y)// || suurimman_index == y)
			{
				std::cout << y << "j ";
			}
		}
	}
	*/

	std::cout << suurimmat_ind.size() << " ";
	std::cout << loput_ind.size() << std::endl;

	//tahan sit se evoluutio algoritmi LOL

}
void evoluutio::risteytys_thread(std::vector<int> koot, int alkuindex, std::vector<int> suurimmat, std::vector<int> loput)
{
	//paskaa
	int id1 = 0;
	int id2 = 0;
	int jalkelaisen_index = 0;

	for (int i = alkuindex; i < alkuindex + 9; i++)
	{
		id1 = i;
		id2 = i + 1;

		for (int z = 0; z < 4; z++)
		{
			jalkelaisen_index = (i * 4) + z;
			for (int j = 0; j < koot.size(); j++)
			{
				for (int k = 0; k < koot[j]; k++)
				{
					if ((rand() % 100 + 1) >= 50)
					{
						this->PelaajaStack[loput[jalkelaisen_index]].aivo.SetSynapsi(j, k, this->PelaajaStack[suurimmat[id1]].aivo.GetSynapsi(j, k));
					}
					else
					{
						this->PelaajaStack[loput[jalkelaisen_index]].aivo.SetSynapsi(j, k, this->PelaajaStack[suurimmat[id2]].aivo.GetSynapsi(j, k));
					}
				}
			}
		}
	}
}
int evoluutio::gethighestscore()
{
	return this->BrainScores[this->biggest_score_index];
}