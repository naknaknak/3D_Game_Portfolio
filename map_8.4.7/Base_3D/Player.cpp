#include "stdafx.h"
#include "Player.h"

/* 플레이어 클래스
// 몬스터와 다른 부분은 스킬, 조작계 정도이다.
*/

//상수 중 int 아닌 것들
const float Player::skillCastingTime = 2.0f;
const float Player::GRAVITY_ACCEL = 9.8f;


Player::Player()
{
}


Player::~Player()
{
}

void Player::Destroy( )
{
	SkinnedMesh::Destroy( );
	//if (hm)
	//{
	//	hm->Destroy( );
	//	delete hm;
	//	hm = nullptr;
	//}
}
//초기화 함수
//파일 로드
//애니메이션 이름, 번호를 넣는 애니메이션 초기화
//HP, 판정(바운딩 볼륨) 초기화를 수행
void Player::Initialize(char* path, char* filename)
{
	SkinnedMesh::Initialize( path,  filename);

	InitializeAnimation();
	forwardBoundingSphere.center = boundingSphere.center;
	forwardBoundingSphere.radius = boundingSphere.radius;
	
	hp = 100;

}

//Update함수
//다른 캐릭터와 거의 동일하다. State를 Update하고 현재 state에서 변한 SRT를 반영시킴
void Player::Update()
{
	
	rotationAngle = GameManager::GetCamera()->GetRotateY();

	D3DXMatrixRotationY(&rotation, rotationAngle);
	
	//rotation행렬을 ProcessState에서 쓰기떄문에 미리 rotation만 계산해준다.
	//postion이 processState에서 바뀌기 때문에 Translation을 나중에 한다.
	ProcessState(currentState);
	
	D3DXMatrixTranslation(&translate, position.x, position.y, position.z);
	D3DXMatrixScaling(&scale, scaleFactor, scaleFactor, scaleFactor);

	world = scale*rotation*translate;
	
	
}

//Render함수. 상위클래스에서 디버그하는 부분만 추가되어 있다.
void Player::Render( )
{
	SkinnedMesh::Render( );
	Debugging( );
}
//화면에 현재 위치, 애니메이션 이름을 띄워주는 함수
void Player::Debugging( )
{
	char str[256];
	RECT rc = { 10, 70, 10, 10 };
	sprintf_s(str, "Player_position: x: %.3f / y: %.3f / z: %.3f", position.x, position.y, position.z);
	UI_Manager::GetFont( )->DrawTextA(nullptr, str, strlen(str), &rc, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));

	RECT rc1 = { 15, 50, 16, 51 };
	sprintf_s(str, "AnimName: %s, AnimPeriod: %.3f", animName, animPeriod);
	UI_Manager::GetFont( )->DrawTextA(nullptr, str, strlen(str), &rc1, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));


}
//애니메이션을 바꿔주는 함수
//state변수와 애니메이션이 같이 움직이기 때문에 state변수도 여기서 바꿔준다.
//FSM을 클래스로 래핑시키면서 같이 바뀔 예정
void Player::ChangeCharacterState(CharacterState state)
{
	currentState = state;
	SetAnimationName(animationNames[state].c_str(), &selectedAnimationLength);
	animController->SetTrackPosition(0, 0.0f);
	animController->SetTrackSpeed(0, 1.5f);
	selectedAnimationLength *= 0.67f;
	currentAnimationTime = 0.0f;
}


