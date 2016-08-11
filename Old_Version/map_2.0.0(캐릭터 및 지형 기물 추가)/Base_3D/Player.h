#pragma once
#include "SkinnedCharacter.h"
enum PlayerState 
{
	PLAYER_IDLE =0,
	PLAYER_MOVE =1 ,
	PLAYER_SPRINT = 1<<1,
	PLAYER_ATTACK  = 1<<2,
	PLAYER_DODGE = 1<<3,
	PLAYER_JUMP = 1<<4,
	PLAYER_DEAD = 1<<5, //죽는중
	PLAYER_DIED = 1<<6, //다죽음

};
class Player :
	public SkinnedCharacter
{
public:
	Player();
	virtual ~Player();
	
	virtual void Destroy( ) override;

	virtual void UpdateAndRender();
	void Debuging( );
	virtual void InitializeAnimation() override;

	virtual void ChangeCharaterState(CharacterState state); //플레이어는 따로 쓴다. 몬스터가 공유한다.
	void ChangePlayerState(PlayerState state);
protected:
	D3DXVECTOR3 rotateAxis = D3DXVECTOR3(0, 1, 0);
	float rotationAngle = 0.0f;
	float moveSpeed = 10.0f;

	PlayerState currentState = PLAYER_IDLE;

	std::vector<SkinnedCharacter*> monsters;
	std::map<PlayerState, std::string> animationNames;

};

