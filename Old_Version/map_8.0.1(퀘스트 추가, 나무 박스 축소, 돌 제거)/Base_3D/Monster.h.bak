#pragma once
#include "SkinnedCharacter.h"
class Monster :
	public SkinnedCharacter
{
public:
	Monster();
	virtual ~Monster();
	virtual void Update()=0;
protected:
	D3DXVECTOR3 rotateAxis = D3DXVECTOR3(0, 1, 0);
	float rotationAngle = 0.0f;
	float moveSpeed = 20.0f;
	D3DXMATRIXA16 translate;
	D3DXMATRIXA16 scale;
	D3DXMATRIXA16 rotation;

	BoundingSphere forwardBoundingSphere = BoundingSphere();
};