//못움직이게 하는 부분을 좀 고쳐야할듯.
//현재 State에서 실행할 일과 State transition이 들어 있다.
void Player::ProcessState(CharacterState state)
{
	//움직이기 위해 필요한 변수들
	D3DXVECTOR3 pos = position;
	
	D3DXVECTOR3 direction;
	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);
	D3DXVECTOR3 forward = D3DXVECTOR3(0, 0, 1);
	D3DXVECTOR3 right;

	//바라보는 방향, 오른쪽을 미리 만들어놓는다
	direction = D3DXVECTOR3(rotation._31, rotation._32, rotation._33);
	right = D3DXVECTOR3(-rotation._11, -rotation._12,- rotation._13);
	
	isDead = false;
	//State transition, state stay로직이 들어 있는 부분
	switch (state)
	{
		//Idle, Move만 특별한 State. changeState로 바꾸지 않는다.
	case CharacterState::CHARACTER_IDLE:
	case CharacterState::CHARACTER_MOVE:
	{
		//바닥충돌검사부터 한다
		if (hm->GetHeight(pos, pos.x, pos.z) != false)
		{
			//필드 안에 있다면
			float tick = (float)GameManager::GetTick();
			bool move = false;
			//키 입력을 받아 움직인다. 이 때 한 tick 앞의 자신과 사물을 Collisition detection한다.
			if ((GetAsyncKeyState('W') & 0x8000) != 0)
			{
				//이동, 다음 틱에 대해서 충돌검사
				pos -= (-direction * moveSpeed * tick);
				forwardBoundingSphere.center = pos;
				
				//충돌검사. 나무만 검사. 돌은 나중에 하려고 남겨둠.
				//충돌안하면 state를 MOVE로 바꿔주면서 앞으로, 충돌하면 제자리에 멈춰있는다.
				bool collisionTrees = CollideTrees();
				bool collisionRocks = false;
				
				if (collisionTrees || collisionRocks)
				{
					//돌아가자
					pos = position;
					break;
				}
				else
				{
					//충돌안하면 움직인다.
					if (currentState == CharacterState::CHARACTER_IDLE)
					{
						ChangeCharacterState(CharacterState::CHARACTER_MOVE);
					}
					move = true;
				}
			}//w key
			else if ((GetAsyncKeyState('S') & 0x8000) != 0)
			{
				//비슷하지만 예전 충돌코드. 나무랑만 충돌부분이 있으므로 바꿔야 함.
				//다른 전부에서 비슷한 느낌
				pos += (-direction * moveSpeed * tick);
				forwardBoundingSphere.center = pos;
				bool collision = false;
				for (auto iter = trees.begin(); iter != trees.end(); ++iter)
				{
					collision = Collision::IsSphereToSphere(forwardBoundingSphere, (*iter)->GetBoundingSphere());
					if (collision)
					{
						pos = position;
						break;
					}

				}
				if (!collision) {
					if (currentState == CharacterState::CHARACTER_IDLE)
					{
						ChangeCharacterState(CharacterState::CHARACTER_MOVE);
					}
					move = true;
				}
			}//s key
			if ((GetAsyncKeyState('A') & 0x8000) != 0)
			{
				//s와 같음
				pos -= (-right * moveSpeed * tick);
				forwardBoundingSphere.center = pos;
				bool collision = false;
				for (auto iter = trees.begin(); iter != trees.end(); ++iter)
				{
					collision = Collision::IsSphereToSphere(forwardBoundingSphere, (*iter)->GetBoundingSphere());
					if (collision)
					{
						pos = position;
						break;
					}

				}
				if (!collision) {
					if (currentState == CharacterState::CHARACTER_IDLE)
					{
						ChangeCharacterState(CharacterState::CHARACTER_MOVE);
					}
					move = true;
				}
			}//a key
			else if ((GetAsyncKeyState('D') & 0x8000) != 0)
			{
				//s와 같음
				pos += (-right * moveSpeed * tick);
				forwardBoundingSphere.center = pos;
				bool collision = false;
				for (auto iter = trees.begin(); iter != trees.end(); ++iter)
				{
					collision = Collision::IsSphereToSphere(forwardBoundingSphere, (*iter)->GetBoundingSphere());
					if (collision)
					{
						pos = position;
						break;
					}

				}
				if (!collision) {
					if (currentState == CharacterState::CHARACTER_IDLE)
					{
						ChangeCharacterState(CharacterState::CHARACTER_MOVE);
					}
					move = true;
				}

			}//d, wasd 끝

			//move변수, 전 틱에서 움직였나 보고 IDLE로 애니메이션을 바꿀지 판정
			if ((!move) && currentState == CharacterState::CHARACTER_MOVE)
			{
				ChangeCharacterState(CharacterState::CHARACTER_IDLE);
			}


			//다른 state들로 바꾸는 부분
			if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0)//공격, 마우스 왼쪽
			{
				ChangeCharacterState(CharacterState::CHARACTER_ATTACK);
			}
			else if ((GetAsyncKeyState('1') & 0x8000) != 0)//1스킬 1
			{
				ChangeCharacterState(CharacterState::CHARACTER_SKILL1);
			}
			else if ((GetAsyncKeyState(VK_LSHIFT) & 0x8000) != 0)//달리기 왼쪽 시프트
			{
				moveSpeed *= SPRINT_MULTYPLICATION;//속도만 곱해주고 애니메이션을 바꾼다
				ChangeCharacterState(CharacterState::CHARACTER_SPRINT);
			}
			if ((GetAsyncKeyState('C') & 0x0001) != 0)//회피 C
			{
				//이동방향으로 애니메이션을 다른걸로 바꾼다. 4방향으로 회피가능.
				if ((GetAsyncKeyState('W') & 0x8000) != 0)
				{
					ChangeCharacterState(CharacterState::CHARACTER_DODGE_F);
				}
				else	if ((GetAsyncKeyState('A') & 0x8000) != 0)
				{
					ChangeCharacterState(CharacterState::CHARACTER_DODGE_L);
				}
				else	if ((GetAsyncKeyState('S') & 0x8000) != 0)
				{
					ChangeCharacterState(CharacterState::CHARACTER_DODGE_B);
				}
				else	if ((GetAsyncKeyState('D') & 0x8000) != 0)
				{
					ChangeCharacterState(CharacterState::CHARACTER_DODGE_R);
				}
				else ChangeCharacterState(CharacterState::CHARACTER_DODGE_B);

			}
			if (isHit)//피격
			{
				if (hp > 0)//살아있다
				{
					hp -= indamage;//HP를 빼줌
					ChangeCharacterState(CharacterState::CHARACTER_HIT); //사망판정을 여기서 다시한다.
				}
				else
				{
					ChangeCharacterState(CharacterState::CHARACTER_DEAD); //죽음
				}
			
			}
			if (GetAsyncKeyState('X') & 0x8000 != 0)//점프키 X
			{
				ChangeCharacterState(CharacterState::CHARACTER_JUMP);
			}
		}
	}
	break;//idlemove 끝
	//공격
	case CharacterState::CHARACTER_ATTACK:
	{
		//애니메이션 시간을 계산
		double tick = GameManager::GetTick();
		currentAnimationTime += tick;
		//공격판정. sphere로 판정을 한다. 
		BoundingSphere attackSphere;
		attackSphere.center = position + meshCenter + forward*0.5f;
		attackSphere.radius = ATTACK_RANGE;
		//애니메이션 끝났나, 안끝났으면 공격판정 충돌처리
		if (currentAnimationTime >= selectedAnimationLength)
		{
			ChangeCharacterState(CharacterState::CHARACTER_IDLE);
		}
		else
		{
			bool hit = HitMonsters(attackSphere,ATTACK_DAMAGE);
			
		}//때리다 맞음
		if (isHit)
		{
			//위와 동일
			if (hp > 0)
			{
				hp -= indamage;
				ChangeCharacterState(CharacterState::CHARACTER_HIT);
			}
			else
			{
				ChangeCharacterState(CharacterState::CHARACTER_DEAD);
			}

		}
		
	}
	break;//attack 끝
	//1스킬. 구모양 범위공격(폭발형)으로 예정해놓음. 판정 타입 몇개를 미리 만들면 쓸 수 있을듯.
	//현재는 sin함수로 구가 시간에 따라 커졌다가 줄어듬.
	case CharacterState::CHARACTER_SKILL1:
	{
		double tick = GameManager::GetTick();
		skill1Pos = position + direction*20.0f;
		skill1Sphere.center = skill1Pos;

		currentAnimationTime += tick;
		skill1Sphere.radius = sinf((float)currentAnimationTime * D3DX_PI) * maxSkill1Radius;
		animController->SetTrackSpeed(0, (float)selectedAnimationLength / skillCastingTime);
		if (currentAnimationTime >= skillCastingTime)
		{
			ChangeCharacterState(CharacterState::CHARACTER_IDLE);
		}
	if (isHit)//스킬쓰다가 맞았다.
		{
			if (hp > 0)
			{
				hp -= indamage;
				ChangeCharacterState(CharacterState::CHARACTER_HIT);
			}
			else
			{
				ChangeCharacterState(CharacterState::CHARACTER_DEAD);
			}

		}
	}
	break;
	//skill1 끝
	//달리기
	case CharacterState::CHARACTER_SPRINT:
	{
		//이동과 기본적으로 비슷하지만 몬스터와 충돌판정을 안한다.
		if (hm->GetHeight(pos, pos.x, pos.z) != false)
		{
			//이동
			float tick = (float)GameManager::GetTick();
			bool move = false;
			if ((GetAsyncKeyState('W') & 0x8000) != 0)
			{

				//뒤로는 안간다
				pos -= (-direction * moveSpeed * tick);
				if ((GetAsyncKeyState('A') & 0x8000) != 0)//대각선 움직임은 단순히 속도를 더해준다.
				{
					pos -= (-right * moveSpeed * tick);
				}
				else if ((GetAsyncKeyState('D') & 0x8000) != 0)
				{
					pos += (-right * moveSpeed * tick);
				}
				
				forwardBoundingSphere.center = pos;
				bool collision = false;
				//충돌하면 IDLE됨
				for (auto iter = trees.begin(); iter != trees.end(); ++iter)
				{
					collision = Collision::IsSphereToSphere(forwardBoundingSphere, (*iter)->GetBoundingSphere());
					if (collision)
					{
						//탈출코드
						pos = position;
						moveSpeed /= SPRINT_MULTYPLICATION ;
						ChangeCharacterState(CharacterState::CHARACTER_IDLE);
						break;
					}

				}

			}//w가 들어와서 앞으로 가는 때
			//앞으로 안 갈 때 state를 탈출한다.
			else
			{
				//탈출코드
				moveSpeed /= SPRINT_MULTYPLICATION ;
				ChangeCharacterState(CharacterState::CHARACTER_IDLE);
			}
		}
		if (isHit)//달리다가 맞으면
		{
			if (hp > 0)
			{
				hp -= indamage;
				moveSpeed /= SPRINT_MULTYPLICATION;
				ChangeCharacterState(CharacterState::CHARACTER_HIT);
			}
			else
			{
				moveSpeed /= SPRINT_MULTYPLICATION;
				ChangeCharacterState(CharacterState::CHARACTER_DEAD);
			}

		}
	}
	break;//sprint 끝
	//피격. 거의 모든 상태에서 이 피격상태로 이동가능
	case CharacterState::CHARACTER_HIT:
	{
		//시간계산용
		float tick = GameManager::GetTick();
		currentAnimationTime += tick;
		//너무 애니메이션이 빨라서 늦춤
		animController->SetTrackSpeed(0, 0.2f);
		//늦춘 길이와 애니메이션 끝나는 시간을 비교
		if (currentAnimationTime >= selectedAnimationLength*5.0f)
		{
			//종료코드. IDLE로 간다
			isHit = false;
			ChangeCharacterState(CharacterState::CHARACTER_IDLE);
		}
		else {
			//애니메이션 실행중에도 회피가능하다. 회피코드를 넣음.
			if ((GetAsyncKeyState('C') & 0x0001) != 0)
			{
				isHit = false;
				if ((GetAsyncKeyState('W') & 0x8000) != 0)
				{
					ChangeCharacterState(CharacterState::CHARACTER_DODGE_F);
				}
				else	if ((GetAsyncKeyState('A') & 0x8000) != 0)
				{
					ChangeCharacterState(CharacterState::CHARACTER_DODGE_L);
				}
				else	if ((GetAsyncKeyState('S') & 0x8000) != 0)
				{
					ChangeCharacterState(CharacterState::CHARACTER_DODGE_B);
				}
				else	if ((GetAsyncKeyState('D') & 0x8000) != 0)
				{
					ChangeCharacterState(CharacterState::CHARACTER_DODGE_R);
				}
				else ChangeCharacterState(CharacterState::CHARACTER_DODGE_B);

			}//계속 맞는 경우를 대비해서 1/2시간까지 무적타임. 지나면 맞는다
			if (currentAnimationTime >= selectedAnimationLength*2.5f)
			{
				if (isHit)
				{
					if (hp > 0)
					{
						hp -= indamage;
						ChangeCharacterState(CharacterState::CHARACTER_HIT);
					}
					else
					{

						ChangeCharacterState(CharacterState::CHARACTER_DEAD);
					}

				}
			}
			else isInvisible = true;//무적타임용 변수. 공격 스피어와 충돌판정 자체를 Disable함
		}

	}
	break;
	//회피 왼쪽
	case CharacterState::CHARACTER_DODGE_L:
	{
		alpha = 0.2f;
		float tick = (float)GameManager::GetTick();
		currentAnimationTime += tick;
		D3DXMATRIXA16 dodgeRot;
		//약간 앞으로 가면서 회피하게 만들었음.
		if (currentAnimationTime >= selectedAnimationLength)
		{
			isInvisible = false;
			alpha = 1.0f;

			ChangeCharacterState(CharacterState::CHARACTER_IDLE);
		}
		else
		{
			//Y축 회전값을 시간에 따라 늘리고, 그 회전 각도를 right, 즉 진행방향에 곱한다. 그리고 그 방향으로 이동한다.
			D3DXMatrixRotationY(&dodgeRot, currentAnimationTime * DODGE_ROTATION_FACTOR);
			D3DXVec3TransformCoord(&right, &right, &dodgeRot);
			pos += (right * dodgeSpeed * tick);
		}
	}
	break;
	//왼쪽과 같음
	case CharacterState::CHARACTER_DODGE_R:
	{
		alpha = 0.2f;

		float tick = (float)GameManager::GetTick();
		currentAnimationTime += tick;
		D3DXMATRIXA16 dodgeRot;

		if (currentAnimationTime >= selectedAnimationLength)
		{
			isInvisible = false;
			alpha = 1.0f;
			ChangeCharacterState(CharacterState::CHARACTER_IDLE);
		}
		else
		{
			D3DXMatrixRotationY(&dodgeRot, -currentAnimationTime * DODGE_ROTATION_FACTOR);
			D3DXVec3TransformCoord(&right, &right, &dodgeRot);
			pos -= (right * dodgeSpeed * tick);
		}
	}
	break;
	//앞으로 회피한다. 
	case CharacterState::CHARACTER_DODGE_F:
	{
		alpha = 0.2f;//반투명하게 할 예정이었으나 값이 안들어감.
		float tick = (float)GameManager::GetTick();
		currentAnimationTime += tick;
		D3DXMATRIXA16 dodgeRot;

		if (currentAnimationTime >= selectedAnimationLength)
		{
			isInvisible = false;
			alpha = 1.0f; //알파값이 안들어감. 수정예정
			ChangeCharacterState(CharacterState::CHARACTER_IDLE);
		}
		else
		{
			pos += direction*dodgeSpeed*tick;
	}
	}
	break;
	//뒤로 회피한다.
	case CharacterState::CHARACTER_DODGE_B:
	{
		alpha = 0.2f;
		float tick = (float)GameManager::GetTick();
		currentAnimationTime += tick;
		D3DXMATRIXA16 dodgeRot;
		

		if (currentAnimationTime >= selectedAnimationLength)
		{
			isInvisible = false;
			ChangeCharacterState(CharacterState::CHARACTER_IDLE);
		}
		else
		{
			pos -= direction*dodgeSpeed*tick;
		}
	}
	break;
	//점프
	case CharacterState::CHARACTER_JUMP:
	{
		float floorY;
		if (hm->GetHeight(floorY, pos.x, pos.z) != false)
		{
			float tick = (float)GameManager::GetTick();
			currentAnimationTime += tick;
			//위치계산은 이전위치 + 속도*tick
			pos.y = position.y+(jumpSpeed - currentAnimationTime*GRAVITY_ACCEL*jumpConstant)*tick;
			if (currentAnimationTime >= selectedAnimationLength)
			{
				//점프 애니메이션보다 높으면 애니메이션이 정지한다
				animController->SetTrackSpeed(0, 0.0f);
			}
			if (pos.y < floorY)
			{
				//바닥보다 낮아지면 착지
				pos.y = floorY;
				ChangeCharacterState(CharacterState::CHARACTER_IDLE);
			}
			else
			{
				//모든방향으로 점프코드를 구현
				if ((GetAsyncKeyState('W') & 0x8000) != 0)
				{

					pos -= (-direction * moveSpeed * tick);
					forwardBoundingSphere.center = pos;
					bool collision = false;
					for (auto iter = trees.begin(); iter != trees.end(); ++iter)
					{
						collision = Collision::IsSphereToSphere(forwardBoundingSphere, (*iter)->GetBoundingSphere());
						if (collision)
						{
							pos = position;
							break;
						}

					}

				}
				else if ((GetAsyncKeyState('S') & 0x8000) != 0)
				{
					pos += (-direction * moveSpeed * tick);
					forwardBoundingSphere.center = pos;
					bool collision = false;
					for (auto iter = trees.begin(); iter != trees.end(); ++iter)
					{
						collision = Collision::IsSphereToSphere(forwardBoundingSphere, (*iter)->GetBoundingSphere());
						if (collision)
						{
							pos = position;
							break;
						}

					}
				}
				if ((GetAsyncKeyState('A') & 0x8000) != 0)
				{
					pos -= (-right * moveSpeed * tick);
					forwardBoundingSphere.center = pos;
					bool collision = false;
					for (auto iter = trees.begin(); iter != trees.end(); ++iter)
					{
						collision = Collision::IsSphereToSphere(forwardBoundingSphere, (*iter)->GetBoundingSphere());
						if (collision)
						{
							pos = position;
							break;
						}

					}
				}
				else if ((GetAsyncKeyState('D') & 0x8000) != 0)
				{
					pos += (-right * moveSpeed * tick);
					forwardBoundingSphere.center = pos;
					bool collision = false;
					for (auto iter = trees.begin(); iter != trees.end(); ++iter)
					{
						collision = Collision::IsSphereToSphere(forwardBoundingSphere, (*iter)->GetBoundingSphere());
						if (collision)
						{
							pos = position;
							break;
						}

					}
				}//점프 방향 if 끝
			}
		}
	}
	break;
	//죽음
	case CharacterState::CHARACTER_DEAD:
	{
		isDead = true;
		float tick = GameManager::GetTick();
		currentAnimationTime += tick;
		//죽는 애니메이션이 끝날때까지 상태 유지
		if (currentAnimationTime > selectedAnimationLength)
		{
			ChangeCharacterState(CharacterState::CHARACTER_DIED);
		}

	}
	break;
	//애니메이션 움직임도 없는 상태. 실제로는 마지막 1프레임만 따서 넣었다.
	case CharacterState::CHARACTER_DIED:
		isDead = true;
		break;
	default:
		break;
		
	}
	//지금까지 바꾼 현재 State에서 나온 position을 업데이트해줌.
	SetPosition(pos);
}
//나무와 충돌검사하는 코드를 따로 빼놓음. loop돌면서 충돌검사
bool Player::CollideTrees()
{
	bool collision = false;
	if (!trees.empty())
	{
		for (auto iter = trees.begin(); iter != trees.end(); ++iter)
		{
			collision = Collision::IsSphereToSphere(forwardBoundingSphere, (*iter)->GetBoundingSphere());
			if (collision)
			{
				break;
			}

		}
		
	}
	return collision;
}
//돌과 충돌검사하는 함수. 현재는 내용이 없다.
bool Player::CollideRocks()
{
	return false;
}
//몬스터와 충돌검사하는 함수. 그전에 피격이 되기때문에 그리 사용될 일은 없을것.
bool Player::CollideMonsters()
{
	bool collision = false;
	if (!monsters.empty())
	{
		for (auto iter = monsters.begin(); iter != monsters.end(); ++iter)
		{
			collision = Collision::IsSphereToSphere(forwardBoundingSphere, (*iter)->GetBoundingSphereValue());
			if (collision)
			{
				break;
			}

		}
	}
	
		return collision;
}
//몬스터를 때리는 함수. 공격판정을 적용한다.
bool Player::HitMonsters(BoundingSphere& attacksphere,int damage)
{
	bool collision = false;
	if (!monsters.empty())
	{
		for (auto iter = monsters.begin(); iter != monsters.end(); ++iter)
		{
			
				collision = Collision::IsSphereToSphere(attacksphere, (*iter)->GetBoundingSphereValue());
				if (collision)
				{
					//이미 피격중일 때는 맞지 않고, 피격중이 아니라면 맞는다. 
					bool monsterHit = (*iter)->GetIsHit();
					if (monsterHit)
					{
						
					}
					else
					{
						//몬스터의 피격함수를 호출. 이 함수는 캐릭터 클래스에 있다.
						DealDamage(*iter, damage);
					}
				}
			

		}
	}

	return collision;
}

void Player::Hit(float damage)
{
	isHit = true;
	indamage = damage;

}