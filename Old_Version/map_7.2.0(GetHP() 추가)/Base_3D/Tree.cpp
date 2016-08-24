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
	std::vector<D3DXVECTOR3> vertexList;
	D3DXVECTOR3 vecMax = D3DXVECTOR3(FLT_MIN, FLT_MIN, FLT_MIN);
	D3DXVECTOR3 vecMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);

	position = position_tree;

	D3DXMatrixIdentity(&world);

	D3DXMATRIXA16 matTrans;
	D3DXMatrixTranslation(&matTrans, position.x, position.y, position.z);

	world *= matTrans;

	if (tree_obj == nullptr)
	{
		tree_obj = new Obj;
		tree_obj->Initialize(mapFileName, &world);
		vertexList = tree_obj->GetVertexList( );


		if (colliderSphere == nullptr)
		{
		Collision::MinMaxVertices(vecMin, vecMax, vertexList);
			colliderSphere = new Collider_Sphere;
			colliderSphere->SetMinMax(vecMin, vecMax);
			colliderSphere->Initialize(&world);
			colliderSphere->SetWireRender(true);
		}

		if (colliderBox == nullptr)
		{
			float baseHeight = (vecMax.y - vecMin.y) * 0.5f;
			vecMin.y += baseHeight;
			vecMax.y += baseHeight;
			
			D3DXVECTOR3 correctHeight = D3DXVECTOR3(0, baseHeight, 0);
			
			D3DXMATRIXA16 correctTrans;
			D3DXMatrixTranslation(&correctTrans, correctHeight.x, correctHeight.y, correctHeight.z);
			
			D3DXMATRIXA16 correctWorld = world;
			correctWorld *= correctTrans;

			colliderBox = new Collider_Box;
			colliderBox->SetMinMax(vecMin, vecMax);
			colliderBox->Initialize(&correctWorld);
			colliderBox->SetWireRender(true);
		}
	}
}

void Tree::Destroy( )
{
	SAFE_DELETE(colliderBox);
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
		
		if (wireMode) GameManager::GetDevice( )->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

		if (colliderSphere)
		{
			if (isColliderSphereDraw) colliderSphere->Render( );
		}
		if (colliderBox)
		{
			if (IsColliderBoxDraw) colliderBox->Render( );
		}
	}

}
