#include "stdafx.h"
#include "UI_Functions.h"


UI_Functions::UI_Functions()
{
}


UI_Functions::~UI_Functions()
{
}

void UI_Functions::ButtonClick()
{
	OutputDebugStringA("Button Click");

}

void UI_Functions::ButtonClick2(UI_Base* ui)
{
	D3DXMATRIXA16 mat = ui->GetWorld();
	ui->SetPosition(mat._41 + 50.0f, mat._42, mat._43);
}

void UI_Functions::ButtonOK(UI_Base* ui)
{
	UI_Text* text = (UI_Text*)ui;
	text->SetText("Accept");
}

void UI_Functions::ButtonCancel(UI_Base* ui)
{
	UI_Text* text = (UI_Text*)ui;
	text->SetText("Cancel");
}