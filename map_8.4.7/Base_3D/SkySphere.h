#pragma once
class SkySphere
{
private:
	LPD3DXMESH m_pSphere = nullptr;
	// ���̴�
	LPD3DXEFFECT m_pTextureMappingShader = nullptr;
	// �ؽ���
	LPDIRECT3DTEXTURE9 m_pEarthDM = nullptr;

	D3DXMATRIXA16 matWorld;
	D3DXMATRIXA16 matView;
	D3DXMATRIXA16 matProjection;

	float gRotationY = 0.f;
	D3DXVECTOR3 vecScale;
	D3DXVECTOR3 vecPosition;
						

public:
	SkySphere();
	~SkySphere();

	void Initialize();
	void Destroy();
	void Update();
	void Render();

};

