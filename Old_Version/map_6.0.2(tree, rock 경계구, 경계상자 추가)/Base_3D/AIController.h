#pragma once
/*
AI컨트롤러들은 같은 종류 MESH전체가 공유하는 클래스들이다.
역할은 Position을 바꾸고 Rotation을 바꿔주는 것이다. Mesh의 Update안에 들어가서
Update(){
UpdatePosition();
UpdateRotation();
ProcessState();
}
같은 식으로 모든 적들의 Update를 통일하기 위한 클래스이다.
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
	//dijkstra dijk; //길 계산용. 패트롤시에 노드로도 쓰인다.
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