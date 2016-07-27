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
	// Tiger.x 파일을 메시로 읽어들인다. 이때 재질 정보도 함께 읽는다.
  	if ( FAILED(D3DXLoadMeshFromXA(filename, D3DXMESH_SYSTEMMEM,
		GameManager::GetDevice(), nullptr,
		&pD3DXMtrlBuffer, nullptr,
		&dwNumMaterials,
		&pMesh)) )
	{
		////현재 폴더 없으면 상위폴더 검색
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

	// 재질 정보와 텍스처 정보를 따로 뽑아낸다.
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
		//재질 복사
		pMeshMaterials[i] = d3dxMaterials[i].MatD3D;

		//재질용 주면광 색깔 설정(D3DX에서 직접 해주지 않음)
		pMeshMaterials[i].Ambient = pMeshMaterials[i].Diffuse;

		pMeshTextures[i] = nullptr;
		if ( d3dxMaterials[i].pTextureFilename != nullptr && lstrlenA(d3dxMaterials[i].pTextureFilename) > 0 )
		{
			//텍스쳐 생성
			if ( FAILED(D3DXCreateTextureFromFileA(GameManager::GetDevice(), d3dxMaterials[i].pTextureFilename, &pMeshTextures[i])) )
			{
				//상위폴더 검색
				const CHAR* strPrefix = "..\\";
				CHAR strTexture[MAX_PATH];
				StringCchCopyA(strTexture, MAX_PATH, strPrefix);
				StringCchCatA(strTexture, MAX_PATH, d3dxMaterials[i].pTextureFilename);
				//상위폴더 검색
				if ( FAILED(D3DXCreateTextureFromFileA(GameManager::GetDevice(), strTexture, &pMeshTextures[i])) )
				{
					MessageBox(nullptr, L"Could not find tiger.x", L"Meshes.exe", MB_OK);
					return E_FAIL;
				}
			}
		}
	}

	// 재질 버퍼 사용 완료
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
	//후면 버퍼와 Z버퍼를 지운다.
	GameManager::GetDevice()->Clear(0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	//렌더링 시작
	GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);

	//메시는 재질이 다른 메시별로 부분집합을 이루고 있다.
	//이들을 루프를 수행해서 모두 그려준다.

	for ( DWORD i = 0; i < dwNumMaterials; ++i )
	{
		//부분집합 메시의 재질과 텍스쳐 설정
		GameManager::GetDevice()->SetMaterial(&pMeshMaterials[i]);
		GameManager::GetDevice()->SetTexture(0, pMeshTextures[i]);

		//부분집합 메시 출력
		pMesh->DrawSubset(i);
	}


}
