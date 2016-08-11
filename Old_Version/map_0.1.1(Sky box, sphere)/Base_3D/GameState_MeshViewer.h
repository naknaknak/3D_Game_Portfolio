#pragma once
#include "GameState.h"
#include "MeshViewer.h"

class GameState_MeshViewer:
	public GameState
{
public:
	GameState_MeshViewer();
	~GameState_MeshViewer();

	virtual void Initialize() override;
	virtual void Destroy() override;
	virtual void Reset() override;
	virtual void Update() override;
	virtual void Render() override;

	virtual void OnEnterState() override;
	virtual void OnLeaveState() override;

	virtual LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

protected:
	MeshViewer* mv = nullptr;

};

