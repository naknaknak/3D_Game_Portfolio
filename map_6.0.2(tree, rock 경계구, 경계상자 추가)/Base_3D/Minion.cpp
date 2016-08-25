#include "stdafx.h"
#include "Minion.h"
#include "AIController.h"

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
}

void Minion::Update()
{
	ProcessState();
	


	SetPosition(position);
	

	D3DXMatrixTranslation(&translate, position.x, position.y, position.z);
	D3DXMatrixScaling(&scale, scaleFactor, scaleFactor, scaleFactor);
	D3DXMatrixIdentity(&rotation);
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
void Minion::InitializeCoolTime()
{

}
void Minion::ProcessState()
{
	D3DXMatrixRotationY(&rotation, rotationAngle);
	D3DXVECTOR3 forward = D3DXVECTOR3(rotation._31, rotation._32, rotation._33);
	switch (currentState)
	{
	case CharacterState::CHARACTER_IDLE:
		SelectPatrolPosition();
		ChangeCharacterState(CharacterState::CHARACTER_PATROL);
		break;
	case CharacterState::CHARACTER_PATROL:
		
		if (Collision::IsSphereToSphere(sight_wide, player->GetBoundingSphereValue()))
		{
			D3DXVECTOR3 toPlayer = player->GetPosition() - position;
			D3DXVec3Normalize(&toPlayer, &toPlayer);

			//cos60과 비교를 해서 내적값이 안에 있으면 쫒아감
			//중간에 어느쪽으로 돌아야 할지 원래 회전각에 90을 더해서 부호가 바뀌는지 판정한다. DX기본 회전은 CW이다.
			if (D3DXVec3Dot(&forward, &toPlayer)>cosf(D3DX_PI*0.33f))
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
					ChangeCharacterState(CharacterState::CHARACTER_TRACE);
				}
			}
		}
		break;
	case CharacterState::CHARACTER_ATTACK:
		break;
	case CharacterState::CHARACTER_DODGE:
		break;
	case CharacterState::CHARACTER_HIT:
		break;
	case CharacterState::CHARACTER_TRACE:
		break;
	case CharacterState::CHARACTER_RETURN:
		break;
	case CharacterState::CHARACTER_DEAD:
		break;
	case CharacterState::CHARACTER_DIED:
		break;
	}
}
void Minion::SelectPatrolPosition()
{
	
}