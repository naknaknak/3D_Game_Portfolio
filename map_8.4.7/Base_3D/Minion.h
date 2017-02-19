#pragma once
#include "Monster.h"
/*�̴Ͼ� Ŭ����
//���� �þ߷� �þ� ��, Ȥ�� ���� Ž������ ���� �� �߽߰� �ٰ����� �����ϴ� ����.
//��ų����� ���Ѵ�.
*/

class Minion :
	public Monster
{
public:
	Minion();
	virtual ~Minion();
	virtual void Update() override;//update
	virtual void Initialize(char* path, char* filename) override;//�ʱ�ȭ
	
	virtual void ChangeCharacterState(CharacterState state);//ĳ���� ���� �ٲٱ�
	virtual float GetMaxHP( )override { return 100.0f; } //UI�� �� MAX HP. ����� ���׶����� �ϵ��ڵ��Ǿ�����.
	virtual float GetHP()override { return hp; } // ���� HP�� �����ش�

protected:
	void ProcessState() override;//���¸ӽ� Stay. ���� ������ �����ϴ� �� polling�� ������.
	void SelectPatrolPosition();//���¸ӽſ� Patrol������ ���� �Լ�. ���� Patrol���� �󸶳� ��� ����
	bool CollisionMonsters(D3DXVECTOR3* direction); //�ٸ� ���Ϳ� �浹�����ϴ� �Լ�.
	virtual void Hit(float damage);//�¾��� �� ����ȴ�. �ڱ� HP�� ���̱� ���� �ʿ�

	BoundingSphere attackSphere = BoundingSphere();//���������� Sphere

	static const float ATTACK_DAMAGE;//���ݷ�
	float hp = 100.0f;//HP
	D3DXVECTOR3 patrolPosition;//Patrol�ϴ� ��ġ
	bool rotateCW = true;//Patrol ����
	float idleTime = 0.0f;//���� ��Ÿ�ӿ� ����
	float roationAngle = 0.0f;//ȸ������
	float inDamage = 0.0f;//������ ������
};


