#pragma once
#include "SkinnedMesh.h"
#include "HeightMap.h"

//character�� ����, �÷��̾�, NPC, �̴Ͼ��� �θ� �� ���̴�.
enum CharacterState
{
		CHARACTER_IDLE = 0,
	CHARACTER_MOVE ,
	CHARACTER_PATROL, //minion��
	CHARACTER_SPRINT,
	CHARACTER_TRACE,
	CHARACTER_ATTACK,
	CHARACTER_ATTACK1,//���ΰ��� ��Ÿ 3Ÿ�� ������
	CHARACTER_ATTACK2,
	CHARACTER_ATTACK3,
	CHARACTER_DODGE,//���Ұ� dodge�� �ϳ��� ����
	CHARACTER_DODGE_F,//������ ���ΰ��� dodge�� �װ��� ����
	CHARACTER_DODGE_B,
	CHARACTER_DODGE_R,
	CHARACTER_DODGE_L,
	CHARACTER_JUMP,
	CHARACTER_SKILL1,
	CHARACTER_SKILL2,
	CHARACTER_SKILL3,
	CHARACTER_SKILL4,
	CHARACTER_DEAD, //�״���
	CHARACTER_DIED, //������
	CHARACTER_HIT, //����
	CHARACTER_RETURN,
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
	bool isHit = false; //����
	bool isDead = false; //����
	float hp;
	bool isInvisible = false; //����
	bool attackHit = false;
	std::vector<float> cooltime;
	
};

