#pragma once
#include "SkinnedMesh.h"

//character�� ����, �÷��̾�, NPC, �̴Ͼ��� �θ� �� ���̴�.
enum CharacterState
{
	CHARACTER_IDLE_MOVE = 0,
	CHARACTER_SPRINT,
	CHARACTER_ATTACK,
	CHARACTER_DODGE,
	CHARACTER_JUMP,
	CHARACTER_DEAD, //�״���
	CHARACTER_DIED, //������
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

