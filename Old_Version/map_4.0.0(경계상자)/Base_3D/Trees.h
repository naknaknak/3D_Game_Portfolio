#pragma once

#include "Obj.h"


class Trees
{
public:
	Trees( );
	~Trees( );

	void Initialize( );
	void Destroy( );
	void Update( );
	void Render( );

	void RawFileRead_Position(const char * fileName, int bitNum = 8);

	inline void SetHeightMap(HeightMap* inputMap) { hm = inputMap; }

private:
	std::vector<D3DXVECTOR3> vRawPosition;
	HeightMap* hm = nullptr;

	const char* RAW_POSITION_DIRECTORY = "./Data/HeightMap/Rawfile_Position/";
	const float hFactor = 0.2f;		//Height Map에 대한 높이 보정 값.

	Obj* tree_ = nullptr;
	Obj* tree_0 = nullptr;

	std::vector<Obj*> trees;

};

