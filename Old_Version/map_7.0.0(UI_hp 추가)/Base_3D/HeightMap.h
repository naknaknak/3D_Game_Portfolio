#pragma once

#include "QuadTree.h"

class HeightMap
{
public:
	HeightMap();
	~HeightMap();

	void Initialize(char* mapFileName, char* textureFileName);
	void Destroy();
	void DebuggingKey( );
	void Render();

	void Debuging( );
	
	// 지정된 조건에 의해 장치 능력을 조사해, 현재의 장치에서의 범프 맵핑의 지원을 검출한다.
	BOOL SupportsBumpMapping( );


	bool GetHeight(float& outHeight, float x, float z); 
	bool GetHeight(D3DXVECTOR3 & pos, float x, float z);
	bool OnTheGround(D3DXVECTOR3 & pos, const D3DXVECTOR3 & p0, const D3DXVECTOR3 & p1, const D3DXVECTOR3 & p2);

	inline DWORD F2DW(FLOAT f) { return *((DWORD*)&f); }

public:
	/// x, z 위치의 정점값을 얻어낸다.
	D3DXVECTOR3 GetVertex(int x, int z)
	{	
		assert(vertex.size( ) > (size_t)(x + z * mapSize));
		return vertex[x + z*mapSize];
	}

	///화면에 지형을 출력한다.
	HRESULT QuadTreeDraw( );

protected:
	const char* HEIGHTMAP_DIRECTORY = "./Data/HeightMap/";

	int mapSize = -1;	//raw파일의 픽셀에 한변의 길이
	int tileCount = -1;	//mapSize-1
	int vertexCount = -1;	//mapSize*mapSize
	int triangleCount = -1;	//tileCount*tileCount*2

	std::vector<D3DXVECTOR3> vertex;
	std::vector<DWORD> indexData;

	LPDIRECT3DVERTEXBUFFER9 vertexBuffer = nullptr;
	LPDIRECT3DINDEXBUFFER9 indexBuffer = nullptr;
	//LPDIRECT3DINDEXBUFFER9 indexBackBuffer = nullptr;

	D3DMATERIAL9 material;
	LPDIRECT3DTEXTURE9 pTexDiffuse = nullptr;
	LPDIRECT3DTEXTURE9 pTexNormal = nullptr;
	
	D3DXMATRIXA16 world;

	//
	QuadTree * m_pQuadTree = nullptr;
	float m_fLODRatio = 0.f;
	bool wireMode = false;
};

