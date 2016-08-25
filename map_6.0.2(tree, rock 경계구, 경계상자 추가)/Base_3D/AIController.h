#pragma once
/*
AI��Ʈ�ѷ����� ���� ���� MESH��ü�� �����ϴ� Ŭ�������̴�.
������ Position�� �ٲٰ� Rotation�� �ٲ��ִ� ���̴�. Mesh�� Update�ȿ� ����
Update(){
UpdatePosition();
UpdateRotation();
ProcessState();
}
���� ������ ��� ������ Update�� �����ϱ� ���� Ŭ�����̴�.
*/
static class AIController
{
public:
	AIController();
	~AIController();
	void InitializeCooltime() {};
	void UpdatePositon(CharacterState state, OUT D3DXVECTOR3* position) {};
	void UpdateRotation(CharacterState state, OUT float* rotationAngleY) {};
	void ProcessState(IN OUT CharacterState* state) {};
protected:
	//dijkstra dijk; //�� ����. ��Ʈ�ѽÿ� ���ε� ���δ�.
};

static class GoatController: public AIController
{
public:
	GoatController();
	virtual ~GoatController();

	static void InitializeCooltime() ;
	static void UpdatePositon(CharacterState state,OUT D3DXVECTOR3* position) ;
	static void UpdateRotation(CharacterState state,OUT float* rotationAngleY );
	static void ProcessState(IN OUT CharacterState* state) ;
};

static class BossController : public AIController
{
public:
	BossController();
	virtual ~BossController();

	static void InitializeCooltime() ;
	static void UpdatePositon(CharacterState state,OUT D3DXVECTOR3* position);
	static void UpdateRotation(CharacterState state,OUT float* rotationAngleY);
	static void ProcessState(IN OUT CharacterState* state) ;
};