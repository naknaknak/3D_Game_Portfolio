#pragma once
#include "SkinnedCharacter.h"
#include "Tree.h"
class Player :
	public SkinnedCharacter
{
public:
	Player();
	virtual ~Player();
	
	virtual void Initialize(char* path, char* filename) override;

	virtual void Destroy( ) override;
	
	virtual void Update() override;
	void Debuging( );
	

	virtual void ChangeCharacterState(CharacterState state) override;
	void SetTrees(std::vector<Tree*> inputTrees) { trees = inputTrees; }
	//void ChangePlayerState(PlayerState state);

protected:
	D3DXVECTOR3 rotateAxis = D3DXVECTOR3(0, 1, 0);
	float rotationAngle = 0.0f;
	float moveSpeed = 20.0f;
	D3DXMATRIXA16 translate;
	D3DXMATRIXA16 scale;
	D3DXMATRIXA16 rotation;

	BoundingSphere forwardBoundingSphere = BoundingSphere();

	CharacterState currentState = CHARACTER_IDLE;

	//스킬
	D3DXVECTOR3 skill1Pos = position;
	BoundingSphere skill1Sphere = BoundingSphere();
	static const int maxSkill1Radius = 20;
	static const float skillCastingTime;
	
	//update의 파트를 나눔
	
	void ProcessState(CharacterState state);


	//충돌처리용 벡터
	std::vector<SkinnedCharacter*> monsters;
	std::vector<Tree*> trees;

};

