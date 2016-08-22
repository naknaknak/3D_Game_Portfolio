#pragma once

#include "AseFrame.h"
#include "Collider_Box.h"
#include "Collider_Sphere.h"

class Rock_location
{
public:
	Rock_location( );
	~Rock_location( );

	//void Initialize( );
	void Initialize(const char* fileName, D3DXVECTOR3 scale, D3DXVECTOR3 position, float rot_axisY, float rot_axisX = 0);
	void Destroy( );
	void Update( );
	void Render( );

	inline void SetWireMode(bool wireMode) { this->wireMode = wireMode; }
	inline void SetIsColliderBoxDraw(bool IsColliderBoxDraw) { this->IsColliderBoxDraw = IsColliderBoxDraw; }
	inline void SetIsColliderSphereDraw(bool isColliderSphereDraw) { this->isColliderSphereDraw = isColliderSphereDraw; }

private:
	AseFrame* aseRootFrame = nullptr;
	
	Collider_Box* colliderBox = nullptr;
	Collider_Sphere* colliderSphere = nullptr;

	D3DXMATRIXA16 world;
	bool textureOn = false;
	bool wireMode = false;
	bool IsColliderBoxDraw = true;
	bool isColliderSphereDraw = true;

};

