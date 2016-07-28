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
	LPD3DXMESH pMesh = nullptr;                   // �޽� ��ü
	D3DMATERIAL9* pMeshMaterials = nullptr;       // �޽ÿ��� ����� ����
	LPDIRECT3DTEXTURE9*pMeshTextures = nullptr;    // �޽ÿ��� ����� �ؽ�ó
	DWORD dwNumMaterials = 0L;                    // �޽ÿ��� ������� ������ ����

	D3DXMATRIXA16 world;
	
};

