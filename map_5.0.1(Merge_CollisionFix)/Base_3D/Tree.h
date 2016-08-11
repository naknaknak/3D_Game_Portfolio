#pragma once
//#include <d3dx9.h>
#include "Obj.h"
#include "Collider_Sphere.h"

class Tree
{
public:
	Tree( );
	~Tree( );

	void Initialize(const  char* mapFileName, D3DXVECTOR3& pos, const  char* groundFileName = nullptr, bool isblender = false);
	void Destroy( );
	void Update( );
	void Render( );


	inline BoundingSphere GetBoundingSphere( ) { return colliderSphere->GetBoundingSphere( ); }
	inline void SetBoundingSphereMaterialColor(D3DXCOLOR color) { if (colliderSphere) colliderSphere->SetMaterialColor(color); }

	inline void SetPosition(D3DXVECTOR3& pos) { position = pos; }

	//inline void SetIsInCameraFrustum(bool in) { IsInCameraFrustum = in; }
	inline void SetWireMode(bool wireMode) { this->wireMode = wireMode; }
	inline void SetIsColliderSphereDraw(bool isColliderSphereDraw) { this->isColliderSphereDraw = isColliderSphereDraw; }

private:
	Obj* tree_obj = nullptr;
	Collider_Sphere* colliderSphere = nullptr;

	D3DXMATRIXA16 world;
	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);

	//bool IsInCameraFrustum = true;
	bool wireMode = false;
	bool isColliderSphereDraw = true;
};

