#pragma once

#include "AseFrame.h"
#include "Collider_Box.h"

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

	void SetWireMode(bool wireMode) { this->wireMode = wireMode; }
	void SetIsColliderBoxDraw(bool IsColliderBoxDraw) { this->IsColliderBoxDraw = IsColliderBoxDraw; }

private:
	AseFrame* aseRootFrame = nullptr;
	Collider_Box* colliderBox = nullptr;
	D3DXMATRIXA16 world;
	float baseHeight = 0.f;
	bool textureOn = false;
	bool wireMode = false;
	bool IsColliderBoxDraw = true;
	
	//
};

