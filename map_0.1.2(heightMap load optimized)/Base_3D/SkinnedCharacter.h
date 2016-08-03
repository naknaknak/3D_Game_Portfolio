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

	inline void SetHeightMap(HeightMap* inputMap) { hm = inputMap; }
	virtual void ChangeCharaterState(CharacterState state) = 0;

protected:
	
	HeightMap* hm = nullptr;
	virtual void Update(D3DXMATRIXA16 * local) = 0;
};

