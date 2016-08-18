#include "stdafx.h"
#include "Tree.h"


Tree::Tree( )
{
}


Tree::~Tree( )
{
}

void Tree::Initialize(const char* mapFileName, D3DXVECTOR3& position_tree, const char* groundFileName /*= nullptr*/, bool isblender /*= false*/)
{
	position = position_tree;
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);

	tree_obj = new Obj;
	tree_obj->Initialize(mapFileName, position_tree);
	std::vector<D3DXVECTOR3> &vertexList = tree_obj->GetVertexList( );
	///
	colliderSphere = new Collider_Sphere;
	colliderSphere->Initialize(vertexList, &world);
	colliderSphere->SetWireRender(true);

}

void Tree::Destroy( )
{
	SAFE_DELETE(colliderSphere);
	SAFE_DELETE(tree_obj);
}

void Tree::Update( )
{
}

void Tree::Render( )
{
	bool isIn = GameManager::GetCamera( )->IsInCameraFrustum(&colliderSphere->GetBoundingSphere( ));
	if (isIn)
	{
		if (wireMode) GameManager::GetDevice( )->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		tree_obj->Render( );
		if (isColliderSphereDraw) colliderSphere->Render( );
		if (wireMode) GameManager::GetDevice( )->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

}
