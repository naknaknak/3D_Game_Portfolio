#pragma once


class MeshViewer
{
public:
	MeshViewer();
	~MeshViewer();

	HRESULT Initialize(char * filename);

	void Destroy();
	void Update();
	void Render();

private:
	LPD3DXMESH pMesh = nullptr;                   // 메시 객체
	D3DMATERIAL9* pMeshMaterials = nullptr;       // 메시에서 사용할 재질
	LPDIRECT3DTEXTURE9*pMeshTextures = nullptr;    // 메시에서 사용할 텍스처
	DWORD dwNumMaterials = 0L;                    // 메시에서 사용중인 재질의 개수

	D3DXMATRIXA16 world;
	
};

