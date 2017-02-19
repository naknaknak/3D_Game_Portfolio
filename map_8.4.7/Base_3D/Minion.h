#pragma once
#include "Monster.h"
/*미니언 클래스
//좁은 시야로 시야 내, 혹은 좁은 탐지범위 내에 적 발견시 다가가서 공격하는 몬스터.
//스킬사용을 안한다.
*/

class Minion :
	public Monster
{
public:
	Minion();
	virtual ~Minion();
	virtual void Update() override;//update
	virtual void Initialize(char* path, char* filename) override;//초기화
	
	virtual void ChangeCharacterState(CharacterState state);//캐릭터 상태 바꾸기
	virtual float GetMaxHP( )override { return 100.0f; } //UI에 줄 MAX HP. 현재는 버그때문에 하드코딩되어있음.
	virtual float GetHP()override { return hp; } // 현재 HP를 돌려준다

protected:
	void ProcessState() override;//상태머신 Stay. 적이 오는지 감지하는 건 polling에 가깝다.
	void SelectPatrolPosition();//상태머신에 Patrol용으로 만든 함수. 다음 Patrol까지 얼마나 갈까를 정함
	bool CollisionMonsters(D3DXVECTOR3* direction); //다른 몬스터와 충돌판정하는 함수.
	virtual void Hit(float damage);//맞았을 때 실행된다. 자기 HP를 줄이기 위해 필요

	BoundingSphere attackSphere = BoundingSphere();//공격판정용 Sphere

	static const float ATTACK_DAMAGE;//공격력
	float hp = 100.0f;//HP
	D3DXVECTOR3 patrolPosition;//Patrol하는 위치
	bool rotateCW = true;//Patrol 방향
	float idleTime = 0.0f;//공격 쿨타임용 변수
	float roationAngle = 0.0f;//회전각도
	float inDamage = 0.0f;//들어오는 데미지
};


