#pragma once
#include "SkinnedMesh.h"
#include "HeightMap.h"

//character는 보스, 플레이어, NPC, 미니언의 부모가 될 것이다.
enum CharacterState
{
	CHARACTER_IDLE = 0,
	CHARACTER_MOVE ,
	CHARACTER_SPRINT,
	CHARACTER_ATTACK,
	CHARACTER_DODGE,
	CHARACTER_JUMP,
	CHARACTER_SKILL1,
	CHARACTER_SKILL2,
	CHARACTER_SKILL3,
	CHARACTER_SKILL4,
	CHARACTER_DEAD, //죽는중
	CHARACTER_DIED, //다죽음
	CHARACTER_HIT, //맞음
	NUM_OF_CHARACTERSTATE
};
class SkinnedCharacter :	// 추상클래스
	public SkinnedMesh
{
public:
	SkinnedCharacter();
	virtual ~SkinnedCharacter();

	virtual void ChangeCharacterState(CharacterState state) = 0;

	

	virtual void DealDamage(SkinnedCharacter* hitCharacter, int damage);
	virtual void Hit(int damage);

	inline D3DXVECTOR3 GetPosition() { return position; }
	inline D3DXVECTOR3* GetPositionAddress(){ return &position; }
	inline void SetHeightMap(HeightMap* inputMap) { hm = inputMap; }

protected:
	virtual void InitializeAnimation();
	virtual void InitializeCoolTime()=0;

	HeightMap* hm = nullptr;
	std::map<CharacterState,std::string> animationNames;
	CharacterState currentState = CharacterState::CHARACTER_IDLE;
	bool isHit = false; //맞음
	int hp;
	bool isInvisible = false; //무적
	std::vector<float> cooltime;
	
};

