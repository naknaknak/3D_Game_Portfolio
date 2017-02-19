#pragma once
#include "SkinnedCharacter.h"
#include "Tree.h"
#include "Minion.h"
#include "Boss.h"
/*
//플레이어만 가능한 것이 분명 있을 터
//일단 지금은 조작, 행동을 나누기 위해 만들어둠.
//플레이어와 보스가 스킬까지 넣는 게 목표.
*/

//회피방향으로 애니메이션을 나누기 위해 사용
enum DodgeDirection
{
	DODGE_FORWARD = 0,
	DODGE_BACKWARD,
	DODGE_LEFT,
	DODGE_RIGHT

};

class Player :
	public SkinnedCharacter
{
public:
	Player();
	virtual ~Player();
	
	virtual void Initialize(char* path, char* filename) override;

	virtual void Destroy( ) override;
	
	virtual void Update() override;
	virtual void Render( );
	void Debugging( );
	

	virtual void ChangeCharacterState(CharacterState state) override;
	
	inline void SetTrees(std::vector<Tree*> inputTrees) { trees = inputTrees; }
	
	inline void SetMonsters(std::vector<Monster*> inputMonsters) { monsters = inputMonsters; }
	inline float* GetPlayerHP() { return &hp; }
	
protected:
	virtual void Hit(float damage);

	bool CollideTrees();
	bool CollideRocks();
	bool CollideMonsters();
	bool HitMonsters(BoundingSphere& sphere,int damage);

	D3DXVECTOR3 rotateAxis = D3DXVECTOR3(0, 1, 0);
	float rotationAngle = 0.0f;
	float moveSpeed = 20.0f;
	D3DXMATRIXA16 translate;
	D3DXMATRIXA16 scale;
	D3DXMATRIXA16 rotation;

	BoundingSphere forwardBoundingSphere = BoundingSphere();

	//이동
	CharacterState currentState = CHARACTER_IDLE;
	DodgeDirection currentDodgeDirection = DODGE_BACKWARD;
	float dodgeSpeed = 30.0f;
	static const int DODGE_ROTATION_FACTOR = 1;
	float jumpSpeed = 30.0f;
	static const float GRAVITY_ACCEL;
	static const int jumpConstant = 8;
	static const int SPRINT_MULTYPLICATION = 2;
	//스킬
	D3DXVECTOR3 skill1Pos = position;
	BoundingSphere skill1Sphere = BoundingSphere();
	BoundingSphere attackSphere = BoundingSphere();
	static const int maxSkill1Radius = 20;
	static const float skillCastingTime;
	static const int ATTACK_DAMAGE = 20;
	static const int SKILL1_DAMAGE = 40;
	float hp = 100.0f;
	float indamage = 0.0f;
	static const int ATTACK_RANGE = 8;
	//update의 파트를 나눔
	
	void ProcessState(CharacterState state);
	

	//충돌처리용 다른 오브젝트들의 배열
	std::vector<Monster*> monsters;
	std::vector<Tree*> trees;

};

