#pragma once
#include "GameState.h"
#include "Grid.h"
#include "SkyBox.h"

class GameState_SkyBox:
	public GameState
{
public:
	GameState_SkyBox();
	~GameState_SkyBox();

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
	SkyBox* sky = nullptr;
};

