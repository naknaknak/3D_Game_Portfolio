#pragma once
#include "HeightMap.h"
#include "Tree.h"

class PlantTree
{
public:
	PlantTree( );
	~PlantTree( );

	void Initialize( );
	void Destroy( );
	void Update( );
	void Render( );

	void RawFileRead_Position(const char * fileName, int bitNum = 8);

	std::vector<Tree*> GetTrees( ) { return trees; }
	inline void SetHeightMap(HeightMap* inputMap) { hm = inputMap; }

private:
	std::vector<D3DXVECTOR3> vRawPosition;
	std::vector<Tree*> trees;

	HeightMap* hm = nullptr;
	Tree* tree_ = nullptr;
	Tree* tree_0 = nullptr;

	const char* RAW_POSITION_DIRECTORY = "./Data/HeightMap/Rawfile_Position/";
	const float hFactor = 0.2f;		//Height Map에 대한 높이 보정 값.


};

