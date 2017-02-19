#pragma once
#include "SkinnedCharacter.h"
/*몬스터 부모 클래스
//모든 몬스터가 이 클래스의 자식이 된다.
//배열에 넣고 한번에 Update하기 위해 필요하다.
//Update순서에 따라 꼬일 수도 있는데 지금 상태에선 서로를 공격하지는 않으므로 이동방향에 
//문제가 생기는 정도이다.
*/

class Monster :
	public SkinnedCharacter
{
public:
	Monster();
	virtual ~Monster();
	virtual void Update()=0;//update는 아무것도 안한다
	inline void SetPlayer(SkinnedCharacter* inputPlayer) { player = inputPlayer; }//타겟을 플레이어로 정해줌
	inline void SetInitialPosition(D3DXVECTOR3 inputInitialPosition) { initialPositon = inputInitialPosition; }//최초위치를 지정해줌
	inline void SetMonsters(std::vector<Monster*> inputMonsters) { monsters = inputMonsters; }//몬스터 배열에 몬스터를 넣는 함수
	virtual float GetMaxHP() = 0;//최대HP를 얻는다
	virtual float GetHP( ) = 0;//현재HP를 얻는다

protected:
	virtual void ProcessState()=0;//State에 계속 있을 때 상태체크, 실행을 위한 함수
	virtual void SetPosition(D3DXVECTOR3 newPostion);//초기화때 바운딩스피어, 바운딩박스 중심을 잡아주는 함수
	virtual void Hit(float damage) override;//맞을때 자기HP를 줄이기 위해 실행됨

	float inDamage = 0.0f; //데미지 들어올 때 쓸 내부변서
	D3DXVECTOR3 rotateAxis = D3DXVECTOR3(0, 1, 0);//회전축은 Y
	float rotationAngle = 0.0f;//회전각도
	float moveSpeed = 20.0f;//이동속도

	D3DXMATRIXA16 translate; //T
	D3DXMATRIXA16 scale;	//S
	D3DXMATRIXA16 rotation;	//R 셋이합쳐 Transform
	D3DXVECTOR3 initialPositon;//초기위치

	BoundingSphere forwardBoundingSphere = BoundingSphere(); //벽충돌, 적이나 같은 몬스터끼리 충돌할 때 쓰기 위해 한 틱 앞의 판정을 유지한다
	
	BoundingSphere sight_wide= BoundingSphere(); //시야 범위. 여기에 들어온 걸 먼저 검사한 뒤에 내부 시야판정을 다시 한다.
	BoundingSphere sight_narrow= BoundingSphere(); // 소리판정이라고 생각해서 만든 시야. 여기 들어오면 무조건 쫓아감
	BoundingSphere areaBoundingSphere= BoundingSphere(); //여기서 나가면 그만쫓아오게 하려고 만듬. 현재는 안쓴다. 
	SkinnedCharacter* player = nullptr; // 플레이어를 알기 위해서 받음
	D3DXVECTOR3* target=nullptr; //쫓아갈때 타겟
	std::vector<Monster*> monsters; //다른 몬스터들이 다 들어가는 벡터
};

