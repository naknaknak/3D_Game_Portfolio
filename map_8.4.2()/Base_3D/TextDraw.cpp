#include "stdafx.h"
#include "TextDraw.h"


TextDraw::TextDraw( )
{
}

TextDraw::~TextDraw( )
{
}


void TextDraw::init()
{
	D3DXMatrixIdentity(&world);
	D3DXMatrixIdentity(&matParent);
	D3DXMatrixIdentity(&matIdentity);

	D3DXMatrixTranslation(&world, position.x, position.y, position.z);

}

void TextDraw::Destroy( )
{
}

void TextDraw::Update( )
{
	D3DXMatrixIdentity(&world);
	D3DXMatrixTranslation(&world, position.x, position.y, position.z);

	if (matParent != matIdentity)
	{
		world *= matParent;
	}

}

void TextDraw::Render( )
{
	RECT rc = { (int)world._41, (int)world._42, (int)(world._41 + size.x), (int)(world._42 + size.y) };
	int len = text.length( );
	int strlenee = strlen(text.c_str( ));
	UI_Manager::GetFont( )->DrawTextA(NULL, text.c_str( ), strlen(text.c_str( )), &rc, textFormat, textColor);
	//font->DrawTextA(NULL, "GameState_HeightMap", strlen("GameState_HeightMap"), &rc, textFormat, textColor);

}





