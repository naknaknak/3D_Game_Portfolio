#include "stdafx.h"
#include "GameState_SkyBox.h"


GameState_SkyBox::GameState_SkyBox()
{
}


GameState_SkyBox::~GameState_SkyBox()
{
}

void GameState_SkyBox::Initialize()
{
	if ( grid == nullptr )
	{
		grid = new Grid;
		grid->Initialize();
		grid->SetShowArrow(true);
		grid->SetShowGround(true);
	}
	if ( sky == nullptr )
	{
		sky = new SkyBox();
		sky->Initialize("sky_surround.png", 20);
		//sky->Initialize("sky_surround_up.png", 20);
	}
}

void GameState_SkyBox::Destroy()
{
	SAFE_DELETE(sky);
	SAFE_DELETE(grid);
}

void GameState_SkyBox::Reset()
{
}

void GameState_SkyBox::Update()
{
	sky->Update();
}

void GameState_SkyBox::Render()
{
	if ( grid )
	{
		grid->Render();
	}
	if ( sky )
	{
		sky->Render();
	}
	RECT rc = { 10, RESOLUTION_Y - 55, 11, 11 };
	UI_Manager::GetFont()->DrawTextA(NULL, "GameState_SkyBox", strlen("GameState_SkyBox"), &rc, DT_TOP | DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));
}

void GameState_SkyBox::OnEnterState()
{
	GameManager::GetCamera()->SetDistance(10.0f);
	Initialize();
}

void GameState_SkyBox::OnLeaveState()
{
	Destroy();
}

LRESULT GameState_SkyBox::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	return result;
}
