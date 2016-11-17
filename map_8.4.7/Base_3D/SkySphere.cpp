#include "stdafx.h"
#include "SkySphere.h"


SkySphere::SkySphere()
{
}


SkySphere::~SkySphere()
{
}

void SkySphere::Initialize()
{
	vecScale = D3DXVECTOR3(10.f, 10.f, 10.f);
	vecPosition = D3DXVECTOR3(256.f, 0.f, -256.f);

	//D3DXMatrixIdentity(&matWorld);

	//D3DXMATRIXA16 matScales;
	//D3DXMatrixScaling(&matScales, vecScale.x, vecScale.y, vecScale.z);

	//D3DXMATRIXA16 matPosition;
	//D3DXMatrixTranslation(&matPosition, vecPosition.x, vecPosition.y, vecPosition.z);

	//matWorld *= matScales * matPosition;


	// 텍스처 로딩
	//char* textureFilename = "hirez_finished_map.jpg";
	char* textureFilename = "sunset.png";
	//char* textureFilename = "daytime.png";
	//char* textureFilename = "cloud_2.png";SS
	D3DXCreateTextureFromFileA(GameManager::GetDevice(), textureFilename, &m_pEarthDM);
	assert(m_pEarthDM != nullptr);
	
	// 모델 로딩
	char* modelFilename = "Sphere_Earth.x";
	D3DXLoadMeshFromXA(modelFilename, D3DXMESH_SYSTEMMEM, GameManager::GetDevice(), NULL, NULL, NULL, NULL, &m_pSphere);
	assert(m_pSphere != nullptr);

	// 쉐이더 로딩
	char* shaderFilename = "TextureMapping.fx";
	LPD3DXBUFFER pError = NULL;
	DWORD dwShaderFlags = 0;
#if _DEBUG
	dwShaderFlags |= D3DXSHADER_DEBUG;
#endif
	D3DXCreateEffectFromFileA(GameManager::GetDevice(), shaderFilename, NULL, NULL, dwShaderFlags, NULL, &m_pTextureMappingShader, &pError);
	assert(m_pTextureMappingShader != nullptr);

}

void SkySphere::Destroy()
{
	// 모델을 release 한다.
	if (m_pSphere)
	{
		m_pSphere->Release();
		m_pSphere = nullptr;
	}
	// 쉐이더를 release 한다.
	if (m_pTextureMappingShader)
	{
		m_pTextureMappingShader->Release();
		m_pTextureMappingShader = nullptr;
	}

	// 텍스처를 release 한다.
	if (m_pEarthDM)
	{
		m_pEarthDM->Release();
		m_pEarthDM = nullptr;
	}

}

void SkySphere::Update()
{
	D3DXMatrixIdentity(&matWorld);

	D3DXMATRIXA16 matScales;
	D3DXMatrixScaling(&matScales, vecScale.x, vecScale.y, vecScale.z);

	D3DXMATRIXA16 matPosition;
	D3DXMatrixTranslation(&matPosition, vecPosition.x, vecPosition.y, vecPosition.z);

	gRotationY += 0.003f * D3DX_PI / 180.0f;
	if (gRotationY > 2 * D3DX_PI)
	{
		gRotationY -= 2 * D3DX_PI;
	}
	D3DXMATRIXA16 matRotate;
	D3DXMatrixRotationY(&matRotate, gRotationY);

	matWorld *= matScales * matRotate * matPosition;
	
	//
	GameManager::GetDevice()->GetTransform(D3DTS_PROJECTION, &matProjection);
	GameManager::GetDevice()->GetTransform(D3DTS_VIEW, &matView);

	m_pTextureMappingShader->SetMatrix("gWorldMatrix", &matWorld);
	m_pTextureMappingShader->SetMatrix("gViewMatrix", &matView);
	m_pTextureMappingShader->SetMatrix("gProjectionMatrix", &matProjection);
	m_pTextureMappingShader->SetTexture("DiffuseSampler_Tex", m_pEarthDM);
}

void SkySphere::Render()
{

	GameManager::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);
	//셰이더를 시작한다.
	UINT numPasses = 0;
	m_pTextureMappingShader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pTextureMappingShader->BeginPass(i);
			{
				// 구체를 그린다.
				m_pSphere->DrawSubset(0);
			}
			m_pTextureMappingShader->EndPass();
		}
		m_pTextureMappingShader->End();
	}
	GameManager::GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

