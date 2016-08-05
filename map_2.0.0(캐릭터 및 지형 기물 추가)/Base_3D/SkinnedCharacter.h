#pragma once
#include "SkinnedMesh.h"

//character는 보스, 플레이어, NPC, 미니언의 부모가 될 것이다.
enum CharacterState
{
	CHARACTER_IDLE_MOVE = 0,
	CHARACTER_SPRINT,
	CHARACTER_ATTACK,
	CHARACTER_DODGE,
	CHARACTER_JUMP,
	CHARACTER_DEAD, //죽는중
	CHARACTER_DIED, //다죽음
	NUM_OF_CHARACTERSTATE
};
class SkinnedCharacter :
	public SkinnedMesh
{
public:
	SkinnedCharacter();
	virtual ~SkinnedCharacter();

	inline D3DXVECTOR3 GetPosition() { return position; }
	inline D3DXVECTOR3* GetPositionAddress(){ return &position; }
	inline void SetHeightMap(HeightMap* inputMap) { hm = inputMap; }
	virtual void ChangeCharaterState(CharacterState state) = 0;
	virtual void UpdateAndRender() = 0;
	virtual void InitializeAnimation()=0;


protected:
	
	HeightMap* hm = nullptr;
	std::map<CharacterState,std::string> animationNames;
};

