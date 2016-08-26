#include "stdafx.h"
#include "Boss.h"
const float Boss::ATTACK_DAMAGE = 20.0f;
const float Boss::ATTACK_RANGE = 12.0f;
const float Boss::BOSS_SIGHT_RANGE = 70.0f;
Boss::Boss()
{
}


Boss::~Boss()
{

}
void Boss::Initialize(char* path, char* filename)
{
	SkinnedMesh::Initialize(path, filename);
	InitializeAnimation();
	position = initialPositon;
	sight_wide.radius = BOSS_SIGHT_RANGE;

	attackSphere.radius = ATTACK_RANGE;
}
void Boss::Update()
{
	ProcessState();
	SetPosition(position);


	D3DXMatrixTranslation(&translate, position.x, position.y, position.z);
	D3DXMatrixScaling(&scale, scaleFactor, scaleFactor, scaleFactor);
	D3DXMatrixRotationY(&rotation, rotationAngle);
	world = scale*rotation*translate;
}

void Boss::ProcessState()
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
		toPlayerRotationAngle = atan2f(direction.x, direction.z);
	}
	else direction = forward;
	switch (currentState)
	{
		case CharacterState::CHARACTER_IDLE:
		{
			if (hm->GetHeight(pos.y, pos.x, pos.z) != false)
			{
				//state traslataion
				if (player)
				{
					if (Collision::IsSphereToSphere(sight_wide, player->GetBoundingSphereValue()))
					{
						target = player->GetPositionAddress();
						ChangeCharacterState(CharacterState::CHARACTER_TRACE);
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
		case CharacterState::CHARACTER_ATTACK1:
		{
			float tick = (float)GameManager::GetTick();

			currentAnimationTime += tick;
			attackSphere.center = meshCenter + position + direction*0.5f;
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
					if (Collision::IsSphereToSphere(attackSphere, player->GetBoundingSphereValue()))
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
		case CharacterState::CHARACTER_HIT:
		{
			float tick = (float)GameManager::GetTick();

			currentAnimationTime += tick;
			
			if (currentAnimationTime >= selectedAnimationLength)
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
					ChangeCharacterState(CharacterState::CHARACTER_ATTACK1);
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
			float tick = GameManager::GetTick();
			animController->SetTrackSpeed(0, 0.0f);
			isDead = true;
			currentAnimationTime += tick;
			if (pos.y > -10.0)
			{
				
				pos.y -= tick;
			}
			else
			{
			
			}
		}
		break;
	}
	position = pos;
}
void Boss::Hit(float damage)
{
	isHit = true;
	inDamage = damage;
}
bool Boss::CollisionMonsters(D3DXVECTOR3* direction)
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
void Boss::ChangeCharacterState(CharacterState state)
{
	currentState = state;
	SetAnimationName(animationNames[state].c_str(), &selectedAnimationLength);
	animController->SetTrackPosition(0, 0.0f);
	animController->SetTrackSpeed(0, 1.0f);
	currentAnimationTime = 0.0f;
}