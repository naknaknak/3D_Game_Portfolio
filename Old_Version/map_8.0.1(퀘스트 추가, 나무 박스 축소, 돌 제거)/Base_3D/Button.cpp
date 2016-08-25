#include "stdafx.h"
#include "Button.h"


Button::Button( )
{
}


Button::~Button( )
{
}
void Button::Initialize(char* normalName, char* overName, char* pressName, D3DXVECTOR3& pos)
{
	SetTexture(normalName, overName, pressName);

	position = pos;

	const float half_x = size.x * 0.5f;
	const float half_y = size.y * 0.5f;

	position.x -= half_x;
	position.y -= half_y;

	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&matParent);
	D3DXMatrixIdentity(&matIdentity);

	D3DXMatrixTranslation(&world, position.x, position.y, position.z);

	text_buttonOK.init( );
	text_buttonOK.SetDrawSize(size);

	char* str = "- ¼ö ¶ô -";
	int nlen = strlen(str);

	//text_buttonOK.SetPosition(D3DXVECTOR3(size.x * 0.5 - nlen * 0.5 * 10, size.y * 0.5 - 10, 0));
	text_buttonOK.SetText(str);
	text_buttonOK.SetPositionCenter(D3DXVECTOR3(size.x * 0.5 - nlen * 0.5 * 10, size.y * 0.5 - 10, 0));

}
void Button::Destroy( )
{
}
void Button::Update( )
{
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);

	if (matParent != matIdentity)
	{
		world *= matParent;
	}

	text_buttonOK.SetParent(world);
	text_buttonOK.Update( );

}
void Button::Render( )
{
	RECT rc = { 0, 0, (int)size.x, (int)size.y };

	UI_Manager::GetSprite( )->SetTransform(&world);
	UI_Manager::GetSprite( )->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	UI_Manager::GetSprite( )->Draw(
		pTextures[state],
		&rc,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_XRGB(255, 255, 255)
	);
	UI_Manager::GetSprite( )->End( );

	text_buttonOK.Render( );

}
void Button::ClickAction(bool& selectOK)
{
	RECT buttonRect = { (int)world._41, (int)world._42, (int)(world._41 + size.x), (int)(world._42 + size.y) };
	// mouse_ move
	if ((g_MousePoint.x > buttonRect.left	&& g_MousePoint.x < buttonRect.right)
		&& (g_MousePoint.y > buttonRect.top && g_MousePoint.y < buttonRect.bottom))
	{
		/*if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000))*/
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			state = ButtonType::STATE_PRESS;
		}
		else
		{
			state = ButtonType::STATE_OVER;
		}

		if (state == ButtonType::STATE_PRESS)
		{
			// Execute
			selectOK = true;
		}
	}
	else
	{
		state = ButtonType::STATE_NORMAL;
	}
}

void Button::SetTexture(char* normalName, char* overName, char* pressName)
{
	D3DXIMAGE_INFO imageInfo;
	pTextures[ButtonType::STATE_NORMAL] = TextureManager::GetTexture(normalName, &imageInfo);
	pTextures[ButtonType::STATE_OVER] = TextureManager::GetTexture(overName);
	pTextures[ButtonType::STATE_PRESS] = TextureManager::GetTexture(pressName);
	size.x = (float)imageInfo.Width;
	size.y = (float)imageInfo.Height;
}