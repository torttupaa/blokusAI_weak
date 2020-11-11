#include "nappula.h"
#include <vector>

nappula::nappula(std::vector<std::vector<int>> k, int amo, std::string nimi)
{
	this->kulmat = k;
	this->maara = amo;
	this->nimi = nimi;
}
nappula::~nappula()
{
	kulmat.clear();
}
void nappula::sub()
{
	this->maara--;
}
std::vector<std::vector<int>> nappula::getkulmat()
{
	return kulmat;
}
void nappula::printInfo()
{
	std::cout << this->nimi << " " << this->maara << std::endl;
}
void nappula::rotate90(int rotations)
{
	std::vector<int> uusikulma(2);
	std::vector<std::vector<int>> uudetkulmat;
	for (auto kulma : kulmat)
	{
		for (int i = 0; i < rotations; i++)
		{


			uusikulma[0] = roundf((float)kulma[0] * cos(1.57079633) - (float)kulma[1] * sin(1.57079633));
			uusikulma[1] = roundf((float)kulma[0] * sin(1.57079633) + (float)kulma[1] * cos(1.57079633));
		}

		uudetkulmat.emplace_back(uusikulma);
	}

	this->kulmat = uudetkulmat;
	uudetkulmat.clear();
}
void nappula::mirror()
{

}
void nappula::vectorsNext()
{

}
int nappula::getmaara()
{
	return this->maara;
}
void nappula::setamo(int amo)
{
	this->maara = amo;
}