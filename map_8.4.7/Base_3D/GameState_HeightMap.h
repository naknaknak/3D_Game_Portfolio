#pragma once
#include "GameState.h"
#include "Grid.h"
#include "HeightMap.h"
//#include "SkyBox.h"
#include "SkySphere.h"

#include "ChracterStatusBar.h"
#include "Quest.h"

#include "Player.h"

#include "PlantTree.h"
#include "Rocks_save.h"
#include "Minion.h"
#include "Boss.h"

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

	void Debugging( );

	virtual void OnEnterState() override;
	virtual void OnLeaveState() override;

	virtual LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	void DebuggingKey( );

protected:
	HeightMap* heightMap = nullptr;
	SkySphere* sky = nullptr;
	Player* lilith = nullptr;

	Rocks_save* rock = nullptr;
	PlantTree* trees = nullptr;
	std::vector<Monster*> monsters;

	ChracterStatusBar* statusBar = nullptr;
	Quest* quest = nullptr;

	bool selectOK[QUEST_NUM] = { false, false, false };
	int count_dead = 0;
	float monsterHP = 0.f;
};

