#include "stdafx.h"
#include "GameState_MeshViewer.h"


GameState_MeshViewer::GameState_MeshViewer()
{
}


GameState_MeshViewer::~GameState_MeshViewer()
{
}

void GameState_MeshViewer::Initialize()
{
	mv = new MeshViewer;
	mv->Initialize("tiger.x");
	//mv->Initialize("Widowmaker_test.x");

	OnEnterState();
}

void GameState_MeshViewer::Destroy()
{
	mv->Destroy();

}

void GameState_MeshViewer::Reset()
{
}

void GameState_MeshViewer::Update()
{
//	mv->Update();
}

void GameState_MeshViewer::Render()
{
	mv->Render();
}

void GameState_MeshViewer::OnEnterState()
{
	GameManager::GetCamera()->SetDistance(10.0f);
	Initialize();
}

void GameState_MeshViewer::OnLeaveState()
{
	Destroy();
}

LRESULT GameState_MeshViewer::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return LRESULT();
}
