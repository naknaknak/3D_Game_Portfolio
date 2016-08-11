#pragma once

#include "AseFrame.h"

class Rock_location
{
public:
	Rock_location( );
	~Rock_location( );

	//void Initialize( );
	void Initialize(char* fileName, D3DXVECTOR3 scale, D3DXVECTOR3 position, float rot_axisY, float rot_axisX = 0);
	void Destroy( );
	void Update( );
	void Render( );

private:
	D3DXMATRIXA16 world;
	D3DXVECTOR3 scale = D3DXVECTOR3(0, 0, 0);
	float rot_axisY = 0.f;
	float rot_axisX = 0.f;
	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);

	AseFrame* aseRootFrame = nullptr;

	bool textureOn = false;
};

