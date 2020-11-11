#include "pelaaja.h"
#include "nappula.h"
#include "pelilauta.h"
#include <Windows.h>
#include <thread>
#include <chrono>
#include <cmath>
#include <algorithm>

pelaaja::pelaaja(bool tuplanappulat,std::string sym, std::vector<int> vekki, std::string BrainName, int ID,std::vector<int> BrainComplexity)
{	
	this->aivo = aivot(ID, true, BrainComplexity, BrainName + std::to_string(ID));
	this->alkuAvail = { vekki };
	int amo = 2;
	if (tuplanappulat)
	{
		amo = 2;
	}
	this->symbol = sym;
	this->AvailableCorners = { vekki };

	nappula trekulma({{0,0},{1,0},{0,1} },amo, "trekulma");
	this->napit.emplace_back(trekulma);
	nappula risti({ {0,0},{1,0},{0,1},{-1,0},{0,-1} }, amo, "risti");
	this->napit.emplace_back(risti);
	nappula nelio({ {0,0},{1,0},{0,1},{1,1} }, amo, "nelio");
	this->napit.emplace_back(nelio);
	nappula nelioja1({ {0,0},{1,0},{0,1},{1,1},{0,2} }, amo, "nelioja1");
	this->napit.emplace_back(nelioja1);
	nappula ristipois1({ {0,0},{1,0},{0,1},{-1,0} }, amo, "ristipois1");
	this->napit.emplace_back(ristipois1);
	nappula one({ {0,0} }, amo, "one");
	this->napit.emplace_back(one);
	nappula two({ {0,0},{1,0} }, amo, "two");
	this->napit.emplace_back(two);
	nappula tre({ {0,0},{1,0},{-1,0} }, amo, "tre");
	this->napit.emplace_back(tre);
	nappula four({ {0,0},{1,0},{-1,0},{2,0} }, amo, "four");
	this->napit.emplace_back(four);
	nappula five({ {0,0},{1,0},{2,0},{-1,0},{-2,0} }, amo, "five");
	this->napit.emplace_back(five);
	nappula al({ {0,0},{1,0},{0,1},{2,0} }, amo, "al");
	this->napit.emplace_back(al);
	nappula Lal({ {0,0},{1,0},{0,1},{2,0},{3,0} }, amo, "Lal");
	this->napit.emplace_back(Lal);

}
pelaaja::pelaaja()
{

}
pelaaja::~pelaaja()
{
	napit.clear();
}
bool pelaaja::OnkoVektoriVektorissa(std::vector<std::vector<int>> v1, std::vector<int> v2)
{
	for (auto vec : v1)
	{
		if (vec == v2)
		{
			return true;
		}
	}
	return false;

}
void pelaaja::PrintNapit()
{
	for (auto nappi : napit)
	{
		nappi.printInfo();
	}
}
std::vector<std::vector<int>> pelaaja::ManipNappula(int nappula_index,bool mirror,int nextcorner_hops, int rotations)
{
	if (mirror)
	{
		napit[nappula_index].mirror();
	}
	for (int i = 0; i < nextcorner_hops; i++)
	{
		napit[nappula_index].vectorsNext();
	}
	for (int i = 0; i < rotations; i++)
	{
		napit[nappula_index].rotate90(rotations);
	}
	return napit[nappula_index].getkulmat();
}
bool pelaaja::PlaceNappula(pelilauta* pelilauta, std::vector<int> Corner, std::vector<std::vector<int>> napin_kulmat)
{
	std::vector<std::vector<std::string>> newlauta = pelilauta->Lauta;
	bool avail = false;
	/*
	if (symbol == "*")
	{
		std::cout << "corner kords" << std::endl;
		std::cout << Corner[0] << "," << Corner[1] << std::endl;
		std::cout << "avail kords" << std::endl;
		for (auto av : AvailableCorners)
		{
			std::cout << av[0] << "," << av[1] << std::endl;
		}
		std::cout << "nappi kords" << std::endl;
		for (auto k : napin_kulmat)
		{
			std::cout << k[0]<< ","<< k[1] << std::endl;
		}
	}*/
	for (auto k : napin_kulmat)
	{
		if (OnkoVektoriVektorissa(this->AvailableCorners, { Corner[0] + k[0],Corner[1] + k[1] }))
		{
			avail = true;
		}
	}
	
	if (avail)
	{
		for (auto kulma : napin_kulmat)
		{
			if (newlauta[Corner[0] + kulma[0]][Corner[1] + kulma[1]] != pelilauta->emptymark)
			{
				//std::cout << "# false" << std::endl;
				return false;

			}

			newlauta[Corner[0] + kulma[0]][Corner[1] + kulma[1]] = symbol;
			if (OnkoVektoriVektorissa(this->Unavailable, { Corner[0] + kulma[0],Corner[1] + kulma[1] }))
			{
				//std::cout << Corner[0] + kulma[0] << "," << Corner[1] + kulma[1] << std::endl;
				//std::cout << "Unavail false" << std::endl;
				return false;
			}

		}

		//*lauta = newlauta;
		pelilauta->Lauta = newlauta;


		//available cords&& Unavailable
		this->AvailableCorners.clear();
		this->Unavailable.clear();
		for (int i = 0; i < newlauta.size(); i++)
		{
			for (int j = 0; j < newlauta.size(); j++)
			{
				if (newlauta[i][j] == symbol)
				{

					if (newlauta[i + 1][j + 1] == pelilauta->emptymark)
					{
						if ((newlauta[i][j + 1] != symbol) && (newlauta[i + 2][j + 1] != symbol) && (newlauta[i + 1][j + 2] != symbol) && (newlauta[i + 1][j] != symbol))
						{
							newlauta[i + 1][j + 1] = "P";
							this->AvailableCorners.emplace_back(std::vector<int>{ (i + 1), (j + 1) });
						}
					}
					if (newlauta[i - 1][j - 1] == pelilauta->emptymark)
					{
						if ((newlauta[i][j - 1] != symbol) && (newlauta[i - 2][j - 1] != symbol) && (newlauta[i - 1][j - 2] != symbol) && (newlauta[i - 1][j] != symbol))
						{
							newlauta[i - 1][j - 1] = "P";
							this->AvailableCorners.emplace_back(std::vector<int>{ (i - 1), (j - 1) });
						}
					}
					if (newlauta[i - 1][j + 1] == pelilauta->emptymark)
					{
						if ((newlauta[i][j + 1] != symbol) && (newlauta[i - 2][j + 1] != symbol) && (newlauta[i - 1][j + 2] != symbol) && (newlauta[i - 1][j] != symbol))
						{
							newlauta[i - 1][j + 1] = "P";
							this->AvailableCorners.emplace_back(std::vector<int>{ (i - 1), (j + 1) });
						}
					}
					if (newlauta[i + 1][j - 1] == pelilauta->emptymark)
					{
						if ((newlauta[i][j - 1] != symbol) && (newlauta[i + 2][j - 1] != symbol) && (newlauta[i + 1][j - 2] != symbol) && (newlauta[i + 1][j] != symbol))
						{
							newlauta[i + 1][j - 1] = "P";
							this->AvailableCorners.emplace_back(std::vector<int>{ (i + 1), (j - 1) });
						}
					}
					if (newlauta[i][j + 1] == pelilauta->emptymark)
					{
						newlauta[i][j + 1] = "*";
						this->Unavailable.emplace_back(std::vector<int>{ (i), (j + 1) });
					}
					if (newlauta[i][j - 1] == pelilauta->emptymark)
					{
						newlauta[i][j - 1] = "*";
						this->Unavailable.emplace_back(std::vector<int>{ (i), (j - 1) });
					}
					if (newlauta[i + 1][j] == pelilauta->emptymark)
					{
						newlauta[i + 1][j] = "*";
						this->Unavailable.emplace_back(std::vector<int>{ (i + 1), (j) });
					}
					if (newlauta[i - 1][j] == pelilauta->emptymark)
					{
						newlauta[i - 1][j] = "*";
						this->Unavailable.emplace_back(std::vector<int>{ (i - 1), (j) });
					}
				}
			}
		}

		//pelilauta->Lauta = newlauta;
		
		newlauta.clear();
		newlauta.shrink_to_fit();
		return true;
	}
	else { return false; }
}
bool pelaaja::vuoro(pelilauta* pelilauta)
{
	if (!this->elossa)
	{
		return false;
	}
	int nappi_index = 0;
	std::vector<int> N_index_jaljella;
	for (int i = 0; i < napit.size(); i++)
	{
		if (napit[i].getmaara() > 0)
		{
			N_index_jaljella.emplace_back(i);
		}
	}
	if (N_index_jaljella.size() == 0)
	{
		this->elossa = false;
		return false;
	}


	bool valmis = false;
	std::string valinta = "";
	std::vector<std::vector<std::string>> newlauta = pelilauta->Lauta;
	std::vector<std::vector<int>> nextckulmat;
	std::vector<std::vector<int>> ckulmat;
	std::vector<int> initpos = { pelilauta->GetSize()/2-2,pelilauta->GetSize() / 2-2 };


	//init nappula laudalle
	ckulmat = napit[N_index_jaljella[nappi_index]].getkulmat();
	bool illegal = false;
	bool canmakemove = true;

	int movecount = 0;

	while (!valmis)
	{
		//shownappulalaudalla
		newlauta = pelilauta->Lauta;
		canmakemove = true;

		
		for (auto kulma : ckulmat)
		{
			newlauta[initpos[0] + kulma[0]][initpos[1] + kulma[1]] = symbol;
		}

		std::cout << symbol << " vuoro" << std::endl;
		std::cout << movecount << " movesmade" << std::endl;
		for (auto x : newlauta)
		{
			for (auto y : x)
			{
				std::cout << y << " ";
			}
			std::cout << std::endl;
		}
		if (illegal)
		{
			std::cout << "illegal move" << std::endl;
		}
		std::cout << "INPUT?: " << std::endl;
		valinta = std::cin.get();
		if (valinta == "r")
		{
			for (auto kulma : ckulmat)
			{
				if ((initpos[1] + kulma[1] > pelilauta->GetSize() - 3))
				{
					canmakemove = false;
				}
			}
			if (canmakemove)
			{
				initpos[1]++;
			}
		}
		else if (valinta == "l")
		{
			for (auto kulma : ckulmat)
			{
				if ((initpos[1] + kulma[1] < 2))
				{
					canmakemove = false;
				}
			}
			if (canmakemove)
			{
				initpos[1]--;
			}
		}
		else if (valinta == "u")
		{
			for (auto kulma : ckulmat)
			{
				if ((initpos[0] + kulma[0] < 2))
				{
					canmakemove = false;
				}
			}
			if (canmakemove)
			{
				initpos[0]--;
			}
		}
		else if (valinta == "d")
		{
			for (auto kulma : ckulmat)
			{
				if ((initpos[0] + kulma[0] > pelilauta->GetSize() - 3))
				{
					canmakemove = false;
				}
			}
			if (canmakemove)
			{
				initpos[0]++;
			}
		}
		else if (valinta == "q")
		{

			napit[N_index_jaljella[nappi_index]].rotate90(1);
			nextckulmat = napit[N_index_jaljella[nappi_index]].getkulmat();
			for (auto kulma : nextckulmat)
			{
				if ((initpos[0] + kulma[0] < 2) || (initpos[0] + kulma[0] > pelilauta->GetSize() - 3) || (initpos[1] + kulma[1] < 2) || (initpos[1] + kulma[1] > pelilauta->GetSize() - 3))
				{
					canmakemove = false;
				}
			}
			if (canmakemove)
			{
				ckulmat = nextckulmat;
			}
		}
		else if (valinta == "c")
		{
			nappi_index++;
			if (nappi_index > N_index_jaljella.size()-1)
			{
				nappi_index = 0;
			}
			nextckulmat = napit[N_index_jaljella[nappi_index]].getkulmat();
			for (auto kulma : nextckulmat)
			{
				if ((initpos[0] + kulma[0] < 2) || (initpos[0] + kulma[0] > pelilauta->GetSize() - 3) || (initpos[1] + kulma[1] < 2) || (initpos[1] + kulma[1] > pelilauta->GetSize() - 3))
				{
					canmakemove = false;
				}
			}
			if (canmakemove)
			{
				ckulmat = nextckulmat;
			}
		}
		else if (valinta == "p")
		{
			if (PlaceNappula(pelilauta, initpos, ckulmat))
			{
				initpos = { pelilauta->GetSize() / 2-2,pelilauta->GetSize() / 2-2 };
				valmis = true;
				pelilogi.emplace_back(valinta);
				pelilogi.emplace_back("TurnSuccess");
				napit[N_index_jaljella[nappi_index]].sub();
				return true;
			}
			else { illegal = true; }
		}
		else if (valinta == "z")
		{
			this->elossa = false;
			pelilogi.emplace_back(valinta);
			return false;
		}

		if (valinta != "\n")
		{
			pelilogi.emplace_back(valinta);
			movecount++;
		}
		if (movecount > 30)
		{
			this->elossa = false;
			pelilogi.emplace_back("MoveCount");
			return false;
		}

		pelilauta->Clear();
	}
}
std::vector<std::string> pelaaja::GetPelilogi()
{
	return this->pelilogi;
}
void pelaaja::Reset()
{
	this->AvailableCorners.clear();
	this->AvailableCorners.shrink_to_fit();
	this->AvailableCorners = this->alkuAvail;

	this->Unavailable.clear();
	this->Unavailable.shrink_to_fit();
	this->Unavailable = { {0,0} };

	this->pelilogi.clear();
	this->pelilogi.shrink_to_fit();

	this->elossa = true;

	for (int i = 0; i < napit.size(); i++)
	{
		this->napit[i].setamo(2);
	}

}
bool pelaaja::AIvuoro(pelilauta* pelilauta)
{

	if (!this->elossa)
	{
		return false;
	}


	bool valmis = false;
	std::string valinta = "";
	std::vector<std::vector<std::string>> newlauta = pelilauta->Lauta;
	std::vector<std::vector<int>> nextckulmat;
	std::vector<std::vector<int>> ckulmat;
	std::vector<int> initpos = { pelilauta->GetSize() / 2 - 2,pelilauta->GetSize() / 2 - 2 };

	

	//init nappula laudalle
	std::vector<std::vector<int>> initposses = { initpos };
	int nappi_index = 0;
	std::vector<int> N_index_jaljella;
	for (int i = 0; i < this->napit.size(); i++)
	{
		if (this->napit[i].getmaara() > 0)
		{
			N_index_jaljella.emplace_back(i);
		}
	}
	if (N_index_jaljella.size() == 0)
	{
		this->elossa = false;
		pelilogi.emplace_back("MC");

		//calc distancet avail spotteihin
		float distanssi = 99.9f;
		float distanssi_min = 99.9f;
		int initpossecount = 0;
		std::vector<std::vector<int>> lapikaydyt;
		for (auto x : this->AvailableCorners)
		{
			//x tassakohtaa avail vec initpos palikan sijainti
			distanssi = sqrt(pow((initpos[0] - x[0]), 2) + pow((initpos[1] - x[1]), 2));
			if (distanssi < distanssi_min)
			{
				distanssi_min = distanssi;
			}
		}

		for (int i = 0; i < initposses.size(); i++)
		{
			if (!(std::find(lapikaydyt.begin(), lapikaydyt.end(), initposses[i]) != lapikaydyt.end()))
			{
				for (int j = 0; j < initposses.size(); j++)
				{
					if ((initposses[i] == initposses[j]) && (i != j))
					{
						initpossecount -= 2;
					}
				}
				lapikaydyt.emplace_back(initposses[i]);
			}
		}
		pelilogi.emplace_back(std::to_string(initpossecount));
		pelilogi.emplace_back(std::to_string(5 - (int(round(distanssi_min)))));
		return false;
	}


	ckulmat = this->napit[N_index_jaljella[nappi_index]].getkulmat();
	bool illegal = false;
	bool canmakemove = true;


	int movecount = 0;

	while (!valmis)
	{
		//shownappulalaudalla
		newlauta = pelilauta->Lauta;
		canmakemove = true;


		for (auto kulma : ckulmat)
		{
			newlauta[initpos[0] + kulma[0]][initpos[1] + kulma[1]] = "N";
		}


		//vision input layer
		this->InputLayer.assign(100, 0.0f);
		std::vector<std::vector<int>> reseptori_vektorit = { {1,0},{2,1},{1,1},{1,2},{0,1},{-1,2},{-1,1},{-2,1},{-1,0},{-2,-1},{-1,-1},{-1,-2},{0,-1},{1,-2},{1,-1},{2,-1} };


		//palikan osat lapi
		int ckulmacounter = 0;
		for (auto kulma : ckulmat)
		{
			//onko osa unavail -> kaikki -1
			if ((this->OnkoVektoriVektorissa(this->Unavailable, { initpos[0] + kulma[0],initpos[1] + kulma[1] })) || (pelilauta->Lauta[initpos[0] + kulma[0]][initpos[1] + kulma[1]] == "X") || (pelilauta->Lauta[initpos[0] + kulma[0]][initpos[1] + kulma[1]] == "0"))
			{
				for (int i = 0; i < 16; i++)
				{
					this->InputLayer[ckulmacounter + i] = -1.0f;
				}
			}
			//onko osa avail -> kaikki 1
			else if (this->OnkoVektoriVektorissa(this->AvailableCorners, { initpos[0] + kulma[0],initpos[1] + kulma[1] }))
			{
				for (int i = 0; i < 16; i++)
				{
					this->InputLayer[ckulmacounter + i] = 1.0f;
				}
			}
			else
			{
				//vektorit osasta availableihin
				for (int i = 0; i < this->AvailableCorners.size(); i++)
				{
					std::vector<int> vektori_to_avail;
					float lahin_kulma = 300.0f;
					float doproduct = 0;
					int lahimman_index = 0;
					vektori_to_avail = { initpos[0] + kulma[0] - this->AvailableCorners[i][0], initpos[1] + kulma[1] - this->AvailableCorners[i][1] };
					//verrataan vektoreiden suuntia reseptoreiden suuntiin
					for (int j = 0;j < reseptori_vektorit.size(); j++)
					{
						//jos vec kulma < lahin kulma -> lahin_kulma = vec kulma -> lahin_index = j
						doproduct = acos((float)(vektori_to_avail[0] * reseptori_vektorit[j][0] + vektori_to_avail[1] * reseptori_vektorit[j][1]) / (float)((sqrt(pow(vektori_to_avail[0], 2) + pow(vektori_to_avail[1], 2))) * (sqrt(pow(reseptori_vektorit[j][0], 2) + pow(reseptori_vektorit[j][1], 2)))));
						if (doproduct < lahin_kulma)
						{
							lahin_kulma = doproduct;
							lahimman_index = j;
						}
					}
					this->InputLayer[ckulmacounter + lahimman_index] = 1.0f;
				}

			}

			//block 4 seinan etaisyydet tahanLOL oikeevase,ylosala
			for (int i = 0; i < 2; i++)
			{
				float joku = ((float)(initpos[i] + kulma[i])) / 22.0f;
				float joku2 = (22.0f-((float)(initpos[i] + kulma[i]))) / 22.0f;
				this->InputLayer[ckulmacounter + 16 + (i*2)] = joku;
				this->InputLayer[ckulmacounter + 16 + ((i * 2)+1)] = joku2;
			}



			ckulmacounter+=20;
		}


		/*
		* old input layer ONE FIELD len 400
		//inputlayerbuild
		//available spots
		for (int o = 1; o < pelilauta->GetSize() - 1; o++)
		{
			for (int p = 1; p < pelilauta->GetSize() - 1; p++)
			{
				if ((this->OnkoVektoriVektorissa(this->Unavailable, { o,p })) || (newlauta[o][p] == "0") || (newlauta[o][p] == "X"))
				{
					this->InputLayer.emplace_back(0.0f);
				}
				else if (this->OnkoVektoriVektorissa(this->AvailableCorners, { o,p }))
				{
					this->InputLayer.emplace_back(0.5f);
				}
				else
				{
					this->InputLayer.emplace_back(0.1f);
				}
			}
		}
		//asettamaton nappula
		int Layer_counter = 0;
		for (int o = 1; o < pelilauta->GetSize() - 1; o++)
		{
			for (int p = 1; p < pelilauta->GetSize() - 1; p++)
			{
				if (newlauta[o][p] == "N")
				{
					if (this->InputLayer[Layer_counter] == 0.0f)
					{
						this->InputLayer[Layer_counter] -= 0.5;
					}
					else
					{
						this->InputLayer[Layer_counter] += 0.5;
					}
				}
				Layer_counter++;
			}
		}*/



		//botti paattaa mita teha
		valinta = this->aivo.MakeMove(InputLayer);

		//valinta = "r";
		//liikkuminen laudalla
		this->InputLayer.clear();
		if (valinta == "r")
		{
			for (auto kulma : ckulmat)
			{
				if ((initpos[1] + kulma[1] > pelilauta->GetSize() - 3))
				{
					pelilogi.emplace_back("CM");
					canmakemove = false;
				}
			}
			if (canmakemove)
			{
				initpos[1]++;
				initposses.emplace_back(initpos);
			}
		}
		else if (valinta == "l")
		{
			for (auto kulma : ckulmat)
			{
				if ((initpos[1] + kulma[1] < 2))
				{
					pelilogi.emplace_back("CM");
					canmakemove = false;
				}
			}
			if (canmakemove)
			{
				initpos[1]--;
				initposses.emplace_back(initpos);
			}
		}
		else if (valinta == "u")
		{
			for (auto kulma : ckulmat)
			{
				if ((initpos[0] + kulma[0] < 2))
				{
					pelilogi.emplace_back("CM");
					canmakemove = false;
				}
			}
			if (canmakemove)
			{
				initpos[0]--;
				initposses.emplace_back(initpos);
			}
		}
		else if (valinta == "d")
		{
			for (auto kulma : ckulmat)
			{
				if ((initpos[0] + kulma[0] > pelilauta->GetSize() - 3))
				{
					pelilogi.emplace_back("CM");
					canmakemove = false;
				}
			}
			if (canmakemove)
			{
				initpos[0]++;
				initposses.emplace_back(initpos);
			}
		}
		else if (valinta == "q")
		{

			this->napit[N_index_jaljella[nappi_index]].rotate90(1);
			nextckulmat = this->napit[N_index_jaljella[nappi_index]].getkulmat();
			for (auto kulma : nextckulmat)
			{
				if ((initpos[0] + kulma[0] < 2) || (initpos[0] + kulma[0] > pelilauta->GetSize() - 3) || (initpos[1] + kulma[1] < 2) || (initpos[1] + kulma[1] > pelilauta->GetSize() - 3))
				{
					pelilogi.emplace_back("CR");
					canmakemove = false;
				}
			}
			if (canmakemove)
			{
				ckulmat = nextckulmat;
			}
		}
		else if (valinta == "c")
		{
			nappi_index++;
			if (nappi_index > N_index_jaljella.size() - 1)
			{
				nappi_index = 0;
			}
			nextckulmat = this->napit[N_index_jaljella[nappi_index]].getkulmat();
			for (auto kulma : nextckulmat)
			{
				if ((initpos[0] + kulma[0] < 2) || (initpos[0] + kulma[0] > pelilauta->GetSize() - 3) || (initpos[1] + kulma[1] < 2) || (initpos[1] + kulma[1] > pelilauta->GetSize() - 3))
				{
					pelilogi.emplace_back("CC");
					canmakemove = false;
				}
			}
			if (canmakemove)
			{
				ckulmat = nextckulmat;
			}
		}
		else if (valinta == "p")
		{
			if (PlaceNappula(pelilauta, initpos, ckulmat))
			{
				initpos = { pelilauta->GetSize() / 2 - 2,pelilauta->GetSize() / 2 - 2 };
				valmis = true;
				pelilogi.emplace_back(valinta);
				pelilogi.emplace_back("TurnSuccess");
				//pelilauta->Clear();
				this->napit[N_index_jaljella[nappi_index]].sub();
				return true;
			}
			else 
			{
				//pelilogi.emplace_back("CP");
				illegal = true;
			}
		}
		else if (valinta == "z")
		{
			this->elossa = false;
			pelilogi.emplace_back(valinta);
			pelilauta->Clear();
			return false;
		}
		pelilogi.emplace_back(valinta);
		movecount++;
		if (movecount > 22)
		{
			this->elossa = false;
			pelilogi.emplace_back("MC");

			//calc distancet avail spotteihin
			float distanssi = 99.9f;
			float distanssi_min = 99.9f;
			int initpossecount = 0;
			std::vector<std::vector<int>> lapikaydyt;
			for (auto x : this->AvailableCorners)
			{
				//x tassakohtaa avail vec initpos palikan sijainti
				distanssi = sqrt(pow((initpos[0] - x[0]),2) + pow((initpos[1] - x[1]),2));
				if (distanssi < distanssi_min)
				{
					distanssi_min = distanssi;
				}
			}

			for (int i = 0; i < initposses.size(); i++)
			{
				if (!(std::find(lapikaydyt.begin(), lapikaydyt.end(), initposses[i]) != lapikaydyt.end()))
				{
					for (int j = 0; j < initposses.size(); j++)
					{
						if ((initposses[i] == initposses[j]) && (i != j))
						{
							initpossecount-=2;
						}
					}
					lapikaydyt.emplace_back(initposses[i]);
				}
			}
			pelilogi.emplace_back(std::to_string(initpossecount));
			pelilogi.emplace_back(std::to_string(5 - (int(round(distanssi_min)))));
			return false;
		}

		//pelilauta->Clear(); //taa itseasiassa vaa pyyhkii nayton LOL clearmarks() resetoi ite laudan
	}
}
bool pelaaja::AIvuoroVisible(pelilauta* pelilauta)
{
	if (!this->elossa)
	{
		return false;
	}


	bool valmis = false;
	std::string valinta = "";
	std::vector<std::vector<std::string>> newlauta = pelilauta->Lauta;
	std::vector<std::vector<int>> nextckulmat;
	std::vector<std::vector<int>> ckulmat;
	std::vector<int> initpos = { pelilauta->GetSize() / 2 - 2,pelilauta->GetSize() / 2 - 2 };


	//init nappula laudalle
	std::vector<std::vector<int>> initposses = { initpos };
	int nappi_index = 0;
	std::vector<int> N_index_jaljella;
	for (int i = 0; i < this->napit.size(); i++)
	{
		if (this->napit[i].getmaara() > 0)
		{
			N_index_jaljella.emplace_back(i);
		}
	}
	if (N_index_jaljella.size() == 0)
	{
		this->elossa = false;
		pelilogi.emplace_back("MC");

		//calc distancet avail spotteihin
		float distanssi = 99.9f;
		float distanssi_min = 99.9f;
		int initpossecount = 0;
		std::vector<std::vector<int>> lapikaydyt;
		for (auto x : this->AvailableCorners)
		{
			//x tassakohtaa avail vec initpos palikan sijainti
			distanssi = sqrt(pow((initpos[0] - x[0]), 2) + pow((initpos[1] - x[1]), 2));
			if (distanssi < distanssi_min)
			{
				distanssi_min = distanssi;
			}
		}

		for (int i = 0; i < initposses.size(); i++)
		{
			if (!(std::find(lapikaydyt.begin(), lapikaydyt.end(), initposses[i]) != lapikaydyt.end()))
			{
				for (int j = 0; j < initposses.size(); j++)
				{
					if ((initposses[i] == initposses[j]) && (i != j))
					{
						initpossecount -= 2;
					}
				}
				lapikaydyt.emplace_back(initposses[i]);
			}
		}
		pelilogi.emplace_back(std::to_string(initpossecount));
		pelilogi.emplace_back(std::to_string(5 - (int(round(distanssi_min)))));
		return false;
	}



	ckulmat = napit[N_index_jaljella[nappi_index]].getkulmat();
	bool illegal = false;
	bool canmakemove = true;

	int movecount = 0;

	while (!valmis)
	{
		//shownappulalaudalla
		newlauta = pelilauta->Lauta;
		canmakemove = true;


		for (auto kulma : ckulmat)
		{
			newlauta[initpos[0] + kulma[0]][initpos[1] + kulma[1]] = "N";
		}

		std::cout << movecount << " movesmade" << std::endl;
		for (auto x : newlauta)
		{
			for (auto y : x)
			{
				std::cout << y << " ";
			}
			std::cout << std::endl;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		//vision input layer
		this->InputLayer.assign(100, 0.0f);
		std::vector<std::vector<int>> reseptori_vektorit = { {1,0},{2,1},{1,1},{1,2},{0,1},{-1,2},{-1,1},{-2,1},{-1,0},{-2,-1},{-1,-1},{-1,-2},{0,-1},{1,-2},{1,-1},{2,-1} };


		//palikan osat lapi
		int ckulmacounter = 0;
		for (auto kulma : ckulmat)
		{
			//onko osa unavail -> kaikki -1
			if ((this->OnkoVektoriVektorissa(this->Unavailable, { initpos[0] + kulma[0],initpos[1] + kulma[1] })) || (pelilauta->Lauta[initpos[0] + kulma[0]][initpos[1] + kulma[1]] == "X") || (pelilauta->Lauta[initpos[0] + kulma[0]][initpos[1] + kulma[1]] == "0"))
			{
				for (int i = 0; i < 16; i++)
				{
					this->InputLayer[ckulmacounter + i] = -1.0f;
				}
			}
			//onko osa avail -> kaikki 1
			else if (this->OnkoVektoriVektorissa(this->AvailableCorners, { initpos[0] + kulma[0],initpos[1] + kulma[1] }))
			{
				for (int i = 0; i < 16; i++)
				{
					this->InputLayer[ckulmacounter + i] = 1.0f;
				}
			}
			else
			{
				//vektorit osasta availableihin
				for (int i = 0; i < this->AvailableCorners.size(); i++)
				{
					std::vector<int> vektori_to_avail;
					float lahin_kulma = 300.0f;
					float doproduct = 0;
					int lahimman_index = 0;
					vektori_to_avail = { initpos[0] + kulma[0] - this->AvailableCorners[i][0], initpos[1] + kulma[1] - this->AvailableCorners[i][1] };
					//verrataan vektoreiden suuntia reseptoreiden suuntiin
					for (int j = 0; j < reseptori_vektorit.size(); j++)
					{
						//jos vec kulma < lahin kulma -> lahin_kulma = vec kulma -> lahin_index = j
						doproduct = acos((float)(vektori_to_avail[0] * reseptori_vektorit[j][0] + vektori_to_avail[1] * reseptori_vektorit[j][1]) / (float)((sqrt(pow(vektori_to_avail[0], 2) + pow(vektori_to_avail[1], 2))) * (sqrt(pow(reseptori_vektorit[j][0], 2) + pow(reseptori_vektorit[j][1], 2)))));
						if (doproduct < lahin_kulma)
						{
							lahin_kulma = doproduct;
							lahimman_index = j;
						}
					}
					this->InputLayer[ckulmacounter + lahimman_index] = 1.0f;
				}

			}

			//block 4 seinan etaisyydet tahanLOL oikeevase,ylosala
			for (int i = 0; i < 2; i++)
			{
				float joku = ((float)(initpos[i] + kulma[i])) / 22.0f;
				float joku2 = (22.0f - ((float)(initpos[i] + kulma[i]))) / 22.0f;
				this->InputLayer[ckulmacounter + 16 + (i * 2)] = joku;
				this->InputLayer[ckulmacounter + 16 + ((i * 2) + 1)] = joku2;
			}



			ckulmacounter += 20;
		}

		/*
		//inputlayerbuild
		//available spots
		this->InputLayer.clear();
		for (int o = 1; o < pelilauta->GetSize() - 1; o++)
		{
			for (int p = 1; p < pelilauta->GetSize() - 1; p++)
			{
				if ((this->OnkoVektoriVektorissa(this->Unavailable, { o,p })) || (newlauta[o][p] == "0") || (newlauta[o][p] == "X"))
				{
					this->InputLayer.emplace_back(0.0f);
				}
				else if (this->OnkoVektoriVektorissa(this->AvailableCorners, { o,p }))
				{
					this->InputLayer.emplace_back(0.4f);
				}
				else
				{
					this->InputLayer.emplace_back(0.1f);
				}
			}
		}
		//asettamaton nappula
		int Layer_counter = 0;
		for (int o = 1; o < pelilauta->GetSize() - 1; o++)
		{
			for (int p = 1; p < pelilauta->GetSize() - 1; p++)
			{
				if (newlauta[o][p] == "N")
				{
					this->InputLayer[Layer_counter] += 0.6;
				}
				Layer_counter++;
			}
		}*/

		valinta = this->aivo.MakeMove(InputLayer);
		//std::cout << valinta << std::endl;
		//std::cin.get();

		this->InputLayer.clear();
		if (valinta == "r")
		{
			for (auto kulma : ckulmat)
			{
				if ((initpos[1] + kulma[1] > pelilauta->GetSize() - 3))
				{
					pelilogi.emplace_back("CM");
					canmakemove = false;
				}
			}
			if (canmakemove)
			{
				initpos[1]++;
			}
		}
		else if (valinta == "l")
		{
			for (auto kulma : ckulmat)
			{
				if ((initpos[1] + kulma[1] < 2))
				{
					pelilogi.emplace_back("CM");
					canmakemove = false;
				}
			}
			if (canmakemove)
			{
				initpos[1]--;
			}
		}
		else if (valinta == "u")
		{
			for (auto kulma : ckulmat)
			{
				if ((initpos[0] + kulma[0] < 2))
				{
					pelilogi.emplace_back("CM");
					canmakemove = false;
				}
			}
			if (canmakemove)
			{
				initpos[0]--;
			}
		}
		else if (valinta == "d")
		{
			for (auto kulma : ckulmat)
			{
				if ((initpos[0] + kulma[0] > pelilauta->GetSize() - 3))
				{
					pelilogi.emplace_back("CM");
					canmakemove = false;
				}
			}
			if (canmakemove)
			{
				initpos[0]++;
			}
		}
		else if (valinta == "q")
		{

			napit[N_index_jaljella[nappi_index]].rotate90(1);
			nextckulmat = napit[N_index_jaljella[nappi_index]].getkulmat();
			for (auto kulma : nextckulmat)
			{
				if ((initpos[0] + kulma[0] < 2) || (initpos[0] + kulma[0] > pelilauta->GetSize() - 3) || (initpos[1] + kulma[1] < 2) || (initpos[1] + kulma[1] > pelilauta->GetSize() - 3))
				{
					pelilogi.emplace_back("CR");
					canmakemove = false;
				}
			}
			if (canmakemove)
			{
				ckulmat = nextckulmat;
			}
		}
		else if (valinta == "c")
		{
			nappi_index++;
			if (nappi_index > N_index_jaljella.size() - 1)
			{
				nappi_index = 0;
			}
			nextckulmat = napit[N_index_jaljella[nappi_index]].getkulmat();
			for (auto kulma : nextckulmat)
			{
				if ((initpos[0] + kulma[0] < 2) || (initpos[0] + kulma[0] > pelilauta->GetSize() - 3) || (initpos[1] + kulma[1] < 2) || (initpos[1] + kulma[1] > pelilauta->GetSize() - 3))
				{
					pelilogi.emplace_back("CC");
					canmakemove = false;
				}
			}
			if (canmakemove)
			{
				ckulmat = nextckulmat;
			}
		}
		else if (valinta == "p")
		{
			if (PlaceNappula(pelilauta, initpos, ckulmat))
			{
				initpos = { pelilauta->GetSize() / 2 - 2,pelilauta->GetSize() / 2 - 2 };
				valmis = true;
				pelilogi.emplace_back(valinta);
				pelilogi.emplace_back("TurnSuccess");
				napit[N_index_jaljella[nappi_index]].sub();
				pelilauta->Clear();
				return true;
			}
			else
			{
				//pelilogi.emplace_back("CP");
				illegal = true;
			}
		}
		else if (valinta == "z")
		{
			this->elossa = false;
			pelilogi.emplace_back(valinta);
			pelilauta->Clear();
			return false;
		}
		if (valinta != "\n")
		{
			pelilogi.emplace_back(valinta);
			movecount++;
		}
		if (movecount > 30)
		{
			this->elossa = false;
			pelilogi.emplace_back("MC");
			std::cin.get();
			pelilauta->Clear();
			return false;
		}

		pelilauta->Clear(); //taa itseasiassa vaa pyyhkii nayton LOL clearmarks() resetoi ite laudan
	}
}