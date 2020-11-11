#pragma once
#include <vector>
#include <string>

class aivot
{
private:
	int AivoID;
	std::string AivoNimi;
	std::vector<std::vector<float>> synapsit;
	std::vector<int> NeuronsInLayer;
public:
	aivot(int id, bool Uudetsynapsit, std::vector<int> layers, std::string nimi);
	aivot();
	~aivot();
	void PrintInfo();
	void PrintSynapsit();
	float RandomFloat();
	std::string MakeMove(std::vector<float> InputLayer);
	void WriteSynapseFile();
	void SetSynapses(std::vector<std::vector<float>> synat);
	std::vector<std::vector<float>> GetSynapsit();
	float GetSynapsi(int i, int j);
	void SetSynapsi(int i, int j, float val);
};