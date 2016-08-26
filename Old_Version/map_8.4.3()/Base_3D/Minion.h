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
	virtual float GetMaxHP()override { return 100.0f; }

protected:
	void ProcessState() override;
	void SelectPatrolPosition();
	bool CollisionMonsters(D3DXVECTOR3* direction);
	virtual void Hit(float damage);

	BoundingSphere attackSphere = BoundingSphere();

	static const float ATTACK_DAMAGE;
	float hp = 100.0f;
	D3DXVECTOR3 patrolPosition;
	bool rotateCW = true;
	float idleTime = 0.0f;
	float roationAngle = 0.0f;
	float inDamage = 0.0f;
};


