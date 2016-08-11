#include "stdafx.h"
#include "Character_HeightMapMove.h"


Character_HeightMapMove::Character_HeightMapMove()
{
}


Character_HeightMapMove::~Character_HeightMapMove()
{
}

void Character_HeightMapMove::GroundCheck()
{
	D3DXVECTOR3 pos = GetPosition();
	if ( hm->GetHeight(pos, pos.x, pos.z) == false )
	{
		float tick = (float)GameManager::GetTick();
		if ( (GetAsyncKeyState('W') & 0x8000) != 0 )
		{
			pos -= (-direction * moveSpeed * tick);
		}
		else if ( (GetAsyncKeyState('S') & 0x8000) != 0 )
		{
			pos += (-direction * moveSpeed * tick);
		}
	}
	SetPosition(pos);
}
