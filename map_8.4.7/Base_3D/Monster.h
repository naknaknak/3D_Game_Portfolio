#pragma once
#include "SkinnedCharacter.h"
/*���� �θ� Ŭ����
//��� ���Ͱ� �� Ŭ������ �ڽ��� �ȴ�.
//�迭�� �ְ� �ѹ��� Update�ϱ� ���� �ʿ��ϴ�.
//Update������ ���� ���� ���� �ִµ� ���� ���¿��� ���θ� ���������� �����Ƿ� �̵����⿡ 
//������ ����� �����̴�.
*/

class Monster :
	public SkinnedCharacter
{
public:
	Monster();
	virtual ~Monster();
	virtual void Update()=0;//update�� �ƹ��͵� ���Ѵ�
	inline void SetPlayer(SkinnedCharacter* inputPlayer) { player = inputPlayer; }//Ÿ���� �÷��̾�� ������
	inline void SetInitialPosition(D3DXVECTOR3 inputInitialPosition) { initialPositon = inputInitialPosition; }//������ġ�� ��������
	inline void SetMonsters(std::vector<Monster*> inputMonsters) { monsters = inputMonsters; }//���� �迭�� ���͸� �ִ� �Լ�
	virtual float GetMaxHP() = 0;//�ִ�HP�� ��´�
	virtual float GetHP( ) = 0;//����HP�� ��´�

protected:
	virtual void ProcessState()=0;//State�� ��� ���� �� ����üũ, ������ ���� �Լ�
	virtual void SetPosition(D3DXVECTOR3 newPostion);//�ʱ�ȭ�� �ٿ�����Ǿ�, �ٿ���ڽ� �߽��� ����ִ� �Լ�
	virtual void Hit(float damage) override;//������ �ڱ�HP�� ���̱� ���� �����

	float inDamage = 0.0f; //������ ���� �� �� ���κ���
	D3DXVECTOR3 rotateAxis = D3DXVECTOR3(0, 1, 0);//ȸ������ Y
	float rotationAngle = 0.0f;//ȸ������
	float moveSpeed = 20.0f;//�̵��ӵ�

	D3DXMATRIXA16 translate; //T
	D3DXMATRIXA16 scale;	//S
	D3DXMATRIXA16 rotation;	//R �������� Transform
	D3DXVECTOR3 initialPositon;//�ʱ���ġ

	BoundingSphere forwardBoundingSphere = BoundingSphere(); //���浹, ���̳� ���� ���ͳ��� �浹�� �� ���� ���� �� ƽ ���� ������ �����Ѵ�
	
	BoundingSphere sight_wide= BoundingSphere(); //�þ� ����. ���⿡ ���� �� ���� �˻��� �ڿ� ���� �þ������� �ٽ� �Ѵ�.
	BoundingSphere sight_narrow= BoundingSphere(); // �Ҹ������̶�� �����ؼ� ���� �þ�. ���� ������ ������ �Ѿư�
	BoundingSphere areaBoundingSphere= BoundingSphere(); //���⼭ ������ �׸��Ѿƿ��� �Ϸ��� ����. ����� �Ⱦ���. 
	SkinnedCharacter* player = nullptr; // �÷��̾ �˱� ���ؼ� ����
	D3DXVECTOR3* target=nullptr; //�Ѿư��� Ÿ��
	std::vector<Monster*> monsters; //�ٸ� ���͵��� �� ���� ����
};

