#include "stdafx.h"
#include "Minion.h"

const float Minion::ATTACK_DAMAGE = 10.0f;
Minion::Minion()
{
}


Minion::~Minion()
{
}
void Minion::Initialize(char* path, char* filename)
{
	SkinnedMesh::Initialize(path, filename);
	InitializeAnimation();
	position = initialPositon;
	sight_wide.radius = 70.0f;
	sight_narrow.radius = 20.0f;
	attackSphere.radius = 5.0f; 
}

void Minion::Update()
{
	ProcessState();
	SetPosition(position);
	

	D3DXMatrixTranslation(&translate, position.x, position.y, position.z);
	D3DXMatrixScaling(&scale, scaleFactor, scaleFactor, scaleFactor);
	D3DXMatrixRotationY(&rotation, rotationAngle);
	world = scale*rotation*translate;
}
void Minion::ChangeCharacterState(CharacterState state)
{
	currentState = state;
	SetAnimationName(animationNames[state].c_str(), &selectedAnimationLength);
	animController->SetTrackPosition(0, 0.0f);
	animController->SetTrackSpeed(0, 1.0f);
	currentAnimationTime = 0.0f;
}
void Minion::ProcessState()
{
	D3DXVECTOR3 pos = position;
	D3DXVECTOR3 direction;
	D3DXVECTOR3 right;
	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);
	float toPlayerRotationAngle;
	D3DXMatrixRotationY(&rotation, rotationAngle);
	D3DXVECTOR3 forward = D3DXVECTOR3(rotation._31, rotation._32, rotation._33);
	if (target)
	{
		direction = *target - position;
		D3DXVec3Normalize(&direction, &direction);
		D3DXVec3Cross(&right, &up, &direction);
		toPlayerRotationAngle = atan2f(direction.x,direction.z);
	}
	else direction = forward;
	
	switch (currentState)
	{
	case CharacterState::CHARACTER_IDLE:
	{
		float tick = GameManager::GetTick();
		idleTime += tick;
		if (hm->GetHeight(pos.y, pos.x, pos.z) != false)
		{
		}
		SelectPatrolPosition();
		if (!player->GetIsDead())
		{
			ChangeCharacterState(CharacterState::CHARACTER_PATROL);
		}
		else
		{
			if (idleTime < (float)rand() / RAND_MAX * 2) {
				ChangeCharacterState(CharacterState::CHARACTER_PATROL);
			}
		}

	}break;
	case CharacterState::CHARACTER_PATROL:
	{
		if (hm->GetHeight(pos.y, pos.x, pos.z) != false)
		{
			if (player)
			{
				if (Collision::IsSphereToSphere(sight_wide, player->GetBoundingSphereValue()))
				{
					D3DXVECTOR3 toPlayer = player->GetPosition() - position;
					D3DXVec3Normalize(&toPlayer, &toPlayer);

					//cos60과 비교를 해서 내적값이 안에 있으면 쫒아감
					//중간에 어느쪽으로 돌아야 할지 원래 회전각에 90을 더해서 부호가 바뀌는지 판정한다. DX기본 회전은 CW이다.
					if (D3DXVec3Dot(&forward, &toPlayer) > cosf(D3DX_PI*0.33f))
					{
						if (D3DXVec3Dot(&toPlayer, &D3DXVECTOR3(cosf(rotationAngle + D3DX_PI*0.5f), 0, sinf(rotationAngle + D3DX_PI*0.5f))) > 0)
						{
							rotateCW = true;

						}
						else rotateCW = false;
						target = player->GetPositionAddress();
						ChangeCharacterState(CharacterState::CHARACTER_TRACE);
					}
					else
					{
						//시야각 밖에 있다.
						//좁은 원 안에 들어오면 판정을 함. 둘다 아니면 가던 길을 간다.
						if (Collision::IsSphereToSphere(sight_narrow, player->GetBoundingSphereValue()))
						{
							if (D3DXVec3Dot(&toPlayer, &D3DXVECTOR3(cosf(rotationAngle + D3DX_PI*0.5f), 0, sinf(rotationAngle + D3DX_PI*0.5f))) > 0)
							{
								rotateCW = true;

							}
							else rotateCW = false;
							target = player->GetPositionAddress();
							if (player->GetIsDead())
							{
								
							}
							else
							{
								ChangeCharacterState(CharacterState::CHARACTER_TRACE);
							}
						}

					}
				}
			}
			if (isHit)
			{
				hp -= inDamage;
				if (hp > 0)
				{
					ChangeCharacterState(CharacterState::CHARACTER_HIT);
				}
				else ChangeCharacterState(CharacterState::CHARACTER_DEAD);
			}
			
		}
	}break;
	case CharacterState::CHARACTER_ATTACK:
	{
		float tick = (float)GameManager::GetTick();
		
		currentAnimationTime += tick;
		attackSphere.center = meshCenter+position + direction*0.5f;
		if (currentAnimationTime >= selectedAnimationLength)
		{
			ChangeCharacterState(CharacterState::CHARACTER_TRACE);
		}
		else
		{
			bool playerInivisible = player->GetInvisible();
			bool playerHit = player->GetIsHit();
			bool playerDead = player->GetIsDead();
			if (!playerInivisible && !playerHit&&!playerDead&&!attackHit)
			{
				if(Collision::IsSphereToSphere(attackSphere, player->GetBoundingSphereValue()))
				{
					if (Collision::IsBoxToSphere(player->GetBoundingBoxValue(), attackSphere))
					{
						DealDamage(player, ATTACK_DAMAGE);
						attackHit = true;
					}
				}
			}
			if (isHit)
			{
				hp -= inDamage;
				if (hp > 0)
				{
					ChangeCharacterState(CharacterState::CHARACTER_HIT);
				}
				else ChangeCharacterState(CharacterState::CHARACTER_DEAD);
			}
		}
	}
		break;
	case CharacterState::CHARACTER_DODGE:
	{

	}
		break;
	case CharacterState::CHARACTER_HIT:
	{
		float tick = (float)GameManager::GetTick();

		currentAnimationTime += tick;
		animController->SetTrackSpeed(0, 0.2f);
		if (currentAnimationTime >= selectedAnimationLength*5.0f)
		{
			isHit = false;
			ChangeCharacterState(CharacterState::CHARACTER_TRACE);
		}
	}
		break;
	case CharacterState::CHARACTER_TRACE:
	{
		if (hm->GetHeight(pos, pos.x, pos.z) != false)
		{
			float tick = (float)GameManager::GetTick();
			bool move = false;
			D3DXVECTOR3 collisionDirection;
			if (CollisionMonsters(&collisionDirection))
			{
				direction += collisionDirection;
				D3DXVec3Normalize(&direction, &direction);
			}
			if (Collision::IsSphereToSphere(player->GetBoundingSphereValue(), boundingSphere))
			{
				attackHit = false;
				ChangeCharacterState(CharacterState::CHARACTER_ATTACK);
			}
			else
			{
				pos -= (-direction * moveSpeed * tick);
				forwardBoundingSphere.center = pos;
			}
			rotationAngle = toPlayerRotationAngle;
		}
		if (isHit)
		{
			hp -= inDamage;
			if (hp > 0)
			{
				ChangeCharacterState(CharacterState::CHARACTER_HIT);
			}
			else ChangeCharacterState(CharacterState::CHARACTER_DEAD);
		}
		if (player->GetIsDead())
		{
			ChangeCharacterState(CharacterState::CHARACTER_PATROL);
		}
	}
		break;
	case CharacterState::CHARACTER_RETURN:
	{

	}
		break;
	case CharacterState::CHARACTER_DEAD:
	{
		
		float tick = (float)GameManager::GetTick();

		currentAnimationTime += tick;
		
		if (currentAnimationTime >= selectedAnimationLength)
		{
			ChangeCharacterState(CharacterState::CHARACTER_DIED);
		}
	}
		break;
	case CharacterState::CHARACTER_DIED:
	{
		float tick=GameManager::GetTick();
		animController->SetTrackSpeed(0, 0.0f);
		
		currentAnimationTime += tick;
		if (pos.y > -10.0)
		{
			pos.y -= tick;
		}
		else
		{
			isDead = true;
		}
	}
		break;
	}
	position = pos;
}
void Minion::SelectPatrolPosition()
{
	
}
bool Minion::CollisionMonsters(D3DXVECTOR3* direction)
{
	bool collision = false;
	if (!monsters.empty())
	{
		for (auto iter = monsters.begin(); iter != monsters.end(); ++iter)
		{

			collision = Collision::IsSphereToSphere(boundingSphere, (*iter)->GetBoundingSphereValue());
			if (collision)
			{
				*direction = boundingSphere.center - (*iter)->GetBoundingSphereValue().center;
				if (D3DXVec3LengthSq(direction) > 0.3)
				{
					break;
				}
				else
				{
					continue;
				}
			}


		}
	}

	return collision;
}
void Minion::Hit(float damage)
{
	isHit = true;
	inDamage = damage;

}