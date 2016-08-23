#include "stdafx.h"
#include "Collider_Sphere.h"


Collider_Sphere::Collider_Sphere()
{
}


Collider_Sphere::~Collider_Sphere()
{
}

void Collider_Sphere::Initialize(D3DXMATRIXA16* trans /*= nullptr*/)
{
	D3DXVECTOR3 vecSize = vecMax - vecMin;
	assert(((vecSize.x > 0 && vecSize.y > 0) && vecSize.z > 0) && "사이즈가 음수이다.");

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
	
	D3DXVec3TransformCoord(&boundingSphere.center, &boundingSphere.center, &world);

	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	material.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	material.Specular = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
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
