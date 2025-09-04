#include "MapChipField.h"
#include <map>
#include <fstream>
#include <sstream>
#include <cassert>

MapChipFiled::MapChipFiled(){}
MapChipFiled::~MapChipFiled() {}

namespace {

std::map<std::string, MapchipType> mapChipTable = {
    {"0", MapchipType::NONE },
    {"1", MapchipType::BLOCK},
};

}



void MapChipFiled::Initilize() {}

void MapChipFiled::Update() {}

void MapChipFiled::Draw() {}

void MapChipFiled::ReseMapChipData() 
{ 
	mapChipData_.data.clear();
	mapChipData_.data.resize(kNumBlockVertical);
	for (std::vector<MapchipType>& mapChipDataLine : mapChipData_.data)
	{
		mapChipDataLine.resize(kNumBlockHorizontal);
	}


}

void MapChipFiled::LoadMapChipCsv(const std::string& filePath) 
{ 
	ReseMapChipData();

	//ファイルを開く
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	//マップチップCSV
	std::stringstream mapChipCsv;
	//ファイル内容を文字列ストリームにコピー
	mapChipCsv << file.rdbuf();
	//ファイルを閉じる
	file.close();
	//CSVからマップチップデータを読み込む
	for (uint32_t i = 0; i < kNumBlockVertical; ++i)
	{
		std::string line;
		getline(mapChipCsv, line);

		//1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j)
		{
			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word))
			{
				mapChipData_.data[i][j] = mapChipTable[word];
			}
		}



	}



}

MapchipType MapChipFiled::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) 
{ 
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex)
	{
		return MapchipType::NONE;
	}

	if (yIndex < 0 || kNumBlockVertical - 1 < yIndex)
	{
		return MapchipType::NONE;
	}

	return mapChipData_.data[yIndex][xIndex];

}

Vector3 MapChipFiled::GetMapChipPostionByIndex(uint32_t xIndex, uint32_t yIndex)
{
	return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVertical - 1 - yIndex), 0);
}
