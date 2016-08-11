#include "stdafx.h"
#include "UI_Button.h"


UI_Button::UI_Button()
{
	ZeroMemory(textures, sizeof(LPDIRECT3DTEXTURE9)*ButtonState::NUM_OF_BUTTON_STATE);
}


UI_Button::~UI_Button()
{
	ZeroMemory(textures, sizeof(LPDIRECT3DTEXTURE9)*ButtonState::NUM_OF_BUTTON_STATE);
}

void UI_Button::Render()
{
	if (isShow != true)
	{
		return;
	}

	RECT rc = { 0, 0, (int)size.x, (int)size.y };

	UI_Manager::GetSprite()->SetTransform(&world);
	UI_Manager::GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	UI_Manager::GetSprite()->Draw(
		textures[state],
		&rc,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_XRGB(255, 255, 255)
		);
	UI_Manager::GetSprite()->End();

	UI_Base::Render();
}

LRESULT UI_Button::InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (isShow != true)
	{
		return -1;
	}

	LRESULT result = 0;
	result = UI_Base::InputProc(hWnd, message, wParam, lParam);
	if (result == 0)
	{
		RECT buttonRect = {
			(int)world._41, (int)world._42,
			(int)(world._41 + size.x),
			(int)(world._42 + size.y)
		};

		switch (message)
		{
		case WM_LBUTTONDOWN:
			{
				POINT mousePoint;
				mousePoint.x = GET_X_LPARAM(lParam);
				mousePoint.y = GET_Y_LPARAM(lParam);

				if (mousePoint.x > buttonRect.left 
					&& mousePoint.x < buttonRect.right
					&& mousePoint.y > buttonRect.top 
					&& mousePoint.y < buttonRect.bottom)
				{
					state = ButtonState::STATE_PRESS;
					result = WM_LBUTTONDOWN;
				}
			}
			break;
		case WM_LBUTTONUP:
			{
				POINT mousePoint;
				mousePoint.x = GET_X_LPARAM(lParam);
				mousePoint.y = GET_Y_LPARAM(lParam);

				if (mousePoint.x > buttonRect.left
					&& mousePoint.x < buttonRect.right
					&& mousePoint.y > buttonRect.top
					&& mousePoint.y < buttonRect.bottom)
				{
					state = ButtonState::STATE_OVER;
					if (OnClick != nullptr)
					{
						OnClick();						
					}
					result = WM_LBUTTONUP;
				}
				else
				{
					state = ButtonState::STATE_NORMAL;
				}

				
			}
			break;
		case WM_MOUSEMOVE:
			{
				POINT mousePoint;
				mousePoint.x = GET_X_LPARAM(lParam);
				mousePoint.y = GET_Y_LPARAM(lParam);

				if (mousePoint.x > buttonRect.left
					&& mousePoint.x < buttonRect.right
					&& mousePoint.y > buttonRect.top
					&& mousePoint.y < buttonRect.bottom)
				{
					if (state == ButtonState::STATE_NORMAL)
					{
						state = ButtonState::STATE_OVER;
					}
				}
				else
				{
					if (state == ButtonState::STATE_OVER)
					{
						state = ButtonState::STATE_NORMAL;
					}
				}
			}
			break;
		}
	}

	return result;
}

void UI_Button::SetTexture(char* normalName, char* overName, char* pressName)
{
	D3DXIMAGE_INFO imageInfo;
	textures[ButtonState::STATE_NORMAL] = 
		TextureManager::GetTexture(
		normalName, &imageInfo);
	size.x = (float)imageInfo.Width;
	size.y = (float)imageInfo.Height;
	textures[ButtonState::STATE_OVER] =
		TextureManager::GetTexture(overName);
	textures[ButtonState::STATE_PRESS] =
		TextureManager::GetTexture(pressName);
}