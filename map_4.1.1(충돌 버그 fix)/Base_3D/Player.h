#pragma once
#include "SkinnedCharacter.h"

class Player :
	public SkinnedCharacter
{
public:
	Player();
	virtual ~Player();
	
	virtual void Destroy( ) override;

	virtual void Update() override;

	void Debuging( );
	

	virtual void ChangeCharacterState(CharacterState state) override;
	//void ChangePlayerState(PlayerState state);

protected:
	D3DXVECTOR3 rotateAxis = D3DXVECTOR3(0, 1, 0);
	float rotationAngle = 0.0f;
	float moveSpeed = 10.0f;

	CharacterState currentState = CHARACTER_IDLE;

	//½ºÅ³
	D3DXVECTOR3 skill1Pos = position;
	BoundingSphere skill1Sphere = BoundingSphere();
	static const int maxSkill1Radius = 20;
	static const float skillCastingTime;
	

	std::vector<SkinnedCharacter*> monsters;
	

};

