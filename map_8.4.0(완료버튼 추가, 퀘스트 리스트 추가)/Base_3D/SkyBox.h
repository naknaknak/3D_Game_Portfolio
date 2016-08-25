#pragma once
class SkyBox
{
public:
	SkyBox();
	~SkyBox();

	//void Initialize(char * textureFileName, float size);
	void Initialize(char * textureFileName, float fRad, UINT slices = 16, UINT stacks = 8);
	LPD3DXMESH CreateMappedSphere(float fRad, UINT slices, UINT stacks);

	void Destroy();
	void Update();
	void Render();

protected:
	D3DXMATRIXA16 world;
	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);
	//D3DXVECTOR3 direction = D3DXVECTOR3(0, 0, 0);

	static const int TRIANGLE_COUNT = 12;
	TextureTriangle triangles[TRIANGLE_COUNT];

	//
	LPDIRECT3DTEXTURE9 texture = nullptr;
	LPD3DXMESH sphereMesh = nullptr;
	D3DMATERIAL9 material;

	float radius = 128.f;
	int vertexSize = 0;
	int vertexNum_sqrt = 128;
	int vertexIndexNum = 0;

	bool wireMode;
};

