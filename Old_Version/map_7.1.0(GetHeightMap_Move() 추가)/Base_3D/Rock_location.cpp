#include "stdafx.h"
#include "Rock_location.h"
#include "AseLoader.h"

Rock_location::Rock_location( )
{
}

Rock_location::~Rock_location( )
{
}

void Rock_location::Initialize(const char* fileName, D3DXVECTOR3 scale, D3DXVECTOR3 position, float rot_axisY, float rot_axisX/*= 0*/)
{
	D3DXVECTOR3 vecMax = D3DXVECTOR3(FLT_MIN, FLT_MIN, FLT_MIN);
	D3DXVECTOR3 vecMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);

	D3DXMatrixIdentity(&world);
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);
	D3DXMATRIXA16 rotationY;
	D3DXMatrixRotationY(&rotationY, rot_axisY);
	D3DXMATRIXA16 rotationX;
	D3DXMatrixRotationX(&rotationX, rot_axisX);
	D3DXMATRIXA16 matTrans;
	D3DXMatrixTranslation(&matTrans, position.x, position.y, position.z);

	world *= matScale * rotationY * rotationX * matTrans;
	
	if (aseRootFrame == nullptr)
	{
		AseLoader ase;
		aseRootFrame = ase.Load(fileName);
		aseRootFrame->SetTextureOn(true);

		if (colliderBox == nullptr)
		{
			D3DXMATRIXA16 mstST = matScale * matTrans;
			D3DXMATRIXA16 matRot;
			D3DXMatrixIdentity(&matRot);
			matRot *= rotationY * rotationX;

			colliderBox = new Collider_Box;

			std::map<int, std::vector<D3DXVECTOR3>> &mapVertexList = ase.GetMapVertices( );
			for (auto iter = mapVertexList.begin( ); iter != mapVertexList.end( ); ++iter)
			{
				Collision::MinMaxVertices(vecMin, vecMax, iter->second, &matRot);
			}
			
			D3DXVec3TransformCoord(&vecMin, &vecMin, &matScale);
			D3DXVec3TransformCoord(&vecMax, &vecMax, &matScale);

			colliderBox->SetMinMax(vecMin, vecMax);
			colliderBox->Initialize(&matTrans);

			colliderBox->SetWireRender(true);
		}

		if (colliderSphere == nullptr)
		{
			colliderSphere = new Collider_Sphere;
			colliderSphere->SetMinMax(vecMin, vecMax);
			colliderSphere->Initialize(&matTrans);
			colliderSphere->SetWireRender(true);
		}
	}
}

void Rock_location::Destroy( )
{
	SAFE_DELETE(colliderSphere);
	SAFE_DELETE(colliderBox);
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
		bool isIn = GameManager::GetCamera( )->IsInCameraFrustum(&colliderSphere->GetBoundingSphere( ));
		if (isIn)
		{
			if (wireMode) GameManager::GetDevice( )->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

			GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
			GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			GameManager::GetDevice( )->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);

			aseRootFrame->Render( );

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
}
