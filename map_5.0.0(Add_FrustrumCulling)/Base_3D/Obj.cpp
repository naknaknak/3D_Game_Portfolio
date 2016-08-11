#include "stdafx.h"
#include "Obj.h"
#include "ObjLoader.h"

Obj::Obj( )
{
}


Obj::~Obj( )
{
}

void Obj::Initialize(const char* mapFileName, D3DXVECTOR3& pos, const char* groundFileName /*= nullptr*/, bool isblender /*= false*/)
{
	Destroy( );
	std::vector<D3DXVECTOR3> vertexList;
	ObjLoader::LoadObjFileData(objGroups, mapFileName, vertexList, isblender);

	if (groundFileName)
	{
		ObjLoader::LoadObj_OnlyVertex(objGround, groundFileName);
	}
	position = pos;
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);


	///
	colliderSphere = new Collider_Sphere;
	colliderSphere->Initialize(vertexList, &world);
	colliderSphere->SetWireRender(true);


}

void Obj::Destroy( )
{
	SAFE_DELETE(colliderSphere);
	for (auto iter = objGroups.begin( ); iter != objGroups.end( ); ++iter)
	{
		delete *iter;
		*iter = nullptr;
	}
	objGroups.clear( );
	objGround.clear( );
}

void Obj::Update( )
{
}

void Obj::Render( )
{
	bool isIn = GameManager::GetCamera( )->IsInCameraFrustum(colliderSphere->GetBoundingSphere( ));
	if (isIn)
	{
		if (wireMode) GameManager::GetDevice( )->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		GameManager::GetDevice( )->SetRenderState(D3DRS_LIGHTING, true);
		GameManager::GetDevice( )->SetTransform(D3DTS_WORLD, &world);
		for (auto iter = objGroups.begin( ); iter != objGroups.end( ); ++iter)
		{
			(*iter)->Render( );
			if(isColliderSphereDraw) colliderSphere->Render( );
		}
		if (wireMode) GameManager::GetDevice( )->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	
}
bool Obj::GroundCheck(IN OUT D3DXVECTOR3& groundPos) const
{
	bool find = false;
	D3DXVECTOR3 rayStart(groundPos.x, 1000.0f, groundPos.z);
	D3DXVECTOR3 rayDirection(0, -1, 0);

	for (size_t i = 0; i < objGround.size( ); i += 3)
	{
		float u, v, distance;
		find = D3DXIntersectTri(
			&objGround[i], &objGround[i + 1], &objGround[i + 2],
			&rayStart, &rayDirection,
			&u, &v, &distance) != 0;

		if (find == true)
		{
			//groundPos = rayStart - rayDirection * distance;
			groundPos.y = 1000.0f - distance;
			//groundPos = objGround[i]
			//			 + ( ( objGround[i + 1] - objGround[i] ) * u ) 
			//			 + ( ( objGround[i + 2] - objGround[i] ) * v );
			break;
		}

	}

	return find;
}
