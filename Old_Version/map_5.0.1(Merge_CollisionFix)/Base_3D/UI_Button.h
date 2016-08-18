#pragma once
#include <functional>
#include "UI_Base.h"

enum ButtonState
{
	STATE_NORMAL,
	STATE_OVER,
	STATE_PRESS,
	NUM_OF_BUTTON_STATE
};

class UI_Button :
	public UI_Base
{
public:
	UI_Button();
	virtual ~UI_Button();

	virtual void Render() override;
	virtual LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	void SetTexture(char* normalName, char* overName, char* pressName);

	inline void SetState(ButtonState _state){ state = _state; }

	std::function<void()> OnClick = nullptr;

protected:
	ButtonState state = ButtonState::STATE_NORMAL;
	LPDIRECT3DTEXTURE9 textures[NUM_OF_BUTTON_STATE];

};

