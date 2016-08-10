#pragma once
#include "HeightMap.h"
class Raw_NormalTexture :
	public HeightMap::RawFileSection
{
public:
	Raw_NormalTexture( );
	~Raw_NormalTexture( );
	virtual void Initialize(OUT std::vector<D3DXVECTOR3> &vertex, OUT int& mapSize, char* mapFileName, char* textureFileName, int bitNum = 8) override;
	DWORD VectortoRGBA(D3DXVECTOR3 * v, float fHeight);
	virtual void Destroy( ) override;
	virtual void Render( ) override;
	void Update( );
	void Animate( );
private:
	const char* HEIGHTMAP_DIRECTORY = "./Data/HeightMap/";

	//int mapSize = -1;	//raw파일의 픽셀에 한변의 길이
	int tileCount = -1;	//mapSize-1
	int vertexCount = -1;	//mapSize*mapSize
	int triangleCount = -1;	//tileCount*tileCount*2

	int vertexSizeCount = -1;

	std::vector<D3DXVECTOR3> vertex; //

	LPDIRECT3DVERTEXBUFFER9 vertexBuffer = nullptr;
	LPDIRECT3DINDEXBUFFER9 indexBuffer = nullptr;

	D3DMATERIAL9 material;
	LPDIRECT3DTEXTURE9 pTexDiffuse = nullptr;
	LPDIRECT3DTEXTURE9 pTexNormal = nullptr;

	D3DXMATRIXA16 world;
	
	D3DXVECTOR3 vecLight;


};

