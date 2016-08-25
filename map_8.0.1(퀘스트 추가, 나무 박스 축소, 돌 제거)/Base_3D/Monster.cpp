#include "stdafx.h"
#include "Monster.h"


Monster::Monster()
{
}


Monster::~Monster()
{
}

void Monster::SetPosition(D3DXVECTOR3 newPosition)
{
	SkinnedMesh::SetPosition(newPosition);

	sight_wide.center = newPosition;
	sight_narrow.center = newPosition;

}
void Monster::Hit(float damage)
{
	isHit = true;
	inDamage = damage;

}