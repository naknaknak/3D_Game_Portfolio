#include "stdafx.h"
#include "Rock_location.h"
#include "AseLoader.h"

Rock_location::Rock_location( )
{
}

Rock_location::~Rock_location( )
{
}

void Rock_location::Initialize(char* fileName, D3DXVECTOR3 scale, D3DXVECTOR3 position, float rot_axisY, float rot_axisX/*= 0*/)
{
	AseLoader ase;
	aseRootFrame = ase.Load(fileName);
	aseRootFrame->SetTextureOn(true);

	this->scale = scale;
	this->rot_axisY = rot_axisY;
	this->rot_axisX = rot_axisX;
	this->position = position;

	D3DXMatrixIdentity(&world);
	D3DXMATRIX scale_;
	D3DXMatrixScaling(&scale_, scale.x, scale.y, scale.z);
	D3DXMATRIX rotationY;
	D3DXMatrixRotationY(&rotationY, rot_axisY);
	D3DXMATRIX rotationX;
	D3DXMatrixRotationX(&rotationX, rot_axisX);
	D3DXMATRIX translation;
	D3DXMatrixTranslation(&translation, position.x, position.y, position.z);

	world *= scale_* rotationY * rotationX * translation;
}

void Rock_location::Destroy( )
{
	SAFE_DELETE(aseRootFrame);
}

void Rock_location::Update( )
{
	if (aseRootFrame)
	{
		int keyFrame = GetTickCount( ) % (3200 - 640) + 640;
		aseRootFrame->Update(keyFrame, &world);
	}
}

void Rock_location::Render( )
{
	if (aseRootFrame)
	{
		aseRootFrame->Render( );
	}

}
