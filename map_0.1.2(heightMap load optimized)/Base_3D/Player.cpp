#include "stdafx.h"
#include "Player.h"


Player::Player()
{
}


Player::~Player()
{
}

void Player::InitializeAnimation()
{
	animationNames[PlayerState::PLAYER_IDLE] = "Idle";
	animationNames[PlayerState::PLAYER_MOVE] = "Run";
	animationNames[PlayerState::PLAYER_ATTACK] = "Baa";

}


void Player::UpdateAndRender()
{
	D3DXVECTOR3 pos = position;
	D3DXVECTOR3 direction = GameManager::GetCamera()->GetCameraDirectionXZ();
	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);
	D3DXVECTOR3 right;
	D3DXVECTOR3 forward = D3DXVECTOR3(0, 0, 1);
	D3DXVec3Cross(&right, &direction, &up);
	rotationAngle = 135.0f;
	D3DXMATRIXA16 rotation;
	rotationAngle += acosf(D3DXVec3Dot(&direction, &forward));
	D3DXMatrixRotationY(&rotation, rotationAngle);

	switch (currentState)
	{
		//Idle, Move만 특별한 State. changeState로 바꾸지 않는다.
	case PlayerState::PLAYER_IDLE:
	case PlayerState::PLAYER_MOVE:
	{
		if (hm->GetHeight(pos, pos.x, pos.z) != false)
		{
			float tick = (float)GameManager::GetTick();
			bool move = false;
			if ((GetAsyncKeyState('W') & 0x8000) != 0)
			{
				pos -= (-direction * moveSpeed * tick);
				SetAnimationName("Run");

				move = true;
			}
			else if ((GetAsyncKeyState('S') & 0x8000) != 0)
			{
				pos += (-direction * moveSpeed * tick);
				

				move = true;
			}
			if ((GetAsyncKeyState('A') & 0x8000) != 0)
			{
				pos -= (-right * moveSpeed * tick);
				

				move = true;
			}
			else if ((GetAsyncKeyState('D') & 0x8000) != 0)
			{
				pos += (-right * moveSpeed * tick);
				

				move = true;
			}
			
			if (!move) SetAnimationName("Idle");
			else SetAnimationName("Run");
			//state transition
			if ((GetAsyncKeyState(VK_SPACE) & 0x8000) != 0)
			{
				ChangePlayerState(PlayerState::PLAYER_ATTACK);
			}
		}
	}
	break;
	case PlayerState::PLAYER_ATTACK:
	{
		double tick = GameManager::GetTick();
		currentAnimationTime += tick;
		if (currentAnimationTime >= selectedAnimationLength)
		{
			ChangePlayerState(PlayerState::PLAYER_IDLE);
		}
		break;
	}
		
	default:
		break;
	}
		SetPosition(pos);
	//여기부턴 skinnedmesh와 같음

	if (animController)
	{
		animController->AdvanceTime(GameManager::GetTick(), nullptr);
	}

	if (rootFrame)
	{
		D3DXMATRIXA16 local;
		D3DXMatrixTranslation(&local, position.x, position.y, position.z);
		D3DXMATRIXA16 scale;
		D3DXMatrixScaling(&scale, scaleFactor.x, scaleFactor.y, scaleFactor.z);
		local = scale*rotation*local;
		Update(rootFrame, &local);
		Render(rootFrame);
	}
}

void Player::ChangeCharaterState(CharacterState state)
{
	//wip
}

void Player::ChangePlayerState(PlayerState state)
{
	currentState = state;
	SetAnimationName(animationNames[state].c_str(), &selectedAnimationLength);
	animController->SetTrackPosition(0, 0.0f);
	currentAnimationTime = 0;
}