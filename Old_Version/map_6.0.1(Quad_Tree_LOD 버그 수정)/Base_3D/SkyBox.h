#pragma once
class SkyBox
{
public:
	SkyBox();
	~SkyBox();

	void Initialize(char * textureFileName, float size);
	//void Initialize(char * textureFileName, float rad = 128.f, int vertexNumSqrt = 128);
	void Destroy();
	void Update();
	void Render();

protected:
	D3DXMATRIXA16 world;
	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);
	//D3DXVECTOR3 direction = D3DXVECTOR3(0, 0, 0);

	static const int TRIANGLE_COUNT = 12;
	TextureTriangle triangles[TRIANGLE_COUNT];

	LPDIRECT3DTEXTURE9 texture = nullptr;

	float scale = 2.0f;

	//
	float radius = 128.f;
	int vertexSize = 0;
	int vertexNum_sqrt = 128;
	int vertexIndexNum = 0;

	LPDIRECT3DVERTEXBUFFER9 vertexBuffer = nullptr;
	LPDIRECT3DINDEXBUFFER9 indexBuffer = nullptr;


};

