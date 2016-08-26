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
	 
	if (monsterHealthBar == nullptr)
	{
		monsterHealthBar = new progressBar;
		monsterHealthBar->init("./UI/monster_black.png", "./UI/monster_red.png", D3DXVECTOR3(RESOLUTION_X * 0.5 - 265.f , 100.f, 0.f));
	}
}
void ChracterStatusBar::Destroy( )
{
	SAFE_DELETE(ball_red);
	SAFE_DELETE(ball_blue);
	SAFE_DELETE(status);
}

void ChracterStatusBar::Update( )
{
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
	if(bShowMonsterHp) monsterHealthBar->Render( );
}