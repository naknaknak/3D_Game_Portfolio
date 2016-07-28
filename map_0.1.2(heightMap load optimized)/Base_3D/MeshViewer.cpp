#include "stdafx.h"
#include "MeshViewer.h"
#include "Strsafe.h"

MeshViewer::MeshViewer()
{
}

MeshViewer::~MeshViewer()
{
}

HRESULT MeshViewer::Initialize(char* filename)
{
	D3DXMatrixIdentity(&world);

	LPD3DXBUFFER pD3DXMtrlBuffer;
	// Tiger.x ������ �޽÷� �о���δ�. �̶� ���� ������ �Բ� �д´�.
  	if ( FAILED(D3DXLoadMeshFromXA(filename, D3DXMESH_SYSTEMMEM,
		GameManager::GetDevice(), nullptr,
		&pD3DXMtrlBuffer, nullptr,
		&dwNumMaterials,
		&pMesh)) )
	{
		////���� ���� ������ �������� �˻�
		//if ( FAILED(D3DXLoadMeshFromX(L"..\\tiger.X", D3DXMESH_SYSTEMMEM,
		//	GameManager::GetDevice(), nullptr,
		//	&pD3DXMtrlBuffer, nullptr,
		//	&dwNumMaterials,
		//	&pMesh)) )
		{
			MessageBoxA(nullptr, "Could not find tiger.x", "Meshes.exe", MB_OK);
			return E_FAIL;
		}
	}

	// ���� ������ �ؽ�ó ������ ���� �̾Ƴ���.
	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	pMeshMaterials = new D3DMATERIAL9[dwNumMaterials];

	if ( pMeshMaterials == nullptr )
	{
		return E_OUTOFMEMORY;
	}
	pMeshTextures = new LPDIRECT3DTEXTURE9[dwNumMaterials];
	if ( pMeshTextures == nullptr )
	{
		return E_OUTOFMEMORY;
	}

	for ( DWORD i = 0; i < dwNumMaterials; ++i )
	{
		//���� ����
		pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

		//������ �ָ鱤 ���� ����(D3DX���� ���� ������ ����)
		pMeshMaterials[i].Ambient = pMeshMaterials[i].Diffuse;

		pMeshTextures[i] = nullptr;
		if ( d3dxMaterials[i].pTextureFilename != nullptr && lstrlenA(d3dxMaterials[i].pTextureFilename) > 0 )
		{
			//�ؽ��� ����
			if ( FAILED(D3DXCreateTextureFromFileA(GameManager::GetDevice(), d3dxMaterials[i].pTextureFilename, &pMeshTextures[i])) )
			{
				//�������� �˻�
				const CHAR* strPrefix = "..\\";
				CHAR strTexture[MAX_PATH];
				StringCchCopyA(strTexture, MAX_PATH, strPrefix);
				StringCchCatA(strTexture, MAX_PATH, d3dxMaterials[i].pTextureFilename);
				//�������� �˻�
				if ( FAILED(D3DXCreateTextureFromFileA(GameManager::GetDevice(), strTexture, &pMeshTextures[i])) )
				{
					MessageBox(nullptr, L"Could not find tiger.x", L"Meshes.exe", MB_OK);
					return E_FAIL;
				}
			}
		}
	}

	// ���� ���� ��� �Ϸ�
	pD3DXMtrlBuffer->Release();


	return E_NOTIMPL;

}

void MeshViewer::Destroy()
{
}

void MeshViewer::Update()
{
	//D3DXMATRIXA16 rot;
	//D3DXMatrixRotationY(&rot, timeGetTime() / 1000.0f);
	//world *= rot;
}

void MeshViewer::Render()
{
	//�ĸ� ���ۿ� Z���۸� �����.
	GameManager::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	//������ ����
	GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);

	//�޽ô� ������ �ٸ� �޽ú��� �κ������� �̷�� �ִ�.
	//�̵��� ������ �����ؼ� ��� �׷��ش�.

	for ( DWORD i = 0; i < dwNumMaterials; ++i )
	{
		//�κ����� �޽��� ������ �ؽ��� ����
		GameManager::GetDevice()->SetMaterial(&pMeshMaterials[i]);
		GameManager::GetDevice()->SetTexture(0, pMeshTextures[i]);

		//�κ����� �޽� ���
		pMesh->DrawSubset(i);
	}


}
