#pragma once
#include "Monster.h"
class Boss :
	public Monster
{
public:
	Boss();
	virtual ~Boss();
	virtual void Update() override;
	virtual void Initialize(char* path, char* filename);
	virtual float GetMaxHP()override { return 250.0f; }
	virtual float GetHP( )override { return hp; }
	virtual void ChangeCharacterState(CharacterState state);

protected:
	void ProcessState();
	virtual void Hit(float damage);
	bool CollisionMonsters(D3DXVECTOR3* direction);

	BoundingSphere attackSphere;
	float hp = 250.0f;
	float rotationAngle = D3DX_PI; //원래 보고있는 방향이 플레이어 길 반대쪽이다. 방향을 좀 틀어줌
	static const float ATTACK_DAMAGE;
	static const float ATTACK_RANGE;
	static const float BOSS_SIGHT_RANGE;
};

