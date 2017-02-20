#pragma once
#include "SkinnedCharacter.h"
#include "Tree.h"

class Monster; //몬스터 클래스 사용

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

//플레이어 클래스
class Player :
	public SkinnedCharacter
{
public:
	Player();
	virtual ~Player();
	
	virtual void Initialize(char* path, char* filename) override;//초기화. 메쉬를 로드한다

	virtual void Destroy( ) override;//제거할때 호출하는 함수
	
	virtual void Update() override;//업데이트 함수
	virtual void Render( );//렌더 함수
	void Debugging( );//디버그용, 현재는 화면에 위치와 애니메이션을 써줌.
	

	virtual void ChangeCharacterState(CharacterState state) override;//State바꾸는 함수
	
	inline void SetTrees(std::vector<Tree*> inputTrees) { trees = inputTrees; }//충돌검사를 위한 나무 포인터 set
	
	inline void SetMonsters(std::vector<Monster*> inputMonsters) { monsters = inputMonsters; }//충돌검사, 공격판정을 검사하기 위한 몬스터 포인터 set
	inline float* GetPlayerHP() { return &hp; }//UI에 사용할 get
	
protected:
	virtual void Hit(float damage);//피격시 호출할 함수

	bool CollideTrees(); //나무와 충돌검사
	bool CollideRocks(); //돌과 충돌검사
	bool CollideMonsters(); //몬스터와 충돌검사
	bool HitMonsters(BoundingSphere& sphere,int damage); //공격판정, 데미지 추가하는 함수

	//회전
	D3DXVECTOR3 rotateAxis = D3DXVECTOR3(0, 1, 0); //회전축은 Y
	float rotationAngle = 0.0f; // 현재 회전각을 저장할 변수
	float moveSpeed = 20.0f; //이동속도
	D3DXMATRIXA16 translate;//SRT
	D3DXMATRIXA16 scale;	//
	D3DXMATRIXA16 rotation;	//

	BoundingSphere forwardBoundingSphere = BoundingSphere(); //한 틱 앞을 검사

	//FSM에서 쓰는 변수들
	CharacterState currentState = CHARACTER_IDLE;	//현재 State
	DodgeDirection currentDodgeDirection = DODGE_BACKWARD;	//회피방향. 초기값은 뒤로
	float dodgeSpeed = 30.0f;								//회피속도
	static const int DODGE_ROTATION_FACTOR = 1;				//옆으로 회피할때 얼마나 회전하면서 앞으로 갈 것인가
	float jumpSpeed = 30.0f;								//점프시 초기속도. (v-gt)로 속도를 계산
	static const float GRAVITY_ACCEL;						//중력가속도. float은 헤더에서 초기화가 안됨.
	static const int jumpConstant = 8;						//점프 상수. 떨어지는 속도가 좀 느려서 추가.
	static const int SPRINT_MULTYPLICATION = 2;				//달릴때 속도배율
	
	//스킬, 공격
	D3DXVECTOR3 skill1Pos = position;						//판정초기값은 자기중심
	BoundingSphere skill1Sphere = BoundingSphere();			//스킬 판정용 구
	BoundingSphere attackSphere = BoundingSphere();			//공격판정용 구
	static const int maxSkill1Radius = 20;					//판정 최대범위
	static const float skillCastingTime;					//스킬 사용시간
	static const int ATTACK_DAMAGE = 20;					//기본 공격력
	static const int SKILL1_DAMAGE = 40;					//스킬1 공격력
	float hp = 100.0f;										//HP
	float indamage = 0.0f;									//맞을때를 위한 변수
	static const int ATTACK_RANGE = 8;						//공격범위

																
	void ProcessState(CharacterState state);				//FSM에서 Stay, Transition을 담당하는 함수
	

	//충돌처리용 다른 오브젝트들의 배열
	std::vector<Monster*> monsters;							//몬스터
	std::vector<Tree*> trees;								//나무

};

