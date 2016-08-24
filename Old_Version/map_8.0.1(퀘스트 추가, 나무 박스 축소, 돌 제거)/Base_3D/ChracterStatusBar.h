#pragma once

#include "UI_Base.h"
#include "image.h"
#include "progressBar.h"

class ChracterStatusBar
{
private:
	/// player
	image* status = nullptr;
	progressBar* ball_red = nullptr;
	progressBar* ball_blue = nullptr;
	/// monster
	progressBar* monsterHealthBar = nullptr;

	// 테스트용
	float nCurrent = 0;

public:
	ChracterStatusBar( );
	~ChracterStatusBar( );

	void Initialize( );
	void Destroy( );
	void Update( );
	void Render( );

	void GetPlayerHP(float& hp, float maxHP) { if (ball_red) ball_red->Gauge_Height(hp, maxHP); };
	void GetPlayerMP(float& mp, float maxMP) { if (ball_blue) ball_blue->Gauge_Height(mp, maxMP); };
	void GetMonsterHP(float& hp, float maxHP) {if(monsterHealthBar) monsterHealthBar->Gauge_Height(hp, maxHP); };

};

