#pragma once
#include "Monster.h"
/*���� �� ���� Ŭ����
//��ų ��뵵 ���ο� �ΰ� Ŭ������ ������.
//������ �ƴ� ���ʹ� ��ų�� ���� �ʴ´�.
*/

class Boss :
	public Monster
{
public:
	Boss(); //����Ʈ ������
	virtual ~Boss();
	virtual void Update() override; //update.
	virtual void Initialize(char* path, char* filename);//�ʱ�ȭ
	virtual float GetMaxHP()override { return 250.0f; }//UI�� ���� ���� HP�� ��� �Լ�. ����� HP�� �� ������ ���� ������ ����� �ϵ��ڵ��Ǿ� ����.
	virtual float GetHP( )override { return hp; } //���� HP�� ��´�. UI���� ���.
	virtual void ChangeCharacterState(CharacterState state); //����, �ִϸ��̼��� �����ϴ� �Լ�.

protected:
	void ProcessState(); //���¸ӽ� Stay�� �� �����ϴ� �Լ�. �� �Լ����� State�� �ٲ۴�.
	virtual void Hit(float damage); // �¾��� �� ����Ǵ� �Լ�
	bool CollisionMonsters(D3DXVECTOR3* direction); // �ٸ� ���Ϳ� �浹�߳� ���θ� �˻��Ѵ�.

	BoundingSphere attackSphere; //���������� ��. ���ݽÿ� Ȱ��ȭ�ȴ�.
	float hp = 250.0f;
	float rotationAngle = D3DX_PI; //���� �����ִ� ������ �÷��̾� �� �ݴ����̴�. ������ �� Ʋ����
	static const float ATTACK_DAMAGE; //���ݷ�
	static const float ATTACK_RANGE; //���ݹ���
	static const float BOSS_SIGHT_RANGE; //�þ߹���. �� �ȿ� ������ �����Ѵ�.
};

