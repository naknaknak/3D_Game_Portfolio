#pragma once
#include "SkinnedCharacter.h"
enum PlayerState
{
	PLAYER_IDLE_MOVE = 0,
	PLAYER_SPRINT,
	PLAYER_ATTACK,
	PLAYER_DODGE,
	PLAYER_JUMP,
	PLAYER_DEAD, //Á×´ÂÁß
	PLAYER_DIED, //´ÙÁ×À½
	NUM_OF_PLAYERSTATE
};
class Player :
	public SkinnedCharacter
{
public:
	Player();
	virtual ~Player();
	virtual void UpdateAndRender();

	virtual void ChangeCharaterState(CharacterState state);

protected:
	D3DXVECTOR3 rotateAxis = D3DXVECTOR3(0, 1, 0);
	float rotationAngle = 0.0f;
	float moveSpeed = 10.0f;

	PlayerState currentState = PLAYER_IDLE_MOVE;

	std::vector<SkinnedCharacter*> monsters;
};

