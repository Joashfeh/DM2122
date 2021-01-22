#include <iostream>
#include <fstream>
#include <map>
#include "LoadTextData.h"
#include <string>

void LoadTextData(int textDataArray[], std::string textDataCSV) {
	std::ifstream data(textDataCSV);
	std::string line;
	int i = 0;

	if (!data.good())
		return;

	while (std::getline(data, line)) {
		textDataArray[i++] = std::stoi(line);
	}
}
