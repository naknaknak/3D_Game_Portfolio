#include "stdafx.h"
#include "progressBar.h"



progressBar::progressBar( )
{
}


progressBar::~progressBar( )
{
}

HRESULT progressBar::init(char* progressBarBack, char* progressBarFront, D3DXVECTOR3& pos)
{
	SetTexture(progressBarBack, progressBarFront);

	position = pos;

	fGauge_Width = size.x;
	fGauge_Height = size.y;

	//nCurrent = 100;
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&matParent);
	D3DXMatrixIdentity(&matIdentity);

	return S_OK;
}

void progressBar::SetTexture(char* progressBarBack, char* progressBarFront)
{
	D3DXIMAGE_INFO imageInfo;
	this->progressBarBack = TextureManager::GetTexture(progressBarBack, &imageInfo);
	this->progressBarFront = TextureManager::GetTexture(progressBarFront, &imageInfo);

	assert(progressBarBack != nullptr && "텍스쳐 없음");
	assert(progressBarFront != nullptr && "텍스쳐 없음");

	size.x = (float)imageInfo.Width;
	size.y = (float)imageInfo.Height;
	fGauge_Height = 0.f;
	fGauge_Width = 0.f;
}
void progressBar::Destroy( )
{

}

void progressBar::Update( )
{
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);

	if (matParent != matIdentity)
	{
		world *= matParent;
	}


}

void progressBar::Render( )
{

	RECT rc_Back = { 0, 0, (LONG)size.x, (LONG)size.y };
	UI_Manager::GetSprite( )->SetTransform(&world);
	UI_Manager::GetSprite( )->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	UI_Manager::GetSprite( )->Draw(
		progressBarBack,
		&rc_Back,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_XRGB(255, 255, 255));
	UI_Manager::GetSprite( )->End( );

	if (bWidth)
	{
		RECT rc_Front = { 0, 0, (LONG)fGauge_Width,  (LONG)size.y };
		UI_Manager::GetSprite( )->SetTransform(&world);
		UI_Manager::GetSprite( )->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
		UI_Manager::GetSprite( )->Draw(
			progressBarFront,
			&rc_Front,
			&D3DXVECTOR3(0, 0, 0),
			&D3DXVECTOR3(0, 0, 0),
			D3DCOLOR_XRGB(255, 255, 255));
		UI_Manager::GetSprite( )->End( );
	}
	else
	{
		RECT rc_Front = { 0, (LONG)fGauge_Height, (LONG)fGauge_Width,  (LONG)size.y };
		UI_Manager::GetSprite( )->SetTransform(&world);
		UI_Manager::GetSprite( )->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
		UI_Manager::GetSprite( )->Draw(
			progressBarFront,
			&rc_Front,
			&D3DXVECTOR3(0, 0, 0),
			&D3DXVECTOR3(0, (LONG)fGauge_Height, 0),
			D3DCOLOR_XRGB(255, 255, 255));
		UI_Manager::GetSprite( )->End( );
	}
}

//체력바 게이지
void progressBar::Gauge_Width(float &currentGauge, float maxGauge)
{
	fGauge_Height = 0.f;
	fGauge_Width = 0.f;
	if (currentGauge > maxGauge)
	{
		currentGauge = maxGauge;
	}
	if (currentGauge < 0)
	{
		currentGauge = 0;
	}
	bWidth = true;
	fGauge_Width = (currentGauge / maxGauge) * size.x;
}
void progressBar::Gauge_Height(float &currentGauge, float maxGauge)
{
	fGauge_Height = 0.f;
	fGauge_Width = 0.f;
	if (currentGauge > maxGauge)
	{
		currentGauge = maxGauge;
	}
	if (currentGauge < 0)
	{
		currentGauge = 0;
	}
	bWidth = false;

	fGauge_Height = ((maxGauge - currentGauge) / maxGauge) * size.y;
}