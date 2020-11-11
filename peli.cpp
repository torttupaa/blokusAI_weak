#include <iostream>
#include "peli.h"
#include "pelaaja.h"


peli::peli(int id, pelaaja* p1, pelaaja* p2)
{
	this->PeliID = id;
	this->p1 = p1;
	this->p2 = p2;
	this->lauta = pelilauta(22, "-");
}
peli::~peli()
{

}
void peli::PrintPeliInfo()
{
	std::cout << "PELI " << this->PeliID << std::endl;
	std::cout << "Pelaaja 1" << std::endl;
	this->p1->aivo.PrintInfo();
	//this->p1.aivo.PrintSynapsit();
	std::cout << std::endl;
	std::cout << "Pelaaja 2" << std::endl;
	this->p2->aivo.PrintInfo();
	//this->p2.aivo.PrintSynapsit();
	//this->lauta.GetState();
	std::cout << std::endl;
}
void peli::Pelaa()
{
	bool p1alive = true;
	bool p2alive = true;

	while (p1alive || p2alive)
	{
		p1alive = p1->AIvuoro(&lauta);
		p2alive = p2->AIvuoro(&lauta);
	}
}
void peli::PelaaAiHuman()
{
	bool p1alive = true;
	bool p2alive = true;

	while (p1alive || p2alive)
	{
		p1alive = p1->AIvuoroVisible(&lauta);
		p2alive = p2->vuoro(&lauta);
	}
}
void peli::PelaaAiAi()
{
	bool p1alive = true;
	bool p2alive = true;

	while (p1alive || p2alive)
	{
		p1alive = p1->AIvuoroVisible(&lauta);
		p2alive = p2->AIvuoroVisible(&lauta);
	}
}
std::vector<std::vector<std::string>> peli::GetLauta()
{
	return this->lauta.Lauta;
}
std::vector<std::string> peli::GetP1Log()
{
	return this->p1->GetPelilogi();
}
std::vector<std::string> peli::GetP2Log()
{
	return this->p2->GetPelilogi();
}
void peli::ClearPeli()
{
	this->lauta.ClearMarks();
	this->p1->Reset();
	this->p2->Reset();
}