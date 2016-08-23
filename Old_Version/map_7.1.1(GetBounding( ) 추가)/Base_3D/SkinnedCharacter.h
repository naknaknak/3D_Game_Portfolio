#pragma once
#include "SkinnedMesh.h"
#include "HeightMap.h"

//character�� ����, �÷��̾�, NPC, �̴Ͼ��� �θ� �� ���̴�.
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
	CHARACTER_DEAD, //�״���
	CHARACTER_DIED, //������
	CHARACTER_HIT, //����
	NUM_OF_CHARACTERSTATE
};
class SkinnedCharacter :	// �߻�Ŭ����
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
	bool isHit = false; //����
	int hp;
	bool isInvisible = false; //����
	std::vector<float> cooltime;
	
};

