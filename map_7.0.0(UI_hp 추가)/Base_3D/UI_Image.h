#pragma once
#include "UI_Base.h"
class UI_Image :
	public UI_Base
{
public:
	UI_Image();
	virtual ~UI_Image();

	virtual void Render() override;

	void SetTexture(char* fileName);	

protected:
	LPDIRECT3DTEXTURE9 texture = nullptr;
};

