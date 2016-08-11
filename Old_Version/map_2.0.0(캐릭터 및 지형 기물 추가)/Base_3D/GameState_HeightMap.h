#pragma once
#include "GameState.h"
#include "Grid.h"
#include "HeightMap.h"
#include "Character_HeightMapMove.h"
#include "SkyBox.h"
#include "Player.h"

#include "Rocks_save.h"


class GameState_HeightMap :
	public GameState
{
public:
	GameState_HeightMap();
	virtual ~GameState_HeightMap();

	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Reset() override;
	virtual void Update() override;
	virtual void Render() override;

	virtual void OnEnterState() override;
	virtual void OnLeaveState() override;

	virtual LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

protected:
	Grid* grid = nullptr;
	HeightMap* heightMap = nullptr;
	Character_HeightMapMove* girl = nullptr;
	SkyBox* sky = nullptr;
	Player* goat = nullptr;

	Rocks_save* rock = nullptr;

};

