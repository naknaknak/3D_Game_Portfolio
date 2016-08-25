#include "stdafx.h"
#include "image.h"


image::image( )
{
}


image::~image( )
{
}

void image::init(char * filename, D3DXVECTOR3 & pos)
{
	D3DXIMAGE_INFO imageInfo;
	pTexture = TextureManager::GetTexture(filename, &imageInfo);

	assert(pTexture != nullptr && "텍스쳐 없음");

	size.x = (float)imageInfo.Width;
	size.y = (float)imageInfo.Height;

	position = pos;

	const float half_x = size.x * 0.5f;
	const float half_y = size.y * 0.5f;

	position.x -= half_x;
	position.y -= half_y;

	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&matParent);
	D3DXMatrixIdentity(&matIdentity);

	D3DXMatrixTranslation(&world, position.x, position.y, position.z);

}

void image::Destroy( )
{
}

void image::Update( )
{
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);

	if (matParent != matIdentity)
	{
		world *= matParent;
	}

}

void image::Render( )
{
	RECT rc_Back = { 0, 0, (LONG)size.x, (LONG)size.y };
	UI_Manager::GetSprite( )->SetTransform(&world);
	UI_Manager::GetSprite( )->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);
	UI_Manager::GetSprite( )->Draw(
		pTexture,
		&rc_Back,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),
		D3DCOLOR_XRGB(255, 255, 255));
	UI_Manager::GetSprite( )->End( );
}

