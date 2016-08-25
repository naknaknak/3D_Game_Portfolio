#include "stdafx.h"
#include "UI_Image.h"


UI_Image::UI_Image()
{
}


UI_Image::~UI_Image()
{
	texture = nullptr;
}


void UI_Image::Render()
{
	if (isShow != true)
	{
		return;
	}

	RECT rc = { 0, 0, (int)size.x, (int)size.y };

	UI_Manager::GetSprite()->SetTransform(&world);
	UI_Manager::GetSprite()->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	UI_Manager::GetSprite()->Draw(
		texture,
		&rc,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_XRGB(255, 255, 255));
	UI_Manager::GetSprite()->End();
	UI_Base::Render();

}

void UI_Image::SetTexture(char* fileName)
{
	D3DXIMAGE_INFO imageInfo;
	texture = TextureManager::GetTexture(fileName, &imageInfo);
	size.x = (float)imageInfo.Width;
	size.y = (float)imageInfo.Height;

}