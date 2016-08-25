#include "stdafx.h"
#include "Collider_Box.h"


Collider_Box::Collider_Box()
{
}


Collider_Box::~Collider_Box()
{
}

void Collider_Box::Initialize(const D3DXMATRIXA16* world/* = nullptr*/)
{
	D3DXMATRIXA16 trans;
	D3DXMatrixIdentity(&this->world);
	this->world = *world;

	// ¹Ú½º
	boundingBox = BoundingBox(vecMin, vecMax);
	D3DXVECTOR3 boxSize = boundingBox.max - boundingBox.min;
	
	D3DXVec3TransformCoord(&boundingBox.min, &boundingBox.min, &this->world);
	D3DXVec3TransformCoord(&boundingBox.max, &boundingBox.max, &this->world);
	
	
	currentBoundingBox = boundingBox;

	// CreateBox
	D3DXCreateBox(GameManager::GetDevice(), boxSize.x, boxSize.y, boxSize.z, &aabbMesh, nullptr);
	assert(aabbMesh != nullptr);

	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	material.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	material.Specular = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);

}

void Collider_Box::Destroy()
{
	SAFE_RELEASE(aabbMesh);
}

void Collider_Box::Update()
{
	//D3DXMatrixTranslation(&world, position.x, position.y, position.z);
	//if ( parent )
	//{
	//	world *= ( *parent );
	//}
	//D3DXVec3TransformCoord(&currentBoundingBox.min, &boundingBox.min, &world);
	//D3DXVec3TransformCoord(&currentBoundingBox.max, &boundingBox.max, &world);
	//vecMax = currentBoundingBox.max;
	//vecMin = currentBoundingBox.min;
}

void Collider_Box::Render()
{
	if ( wireMode )
	{
		GameManager::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
	GameManager::GetDevice()->SetTexture(0, nullptr);
	GameManager::GetDevice()->SetMaterial(&material);
	GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);
	aabbMesh->DrawSubset(0);
	if ( wireMode )
	{
		GameManager::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}
