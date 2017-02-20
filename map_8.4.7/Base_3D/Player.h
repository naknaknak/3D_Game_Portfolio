#pragma once
#include "SkinnedCharacter.h"
#include "Tree.h"

class Monster; //���� Ŭ���� ���

/*
//�÷��̾ ������ ���� �и� ���� ��
//�ϴ� ������ ����, �ൿ�� ������ ���� ������.
//�÷��̾�� ������ ��ų���� �ִ� �� ��ǥ.
*/

//ȸ�ǹ������� �ִϸ��̼��� ������ ���� ���
enum DodgeDirection
{
	DODGE_FORWARD = 0,
	DODGE_BACKWARD,
	DODGE_LEFT,
	DODGE_RIGHT

};

//�÷��̾� Ŭ����
class Player :
	public SkinnedCharacter
{
public:
	Player();
	virtual ~Player();
	
	virtual void Initialize(char* path, char* filename) override;//�ʱ�ȭ. �޽��� �ε��Ѵ�

	virtual void Destroy( ) override;//�����Ҷ� ȣ���ϴ� �Լ�
	
	virtual void Update() override;//������Ʈ �Լ�
	virtual void Render( );//���� �Լ�
	void Debugging( );//����׿�, ����� ȭ�鿡 ��ġ�� �ִϸ��̼��� ����.
	

	virtual void ChangeCharacterState(CharacterState state) override;//State�ٲٴ� �Լ�
	
	inline void SetTrees(std::vector<Tree*> inputTrees) { trees = inputTrees; }//�浹�˻縦 ���� ���� ������ set
	
	inline void SetMonsters(std::vector<Monster*> inputMonsters) { monsters = inputMonsters; }//�浹�˻�, ���������� �˻��ϱ� ���� ���� ������ set
	inline float* GetPlayerHP() { return &hp; }//UI�� ����� get
	
protected:
	virtual void Hit(float damage);//�ǰݽ� ȣ���� �Լ�

	bool CollideTrees(); //������ �浹�˻�
	bool CollideRocks(); //���� �浹�˻�
	bool CollideMonsters(); //���Ϳ� �浹�˻�
	bool HitMonsters(BoundingSphere& sphere,int damage); //��������, ������ �߰��ϴ� �Լ�

	//ȸ��
	D3DXVECTOR3 rotateAxis = D3DXVECTOR3(0, 1, 0); //ȸ������ Y
	float rotationAngle = 0.0f; // ���� ȸ������ ������ ����
	float moveSpeed = 20.0f; //�̵��ӵ�
	D3DXMATRIXA16 translate;//SRT
	D3DXMATRIXA16 scale;	//
	D3DXMATRIXA16 rotation;	//

	BoundingSphere forwardBoundingSphere = BoundingSphere(); //�� ƽ ���� �˻�

	//FSM���� ���� ������
	CharacterState currentState = CHARACTER_IDLE;	//���� State
	DodgeDirection currentDodgeDirection = DODGE_BACKWARD;	//ȸ�ǹ���. �ʱⰪ�� �ڷ�
	float dodgeSpeed = 30.0f;								//ȸ�Ǽӵ�
	static const int DODGE_ROTATION_FACTOR = 1;				//������ ȸ���Ҷ� �󸶳� ȸ���ϸ鼭 ������ �� ���ΰ�
	float jumpSpeed = 30.0f;								//������ �ʱ�ӵ�. (v-gt)�� �ӵ��� ���
	static const float GRAVITY_ACCEL;						//�߷°��ӵ�. float�� ������� �ʱ�ȭ�� �ȵ�.
	static const int jumpConstant = 8;						//���� ���. �������� �ӵ��� �� ������ �߰�.
	static const int SPRINT_MULTYPLICATION = 2;				//�޸��� �ӵ�����
	
	//��ų, ����
	D3DXVECTOR3 skill1Pos = position;						//�����ʱⰪ�� �ڱ��߽�
	BoundingSphere skill1Sphere = BoundingSphere();			//��ų ������ ��
	BoundingSphere attackSphere = BoundingSphere();			//���������� ��
	static const int maxSkill1Radius = 20;					//���� �ִ����
	static const float skillCastingTime;					//��ų ���ð�
	static const int ATTACK_DAMAGE = 20;					//�⺻ ���ݷ�
	static const int SKILL1_DAMAGE = 40;					//��ų1 ���ݷ�
	float hp = 100.0f;										//HP
	float indamage = 0.0f;									//�������� ���� ����
	static const int ATTACK_RANGE = 8;						//���ݹ���

																
	void ProcessState(CharacterState state);				//FSM���� Stay, Transition�� ����ϴ� �Լ�
	

	//�浹ó���� �ٸ� ������Ʈ���� �迭
	std::vector<Monster*> monsters;							//����
	std::vector<Tree*> trees;								//����

};

