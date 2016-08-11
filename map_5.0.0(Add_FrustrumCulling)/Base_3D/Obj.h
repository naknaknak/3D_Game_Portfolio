#pragma once
#include "ObjFileGroup.h"


class Obj
{
public:
	Obj( );
	~Obj( );

	void Initialize(const  char* mapFileName, D3DXVECTOR3& pos, const  char* groundFileName = nullptr, bool isblender = false);
	void Destroy( );
	void Update( );
	void Render( );


	bool GroundCheck(IN OUT D3DXVECTOR3& groundPos) const;

	inline void SetPosition(D3DXVECTOR3& pos) { position = pos; }
	inline void SetIsInCameraFrustum(bool in) { IsInCameraFrustum = in; }
	void SetWireMode(bool wireMode) { this->wireMode = wireMode; }
	void SetIsColliderSphereDraw(bool isColliderSphereDraw) { this->isColliderSphereDraw = isColliderSphereDraw; }

private:

	D3DXMATRIXA16 world;
	D3DXMATRIXA16 rotation;

	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);

	std::vector<ObjFileGroup*> objGroups;
	std::vector<D3DXVECTOR3> objGround;

	Collider_Sphere* colliderSphere = nullptr;
	bool IsInCameraFrustum = true;
	bool wireMode = false;
	bool isColliderSphereDraw = true;
};

