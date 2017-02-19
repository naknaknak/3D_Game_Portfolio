#pragma once
#include "Monster.h"
/*몬스터 중 보스 클래스
//스킬 사용도 염두에 두고 클래스를 나눴다.
//보스가 아닌 몬스터는 스킬을 쓰지 않는다.
*/

class Boss :
	public Monster
{
public:
	Boss(); //디폴트 생성자
	virtual ~Boss();
	virtual void Update() override; //update.
	virtual void Initialize(char* path, char* filename);//초기화
	virtual float GetMaxHP()override { return 250.0f; }//UI에 쓰기 위해 HP를 얻는 함수. 제대로 HP가 안 나오는 버그 때문에 현재는 하드코딩되어 있음.
	virtual float GetHP( )override { return hp; } //현재 HP를 얻는다. UI에서 사용.
	virtual void ChangeCharacterState(CharacterState state); //상태, 애니메이션을 제어하는 함수.

protected:
	void ProcessState(); //상태머신 Stay할 때 실행하는 함수. 이 함수에서 State를 바꾼다.
	virtual void Hit(float damage); // 맞았을 때 실행되는 함수
	bool CollisionMonsters(D3DXVECTOR3* direction); // 다른 몬스터와 충돌했나 여부를 검사한다.

	BoundingSphere attackSphere; //공격판정용 구. 공격시에 활성화된다.
	float hp = 250.0f;
	float rotationAngle = D3DX_PI; //원래 보고있는 방향이 플레이어 길 반대쪽이다. 방향을 좀 틀어줌
	static const float ATTACK_DAMAGE; //공격력
	static const float ATTACK_RANGE; //공격범위
	static const float BOSS_SIGHT_RANGE; //시야범위. 이 안에 들어오면 반응한다.
};

