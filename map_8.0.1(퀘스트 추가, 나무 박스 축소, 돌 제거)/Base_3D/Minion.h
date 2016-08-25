#pragma once
#include "Monster.h"
class Minion :
	public Monster
{
public:
	Minion();
	virtual ~Minion();
	virtual void Update() override;
	virtual void Initialize(char* path, char* filename) override;
	
	virtual void ChangeCharacterState(CharacterState state);
	

protected:
	void ProcessState() override;
	void SelectPatrolPosition();
	bool CollisionMonsters(D3DXVECTOR3* direction);
	
	BoundingSphere attackSphere = BoundingSphere();

	static const float ATTACK_DAMAGE;
	float hp = 100.0f;
	D3DXVECTOR3 patrolPosition;
	bool rotateCW = true;
	float idleTime = 0.0f;
	float roationAngle = 0.0f;
	
};


