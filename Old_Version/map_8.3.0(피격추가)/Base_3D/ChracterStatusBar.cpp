#include "stdafx.h"
#include "ChracterStatusBar.h"

ChracterStatusBar::ChracterStatusBar( )
{
}


ChracterStatusBar::~ChracterStatusBar( )
{
}

void ChracterStatusBar::Initialize( )
{
	// player
	if (status == nullptr)
	{
		status = new image;
		status->init("./UI/life_empty.png", D3DXVECTOR3(RESOLUTION_X * 0.5, RESOLUTION_Y - 57, 0));

		ball_red = new progressBar;
		ball_red->init("./UI/back.png", "./UI/red.png", D3DXVECTOR3(90, 2, 0));
		ball_red->SetParent(status->GetWorld( ));

		ball_blue = new progressBar;
		ball_blue->init("./UI/back.png", "./UI/blue.png", D3DXVECTOR3(653, 2, 0));
		ball_blue->SetParent(status->GetWorld( ));
	}
	// monster
	//if (monsterHealthBar == nullptr)
	//{
	//	monsterHealthBar = new progressBar;
	//	monsterHealthBar->init("./UI/back.png", "./UI/blue.png", D3DXVECTOR3(RESOLUTION_X * 0.5, 2, 0));
	//}
}
void ChracterStatusBar::Destroy( )
{
	SAFE_DELETE(ball_red);
	SAFE_DELETE(ball_blue);
	SAFE_DELETE(status);
}

void ChracterStatusBar::Update( )
{
	// 테스트용
	if (GetAsyncKeyState('K') & 0x8001)
	{
		nCurrent -= 5;
		ball_red->Gauge_Height(nCurrent, 256);
		ball_blue->Gauge_Height(nCurrent, 256);
		//Gauge_Width(nCurrent, 256);
	}
	if (GetAsyncKeyState('L') & 0x8001)
	{
		nCurrent += 5;
		ball_red->Gauge_Height(nCurrent, 256);
		ball_blue->Gauge_Height(nCurrent, 256);
	}
	if (status)
	{
		ball_red->Update( );
		ball_blue->Update( );
		status->Update( );
	}
	if (monsterHealthBar)
	{
		monsterHealthBar->Update( );
	}
}
void ChracterStatusBar::Render( )
{
	if (status)
	{
		ball_red->Render( );
		ball_blue->Render( );
		status->Render( );
	}
}