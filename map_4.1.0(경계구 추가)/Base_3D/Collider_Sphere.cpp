#include "stdafx.h"
#include "Collider_Sphere.h"


Collider_Sphere::Collider_Sphere()
{
}


Collider_Sphere::~Collider_Sphere()
{
}

void Collider_Sphere::Initialize(std::vector<D3DXVECTOR3> & pos, D3DXMATRIXA16* trans /*= nullptr*/, D3DXMATRIXA16* rot /*= nullptr*/)
{
	MinMaxVertices(pos, rot != nullptr ? rot : nullptr);
	D3DXVECTOR3 vecSize = vecMax - vecMin;
	float fDiameter = D3DXVec3Length(&vecSize);
	boundingSphere.center = vecSize * 0.5f + vecMin;
	boundingSphere.radius = fDiameter * 0.5f;

	D3DXCreateSphere(GameManager::GetDevice(), boundingSphere.radius, 16, 8, &sphereMesh, nullptr);
	assert(sphereMesh != nullptr);

	D3DXMATRIXA16 matTrans;
	D3DXMatrixTranslation(&matTrans, boundingSphere.center.x, boundingSphere.center.y, boundingSphere.center.z);

	D3DXMatrixIdentity(&world);
	world *= matTrans;
	if (trans)
	{
		world *= (*trans);
	}
	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	material.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	material.Specular = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
}
void Collider_Sphere::MinMaxVertices(std::vector<D3DXVECTOR3> & pos, const D3DXMATRIXA16* matRot /*= nullptr*/)
{
	D3DXVECTOR3 vecComp;
	for (auto iter = pos.begin( ); iter != pos.end( ); ++iter)
	{
		if (matRot != nullptr)
		{
			D3DXVec3TransformNormal(&vecComp, &*iter, &*matRot);
		}
		else
		{
			vecComp = (*iter);
		}
		D3DXVec3Maximize(&vecMax, &vecComp, &vecMax);
		D3DXVec3Minimize(&vecMin, &vecComp, &vecMin);
	}
}
void Collider_Sphere::Destroy()
{
	SAFE_RELEASE(sphereMesh);
}

void Collider_Sphere::Update()
{
	D3DXMatrixTranslation(&world, boundingSphere.center.x, boundingSphere.center.y, boundingSphere.center.z);
	if ( parent )
	{
		world *= ( *parent );
	}
}

void Collider_Sphere::Render()
{
	if ( wireMode )
	{
		GameManager::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
	GameManager::GetDevice()->SetTexture(0, nullptr);
	GameManager::GetDevice()->SetMaterial(&material);
	GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);
	sphereMesh->DrawSubset(0);
	if ( wireMode )
	{
		GameManager::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

}
