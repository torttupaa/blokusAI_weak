#pragma once
#include <iostream>
#include <vector>
class nappula
{
private:
	std::string nimi;
	int maara;
	std::vector<std::vector<int>> kulmat;
public:
	nappula(std::vector<std::vector<int>> k, int amo, std::string nimi);
	~nappula();
	void sub();
	int getmaara();
	std::vector<std::vector<int>> getkulmat();
	void printInfo();
	void rotate90(int rotations);
	void mirror();
	void vectorsNext();
	void setamo(int amo);
};