#include "stdafx.h"
#include "Player.h"

const float Player::skillCastingTime = 2.0f;

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

void Player::InitializeAnimation()
{
	animationNames[CharacterState::CHARACTER_IDLE] = "Idle";
	animationNames[CharacterState::CHARACTER_MOVE] = "Run";
	animationNames[CharacterState::CHARACTER_ATTACK] = "Baa";

}

void Player::UpdateAndRender()
{
	D3DXVECTOR3 pos = position;
	D3DXVECTOR3 direction;
	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);
	D3DXVECTOR3 forward = D3DXVECTOR3(0, 0, 1);
	D3DXVECTOR3 right;
	
	D3DXMATRIXA16 rotation;

	rotationAngle = GameManager::GetCamera()->GetRotateY();
	
	D3DXMatrixRotationY(&rotation, rotationAngle);
	D3DXVec3TransformCoord(&direction, &forward, &rotation);
	
	skill1Pos = pos + direction*20.0f;
	skill1Sphere.center = skill1Pos;


	D3DXVec3Cross(&right, &direction, &up);
	rotationAngle += D3DX_PI;
	D3DXMatrixRotationY(&rotation, rotationAngle);
	
	switch (currentState)
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
				//SetAnimationName("Run");

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
			
			if (!move)
			{
				currentState = CharacterState::CHARACTER_IDLE;
				SetAnimationName("Idle");
			}
			else
			{
				currentState = CharacterState::CHARACTER_MOVE;
				SetAnimationName("Run");
			}
			//state transition
			if ((GetAsyncKeyState(VK_SPACE) & 0x8000) != 0)
			{
				ChangeCharacterState(CharacterState::CHARACTER_ATTACK);
			}
			else if ((GetAsyncKeyState('1') & 0x8000) != 0)
			{
				ChangeCharacterState(CharacterState::CHARACTER_SKILL1);
			}
		}
	}
	break;
	case CharacterState::CHARACTER_ATTACK:
	{
		double tick = GameManager::GetTick();
		currentAnimationTime += tick;
		if (currentAnimationTime >= selectedAnimationLength)
		{
			ChangeCharacterState(CharacterState::CHARACTER_IDLE);
		}
		break;
	}
	case CharacterState::CHARACTER_SKILL1:
	{
		double tick = GameManager::GetTick();
		currentAnimationTime += tick;
		skill1Sphere.radius = sinf((float)currentAnimationTime * D3DX_PI) * (float)maxSkill1Radius;
		if (currentAnimationTime >= skillCastingTime)
		{
			ChangeCharacterState(CharacterState::CHARACTER_IDLE);
		}
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
	Debuging( );
}
void Player::Debuging( )
{
	char str[256];
	RECT rc = { 10, 90, 10, 10 };
	sprintf_s(str, "Player_position: x: %.3f / y: %.3f / z: %.3f", position.x, position.y, position.z);
	UI_Manager::GetFont( )->DrawTextA(nullptr, str, strlen(str), &rc, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));

}
\
void Player::ChangeCharacterState(CharacterState state)
{
	currentState = state;
	SetAnimationName(animationNames[state].c_str(), &selectedAnimationLength);
	animController->SetTrackPosition(0, 0.0f);
	currentAnimationTime = 0.0f;
}