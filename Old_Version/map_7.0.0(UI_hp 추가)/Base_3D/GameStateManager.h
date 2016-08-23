#pragma once
#include "GameState.h"
#include "GameState_HeightMap.h"
#include "GameState_SkyBox.h"


enum GameStateType
{
	STATE_OBJ_LOAD = 0,				//OBJ���Ϸ� �𵨱׸���(�ٸ� ���� ������� ���� ���̵� Ȯ��)
	STATE_HEIGHT_MAP,
	STATE_SKYBOX,
	NUM_OF_STATE
};

class GameStateManager
{
private:
	GameStateManager();
	GameStateManager(const GameStateManager& other);
	~GameStateManager();

public:
	static GameStateManager& Get()
	{
		static GameStateManager instance;
		return instance;
	}

	void Initialize();
	void Destroy();
	void Update();
	void Render();
	LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	bool ChangeGameState(GameStateType state);

protected:
	GameStateType currentState = GameStateType::STATE_OBJ_LOAD;
	GameState* gameStates[GameStateType::NUM_OF_STATE];
};

