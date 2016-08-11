#include "stdafx.h"
#include "Obj.h"
#include "ObjLoader.h"

Obj::Obj()
{
}


Obj::~Obj()
{
}

void Obj::Initialize(const char* mapFileName, const char* groundFileName /*= nullptr*/, bool isblender /*= false*/)
{
	Destroy();
	ObjLoader::LoadObjFileData(objGroups, mapFileName, isblender);

	if (groundFileName)
	{
		ObjLoader::LoadObj_OnlyVertex(objGround, groundFileName);
	}

	D3DXMatrixIdentity(&world);
}

void Obj::Destroy()
{
	for ( auto iter = objGroups.begin(); iter != objGroups.end(); ++iter )
	{
		delete *iter;
		*iter = nullptr;
	}
	objGroups.clear();
	objGround.clear();
}

void Obj::Update()
{
}

void Obj::Render()
{
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);
	//
	//GameManager::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	//
	GameManager::GetDevice()->SetRenderState(D3DRS_LIGHTING, true);
	GameManager::GetDevice()->SetTransform(D3DTS_WORLD, &world);
	for ( auto iter = objGroups.begin(); iter != objGroups.end(); ++iter )
	{
		( *iter )->Render();
	}
	//
	//GameManager::GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	//
}

bool Obj::GroundCheck(IN OUT D3DXVECTOR3& groundPos) const
{
	bool find = false;
	D3DXVECTOR3 rayStart(groundPos.x, 1000.0f, groundPos.z);
	D3DXVECTOR3 rayDirection(0, -1, 0);

	for (size_t i = 0; i < objGround.size(); i += 3)
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