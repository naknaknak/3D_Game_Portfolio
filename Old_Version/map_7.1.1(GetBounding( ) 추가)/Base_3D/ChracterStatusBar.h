#pragma once

#include "UI_Base.h"
#include "image.h"
#include "progressBar.h"

class ChracterStatusBar
{
private:
	image* status = nullptr;
	progressBar* ball_red = nullptr;
	progressBar* ball_blue = nullptr;

	// 테스트용
	float nCurrent = 0;

public:
	ChracterStatusBar( );
	~ChracterStatusBar( );

	void Initialize( );
	void Destroy( );
	void Update( );
	void Render( );


};

