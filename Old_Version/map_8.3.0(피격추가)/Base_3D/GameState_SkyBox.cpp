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
		//grid = new Grid;
		//grid->Initialize();
		//grid->SetShowArrow(true);
		//grid->SetShowGround(true);
	}
	if ( sky == nullptr )
	{
		sky = new SkyBox();
		sky->Initialize("cloud_1_2.png", 100, 64, 32);
		//sky->Initialize("cloud_1_2.png", 100);
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

}

void GameState_SkyBox::OnEnterState()
{
	GameManager::GetCamera()->SetDistance(700.0f);
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
