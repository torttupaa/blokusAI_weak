#include <iostream>
#include <vector>
#include "aivot.h"
#include <fstream>


aivot::aivot(int id, bool UudetSynapsit, std::vector<int> layers, std::string nimi)
{
	this->AivoID = id;
	this->AivoNimi = nimi;
	this->NeuronsInLayer = layers;

	if (UudetSynapsit)
	{
		std::vector<std::vector<float>> LayerSynat;
		std::vector<float> Synat;

		for (int i = 0; i < layers.size()-1; i++)
		{
			for (int j = 0; j < layers[i]; j++)
			{
				for (int k = 0; k < layers[i + 1]; k++)
				{
					Synat.emplace_back(RandomFloat());
				}
			}
			LayerSynat.emplace_back(Synat);
			Synat.clear();
			Synat.shrink_to_fit();
		}
		this->synapsit = LayerSynat;
		LayerSynat.clear();
		LayerSynat.shrink_to_fit();
	}
	else
	{
		//lukee omaa nimea vastaavasta tekstitiedostosta synat
	}
}
aivot::aivot()
{

}
aivot::~aivot()
{
	this->synapsit.clear();
	this->synapsit.shrink_to_fit();
}
void aivot::PrintInfo()
{
	std::cout << "Nimi-> " << AivoNimi << "\nID-> " << AivoID << std::endl;
	int layers = NeuronsInLayer.size();
	int neurons = 0;
	long synapses = 0;

	for (int i = 0; i < synapsit.size(); i++)
	{
		neurons += NeuronsInLayer[i];
		synapses += synapsit[i].size();
	}
	std::cout << layers << " layers\n";
	std::cout << neurons+ NeuronsInLayer.back() << " neurons\n";
	std::cout << synapses << " synapses\n";
}
void aivot::PrintSynapsit() 
{
	std::cout << std::endl;
	for (auto x : synapsit)
	{
		for (auto y : x)
		{
			std::cout << y << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
float aivot::RandomFloat()
{
	return (rand() % 2000 - 1000)/1000.0f;
}
std::string aivot::MakeMove(std::vector<float> InputLayer)
{
	//lasketaan output vektori synapseilla

	std::vector<float> CurrentLayer = InputLayer;
	std::vector<float> NextLayer;
	int counter = 0;

	for (int i = 0; i < this->NeuronsInLayer.size()-1; i++)
	{
		NextLayer.assign(this->NeuronsInLayer[i+1],0);
		counter = 0;
		for (int j = 0; j < this->NeuronsInLayer[i]; j++)
		{
			for (int k = 0; k < this->NeuronsInLayer[i + 1]; k++)
			{
				NextLayer[k] += synapsit[i][counter]* CurrentLayer[j];
				counter++;
			}
		}
		CurrentLayer = NextLayer;
		CurrentLayer.shrink_to_fit();

		//joku sigmoidi tai jotain current layerilla?
		

		if (CurrentLayer.size() != 7)
		{
			for (int asd = 0; asd < CurrentLayer.size(); asd++)
			{
				CurrentLayer[asd] = 1 / (1 + exp(-CurrentLayer[asd]));
			}
		}



		NextLayer.clear();
		NextLayer.shrink_to_fit();

	}

	/*
	std::cout << "decision neurons" << std::endl;
	for (auto x : CurrentLayer)
	{
		std::cout << x << " ";
	}
	std::cout << std::endl;
	*/
	

	int biggestIndex = 0;
	for (int i = 0; i < CurrentLayer.size(); i++)
	{
		if (CurrentLayer[biggestIndex] < CurrentLayer[i])
		{
			biggestIndex = i;
		}
	}


	switch (biggestIndex)
	{
	case 0:
		return "r";
		break;
	case 1:
		return "l";
		break;
	case 2:
		return "u";
		break;
	case 3:
		return "d";
		break;
	case 4:
		return "q";
		break;
	case 5:
		return "c";
		break;
	case 6:
		return "p";
		break;
	default:
		return "z";
		break;
	}
}
void aivot::WriteSynapseFile()
{
	std::ofstream geenit;
	geenit.open("geenit\\"+this->AivoNimi + ".txt");
	for (auto x : synapsit)
	{
		for (auto y : x)
		{
			geenit << y << " ";
		}
		geenit << std::endl;
	}
	geenit.close();

}
void aivot::SetSynapses(std::vector<std::vector<float>> synat)
{
	this->synapsit = synat;
}
std::vector<std::vector<float>> aivot::GetSynapsit()
{
	return this->synapsit;
}
float aivot::GetSynapsi(int i, int j)
{
	return this->synapsit[i][j];
}
void aivot::SetSynapsi(int i, int j, float val)
{
	this->synapsit[i][j] = val;
}