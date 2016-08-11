#include "stdafx.h"
#include "Collider_Box.h"


Collider_Box::Collider_Box()
{
}


Collider_Box::~Collider_Box()
{
}

void Collider_Box::Initialize(std::vector<D3DXVECTOR3>& pos, const D3DXMATRIXA16* matScale/* = nullptr*/, const D3DXMATRIXA16* matRot/* = nullptr*/, const D3DXMATRIXA16* matTrans /* = nullptr*/)
{
	D3DXMatrixIdentity(&world);
	if (matScale)
	{
		scale = *matScale;
	}
	if (matTrans)
	{
		trans = *matTrans;
	}
	world *= scale * trans;

	MinMaxVertices(pos, matRot);

	boundingBox = BoundingBox(vecMin, vecMax);
	currentBoundingBox = boundingBox;

	D3DXVECTOR3 boxSize = boundingBox.max - boundingBox.min;
	D3DXCreateBox(GameManager::GetDevice(), boxSize.x, boxSize.y, boxSize.z, &aabbMesh, nullptr);
	assert(aabbMesh != nullptr);

	ZeroMemory(&material, sizeof(D3DMATERIAL9));
	material.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	material.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	material.Specular = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);
}
void Collider_Box::MinMaxVertices(std::vector<D3DXVECTOR3> & pos, const D3DXMATRIXA16* matRot /*= nullptr*/)
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
void Collider_Box::Destroy()
{
	SAFE_RELEASE(aabbMesh);
}

void Collider_Box::Update()
{
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);
	if ( parent )
	{
		world *= ( *parent );
	}
	D3DXVec3TransformCoord(&currentBoundingBox.min, &boundingBox.min, &world);
	D3DXVec3TransformCoord(&currentBoundingBox.max, &boundingBox.max, &world);
	vecMax = currentBoundingBox.max;
	vecMin = currentBoundingBox.min;
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
