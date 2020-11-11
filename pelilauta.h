#pragma once
#include <iostream>
#include <vector>

class pelilauta
{
private:
	int koko = 10;
public:
	std::string emptymark = "-";
	std::vector<std::vector<std::string>> Lauta;
	pelilauta();
	pelilauta(int k,std::string mark);
	~pelilauta();
	void GetState();
	void Clear();
	void ClearMarks();
	int GetSize();

};

