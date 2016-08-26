#include "stdafx.h"
#include "Player.h"

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

void Player::Initialize(char* path, char* filename)
{
	SkinnedMesh::Initialize( path,  filename);

	InitializeAnimation();
	forwardBoundingSphere.center = boundingSphere.center;
	forwardBoundingSphere.radius = boundingSphere.radius;
	
	hp = 100;

}

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
void Player::Render( )
{
	SkinnedMesh::Render( );
	Debugging( );
}
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
void Player::ProcessState(CharacterState state)
{
	D3DXVECTOR3 pos = position;
	
	D3DXVECTOR3 direction;
	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);
	D3DXVECTOR3 forward = D3DXVECTOR3(0, 0, 1);
	D3DXVECTOR3 right;

	direction = D3DXVECTOR3(rotation._31, rotation._32, rotation._33);
	right = D3DXVECTOR3(-rotation._11, -rotation._12,- rotation._13);
	
	isDead = false;
	switch (state)
	{
		//Idle, Move만 특별한 State. changeState로 바꾸지 않는다.
	case CharacterState::CHARACTER_IDLE:
	case CharacterState::CHARACTER_MOVE:
	{
		if (hm->GetHeight(pos, pos.x, pos.z) != false)
		{
			float tick = (float)GameManager::GetTick();
			bool move = false;
			if ((GetAsyncKeyState('W') & 0x8000) != 0)
			{

				pos -= (-direction * moveSpeed * tick);
				forwardBoundingSphere.center = pos;
			bool collisionTrees = CollideTrees();
				bool collisionRocks = false;
				if (collisionTrees || collisionRocks)
				{
					pos = position;
					break;
				}
				else
				{
					if (currentState == CharacterState::CHARACTER_IDLE)
					{
						ChangeCharacterState(CharacterState::CHARACTER_MOVE);
					}
					move = true;
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
				if (!collision) {
					if (currentState == CharacterState::CHARACTER_IDLE)
					{
						ChangeCharacterState(CharacterState::CHARACTER_MOVE);
					}
					move = true;
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
				if (!collision) {
					if (currentState == CharacterState::CHARACTER_IDLE)
					{
						ChangeCharacterState(CharacterState::CHARACTER_MOVE);
					}
					move = true;
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
				if (!collision) {
					if (currentState == CharacterState::CHARACTER_IDLE)
					{
						ChangeCharacterState(CharacterState::CHARACTER_MOVE);
					}
					move = true;
				}

			}


			if ((!move) && currentState == CharacterState::CHARACTER_MOVE)
			{
				ChangeCharacterState(CharacterState::CHARACTER_IDLE);
			}


			//state transition
			if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0)
			{
				ChangeCharacterState(CharacterState::CHARACTER_ATTACK);
			}
			else if ((GetAsyncKeyState('1') & 0x8000) != 0)
			{
				ChangeCharacterState(CharacterState::CHARACTER_SKILL1);
			}
			else if ((GetAsyncKeyState(VK_LSHIFT) & 0x8000) != 0)
			{
				moveSpeed *= SPRINT_MULTYPLICATION;
				ChangeCharacterState(CharacterState::CHARACTER_SPRINT);
			}
			if ((GetAsyncKeyState('C') & 0x0001) != 0)
			{
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
			if (GetAsyncKeyState('X') & 0x8000 != 0)
			{
				ChangeCharacterState(CharacterState::CHARACTER_JUMP);
			}
		}
	}
	break;//idlemove
	case CharacterState::CHARACTER_ATTACK:
	{
		
		double tick = GameManager::GetTick();
		currentAnimationTime += tick;
		BoundingSphere attackSphere;
		attackSphere.center = position + meshCenter + forward*0.5f;
		attackSphere.radius = ATTACK_RANGE;
		if (currentAnimationTime >= selectedAnimationLength)
		{
			ChangeCharacterState(CharacterState::CHARACTER_IDLE);
		}
		else
		{
			bool hit = HitMonsters(attackSphere,ATTACK_DAMAGE);
			
		}
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
	break;//attack
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
	break;//skill1
	case CharacterState::CHARACTER_SPRINT:
	{
		if (hm->GetHeight(pos, pos.x, pos.z) != false)
		{
			float tick = (float)GameManager::GetTick();
			bool move = false;
			if ((GetAsyncKeyState('W') & 0x8000) != 0)
			{

				pos -= (-direction * moveSpeed * tick);
				if ((GetAsyncKeyState('A') & 0x8000) != 0)
				{
					pos -= (-right * moveSpeed * tick);
				}
				else if ((GetAsyncKeyState('D') & 0x8000) != 0)
				{
					pos += (-right * moveSpeed * tick);
				}
				forwardBoundingSphere.center = pos;
				bool collision = false;
				for (auto iter = trees.begin(); iter != trees.end(); ++iter)
				{
					collision = Collision::IsSphereToSphere(forwardBoundingSphere, (*iter)->GetBoundingSphere());
					if (collision)
					{
						pos = position;
						moveSpeed /= SPRINT_MULTYPLICATION ;
						ChangeCharacterState(CharacterState::CHARACTER_IDLE);
						break;
					}

				}

			}
			else
			{
				moveSpeed /= SPRINT_MULTYPLICATION ;
				ChangeCharacterState(CharacterState::CHARACTER_IDLE);
			}
		}
		if (isHit)
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
	break;//sprint
	case CharacterState::CHARACTER_HIT:
	{
		float tick = GameManager::GetTick();
		currentAnimationTime += tick;
		
		animController->SetTrackSpeed(0, 0.2f);
			if (currentAnimationTime >= selectedAnimationLength*5.0f)
			{
				isHit = false;
				ChangeCharacterState(CharacterState::CHARACTER_IDLE);
			}
			else {
				
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

				}
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
				else isInvisible = true;
			}

	}
		break;
	case CharacterState::CHARACTER_DODGE_L:
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
		
			D3DXMatrixRotationY(&dodgeRot, currentAnimationTime * DODGE_ROTATION_FACTOR);
			D3DXVec3TransformCoord(&right, &right, &dodgeRot);
			pos += (right * dodgeSpeed * tick);
		}
	}
	break;
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
	case CharacterState::CHARACTER_DODGE_F:
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
			pos += direction*dodgeSpeed*tick;
	}
	}
	break;
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
	case CharacterState::CHARACTER_JUMP:
	{
		float floorY;
		if (hm->GetHeight(floorY, pos.x, pos.z) != false)
		{
			float tick = (float)GameManager::GetTick();
			currentAnimationTime += tick;
			pos.y = position.y+(jumpSpeed - currentAnimationTime*GRAVITY_ACCEL*jumpConstant)*tick;
			if (currentAnimationTime >= selectedAnimationLength)
			{
				animController->SetTrackSpeed(0, 0.0f);
			}
			if (pos.y < floorY)
			{
				pos.y = floorY;
				ChangeCharacterState(CharacterState::CHARACTER_IDLE);
			}
			else
			{
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
				}
			}
		}
	}
	break;
	case CharacterState::CHARACTER_DEAD:
	{
		isDead = true;
		float tick = GameManager::GetTick();
		currentAnimationTime += tick;
		if (currentAnimationTime > selectedAnimationLength)
		{
			ChangeCharacterState(CharacterState::CHARACTER_DIED);
		}

	}
	break;
	case CharacterState::CHARACTER_DIED:
		isDead = true;
		break;
	default:
		break;
		
	}
	SetPosition(pos);
}
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

bool Player::CollideRocks()
{
	return false;
}
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
					bool monsterHit = (*iter)->GetIsHit();
					if (monsterHit)
					{
						
					}
					else
					{
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