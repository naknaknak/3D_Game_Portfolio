#pragma once
class HeightMap
{
public:
	HeightMap();
	~HeightMap();

	void Initialize(char* mapFileName, char* textureFileName, int bitNum = 8);
	void Destroy();
	void Render();

	bool GetHeight(float& outHeight, float x, float z);
	bool GetHeight(D3DXVECTOR3 & pos, float x, float z);
	bool OnTheGround(D3DXVECTOR3 & pos, const D3DXVECTOR3 & p0, const D3DXVECTOR3 & p1, const D3DXVECTOR3 & p2);

protected:
	const char* HEIGHTMAP_DIRECTORY = "./Data/HeightMap/";

	int mapSize = -1;	//raw파일의 픽셀에 한변의 길이
	int tileCount = -1;	//mapSize-1
	int vertexCount = -1;	//mapSize*mapSize
	int triangleCount = -1;	//tileCount*tileCount*2
	
	size_t vertexSizeCount = -1;

	std::vector<D3DXVECTOR3> vertex;

	LPDIRECT3DVERTEXBUFFER9 vertexBuffer = nullptr;
	LPDIRECT3DINDEXBUFFER9 indexBuffer = nullptr;

	D3DMATERIAL9 material;
	LPDIRECT3DTEXTURE9 texture = nullptr;

	D3DXMATRIXA16 world;
};

