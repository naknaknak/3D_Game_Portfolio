#include "stdafx.h"
#include "GameState_HeightMap.h"


GameState_HeightMap::GameState_HeightMap()
{
}


GameState_HeightMap::~GameState_HeightMap()
{
}

void GameState_HeightMap::Initialize()
{
	if ( sky == nullptr )
	{
		sky = new SkyBox;
		sky->Initialize("cloud_1.png", 4096);
	}
	if (grid == nullptr)
	{
		grid = new Grid;
		grid->Initialize();
		grid->SetShowArrow(true);
		grid->SetShowGround(true);
	}
	if ( heightMap == nullptr )
	{
		heightMap = new HeightMap;
		heightMap->Initialize("field_.raw", "field_0.png", 8);
	}
	if ( girl == nullptr )
	{
		girl = new Character_HeightMapMove();
		girl->Initialize();
		girl->SetHeightMap(heightMap);
		//girl->SetPosition(D3DXVECTOR3(5.f, 10.0f, -5.f));
		girl->SetPosition(D3DXVECTOR3(35.5f, 20.0f, -65.5f));
		GameManager::GetCamera()->SetLookTarget(girl->GetPositionAdress());
	}
}

void GameState_HeightMap::Destroy()
{
	SAFE_DELETE(girl);
	SAFE_DELETE(heightMap);
	SAFE_DELETE(grid);
	SAFE_DELETE(sky);
}

void GameState_HeightMap::Reset()
{
}

void GameState_HeightMap::Update()
{
	if ( sky )
	{
		sky->Update();
	}
	if ( girl )
	{
		girl->Update();
		girl->GroundCheck();
	}
}

void GameState_HeightMap::Render()
{
	if ( sky )
	{
		sky->Render();
	}
	if ( grid )
	{
		grid->Render();
	}
	if ( heightMap )
	{
		heightMap->Render();
	}
	if ( girl )
	{
		girl->Render();
	}
	RECT rc = { 10, RESOLUTION_Y - 55, 11, 11 };
	UI_Manager::GetFont()->DrawTextA(NULL, "GameState_HeightMap", strlen("GameState_HeightMap"), &rc, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
}

void GameState_HeightMap::OnEnterState()
{
	GameManager::GetCamera()->SetDistance(10.0f);	
	Initialize();
}

void GameState_HeightMap::OnLeaveState()
{
	Destroy();
}

LRESULT GameState_HeightMap::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	//if ( result == 0 && girl )
	//{
	//	girl->InputProc(hWnd, message, wParam, lParam);
	//}
	return result;
}
