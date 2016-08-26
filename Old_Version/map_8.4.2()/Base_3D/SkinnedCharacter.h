#pragma once
#include "SkinnedMesh.h"
#include "HeightMap.h"

//character는 보스, 플레이어, NPC, 미니언의 부모가 될 것이다.
enum CharacterState
{
		CHARACTER_IDLE = 0,
	CHARACTER_MOVE ,
	CHARACTER_PATROL, //minion용
	CHARACTER_SPRINT,
	CHARACTER_TRACE,
	CHARACTER_ATTACK,
	CHARACTER_ATTACK1,//주인공은 평타 3타를 때린다
	CHARACTER_ATTACK2,
	CHARACTER_ATTACK3,
	CHARACTER_DODGE,//염소가 dodge를 하나만 쓴다
	CHARACTER_DODGE_F,//보스와 주인공은 dodge를 네개씩 쓴다
	CHARACTER_DODGE_B,
	CHARACTER_DODGE_R,
	CHARACTER_DODGE_L,
	CHARACTER_JUMP,
	CHARACTER_SKILL1,
	CHARACTER_SKILL2,
	CHARACTER_SKILL3,
	CHARACTER_SKILL4,
	CHARACTER_DEAD, //죽는중
	CHARACTER_DIED, //다죽음
	CHARACTER_HIT, //맞음
	CHARACTER_RETURN,
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
	

	inline D3DXVECTOR3 GetPosition() { return position; }
	inline D3DXVECTOR3* GetPositionAddress(){ return &position; }
	inline void SetHeightMap(HeightMap* inputMap) { hm = inputMap; }
	inline bool GetIsHit() { return isHit; }
	inline bool GetInvisible() { return isInvisible; }
	inline bool GetIsDead() { return isDead; }
protected:
	virtual void InitializeAnimation();
	virtual void Hit(float damage);

	HeightMap* hm = nullptr;
	std::map<CharacterState,std::string> animationNames;
	CharacterState currentState = CharacterState::CHARACTER_IDLE;
	bool isHit = false; //맞음
	bool isDead = false; //죽음
	float hp;
	bool isInvisible = false; //무적
	bool attackHit = false;
	std::vector<float> cooltime;
	
};

