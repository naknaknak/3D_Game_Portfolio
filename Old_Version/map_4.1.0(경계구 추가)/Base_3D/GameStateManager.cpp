#include "stdafx.h"
#include "GameStateManager.h"


GameStateManager::GameStateManager()
{
}


GameStateManager::~GameStateManager()
{
}
  
void GameStateManager::Initialize()
{
	gameStates[GameStateType::STATE_OBJ_LOAD] = new GameState_OBJ_Load;
	gameStates[GameStateType::STATE_HEIGHT_MAP] = new GameState_HeightMap;
	gameStates[GameStateType::STATE_SKYBOX] = new GameState_SkyBox;
	
	currentState = GameStateType::STATE_HEIGHT_MAP;
	gameStates[currentState]->OnEnterState();
}

void GameStateManager::Destroy()
{
	gameStates[currentState]->OnLeaveState();
	for ( int i = 0; i < GameStateType::NUM_OF_STATE; ++i )
	{
		if ( gameStates[i] != nullptr )
		{
			SAFE_DELETE(gameStates[i]);
		}
	}
}

void GameStateManager::Update()
{
	gameStates[currentState]->Update();
}

void GameStateManager::Render()
{
	gameStates[currentState]->Render();
}

LRESULT GameStateManager::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return gameStates[currentState]->InputProc(hWnd, message, wParam, lParam);
	return 0;
}

bool GameStateManager::ChangeGameState(GameStateType state)
{
	bool result = false;
	if ( gameStates[state] )
	{
		gameStates[currentState]->OnLeaveState();
		currentState = state;
		gameStates[currentState]->OnEnterState();

		result = true;
	}

	return result;
}
