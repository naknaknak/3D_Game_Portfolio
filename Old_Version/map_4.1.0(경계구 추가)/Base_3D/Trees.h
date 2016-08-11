#pragma once

#include "Obj.h"
#include "Collider_Sphere.h"


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
	std::vector<Obj*> trees;
	std::vector<Collider_Sphere*> vColliderSphere;

	HeightMap* hm = nullptr;
	Obj* tree_ = nullptr;
	Obj* tree_0 = nullptr;

	const char* RAW_POSITION_DIRECTORY = "./Data/HeightMap/Rawfile_Position/";
	const float hFactor = 0.2f;		//Height Map에 대한 높이 보정 값.



};

