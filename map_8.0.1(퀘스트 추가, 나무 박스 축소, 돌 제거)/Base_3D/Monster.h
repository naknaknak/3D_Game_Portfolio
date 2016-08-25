#pragma once
#include "SkinnedCharacter.h"
class Monster :
	public SkinnedCharacter
{
public:
	Monster();
	virtual ~Monster();
	virtual void Update()=0;
	inline void SetPlayer(SkinnedCharacter* inputPlayer) { player = inputPlayer; }
	inline void SetInitialPosition(D3DXVECTOR3 inputInitialPosition) { initialPositon = inputInitialPosition; }
	inline void SetMonsters(std::vector<Monster*> inputMonsters) { monsters = inputMonsters; }

protected:
	virtual void ProcessState()=0;
	virtual void SetPosition(D3DXVECTOR3 newPostion);
	D3DXVECTOR3 rotateAxis = D3DXVECTOR3(0, 1, 0);
	float rotationAngle = 0.0f;
	float moveSpeed = 20.0f;
	D3DXMATRIXA16 translate;
	D3DXMATRIXA16 scale;
	D3DXMATRIXA16 rotation;
	D3DXVECTOR3 initialPositon;

	BoundingSphere forwardBoundingSphere = BoundingSphere();
	
	BoundingSphere sight_wide= BoundingSphere();
	BoundingSphere sight_narrow= BoundingSphere();
	BoundingSphere areaBoundingSphere= BoundingSphere();
	SkinnedCharacter* player = nullptr;
	D3DXVECTOR3* target=nullptr;
	std::vector<Monster*> monsters;
};

