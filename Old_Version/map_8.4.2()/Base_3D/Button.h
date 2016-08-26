#pragma once

#include "Button.h"

class Button
{
public:
	enum ButtonType
	{
		STATE_NORMAL,
		STATE_OVER,
		STATE_PRESS,
		NUM_OF_BUTTON_STATE
	};

public:
	Button( );
	~Button( );

	void Initialize(char * normalName, char * overName, char * pressName, D3DXVECTOR3 & pos);
	void Destroy( );
	void Update( );
	void Render( );

	void ClickAction(bool & selectOK);

	inline D3DXMATRIXA16& GetWorld( ) { return world; }
	inline D3DXVECTOR2& GetSize( ) { return size; }

	inline void SetParent(D3DXMATRIXA16& myParent) { matParent = myParent; }
	void SetTexture(char* normalName, char* overName, char* pressName);
	void SetText(std::string text) { text_buttonOK.SetText(text); };

protected:
	ButtonType state = ButtonType::STATE_NORMAL;
	LPDIRECT3DTEXTURE9 pTextures[NUM_OF_BUTTON_STATE];

	D3DXMATRIXA16 world;
	D3DXMATRIXA16 matParent;
	D3DXMATRIXA16 matIdentity;

	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);	 // 위치
	D3DXVECTOR2 size = D3DXVECTOR2(0, 0);			// 크기

	TextDraw text_buttonOK;

};

