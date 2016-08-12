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

void Player::Initialize(char* path, char* filename)
{
	SkinnedMesh::Initialize( path,  filename);

	InitializeAnimation();
	forwardBoundingSphere.center = boundingSphere.center;
	forwardBoundingSphere.radius = boundingSphere.radius;


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
	
	
	
	//여기부턴 skinnedmesh와 같음

	/*if (animController)
	{
		animController->AdvanceTime(GameManager::GetTick(), nullptr);
	}

	if (rootFrame)
	{
		D3DXMATRIXA16 local;
		D3DXMatrixTranslation(&local, position.x, position.y, position.z);
		D3DXMATRIXA16 scale;
		D3DXMatrixScaling(&scale, scaleFactor, scaleFactor, scaleFactor);
		local = scale*rotation*local;
		BoneUpdate(rootFrame, &local);
		Render(rootFrame);
	}*/
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
	animController->SetTrackSpeed(0, 1.0f);
	currentAnimationTime = 0.0f;
}


//못움직이게 하는 부분을 좀 고쳐야할듯.
void Player::ProcessState(CharacterState state)
{
	D3DXVECTOR3 pos = position;
	D3DXVECTOR3 prevPos = position;
	D3DXVECTOR3 direction;
	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);
	D3DXVECTOR3 forward = D3DXVECTOR3(0, 0, 1);
	D3DXVECTOR3 right;

	D3DXVec3TransformCoord(&direction, &forward, &rotation);
	D3DXVec3Cross(&right, &direction, &up);

	

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
				bool collision=false;
				for (auto iter = trees.begin(); iter != trees.end(); ++iter)
				{
					collision = Collision::IsSphereToSphere(forwardBoundingSphere, (*iter)->GetBoundingSphere());
					if (collision)
					{
						pos = prevPos;
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
						pos = prevPos;
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
						pos = prevPos;
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
						pos = prevPos;
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
			if ((GetAsyncKeyState(VK_SPACE) & 0x8000) != 0)
			{
				ChangeCharacterState(CharacterState::CHARACTER_ATTACK);
			}
			else if ((GetAsyncKeyState('1') & 0x8000) != 0)
			{
				ChangeCharacterState(CharacterState::CHARACTER_SKILL1);
			}
			else if ((GetAsyncKeyState(VK_LSHIFT) & 0x8000) != 0)
			{
				moveSpeed *= 2;
				ChangeCharacterState(CharacterState::CHARACTER_SPRINT);
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
		skill1Pos = position + direction*20.0f;
		skill1Sphere.center = skill1Pos;

		currentAnimationTime += tick;
		skill1Sphere.radius = sinf((float)currentAnimationTime * D3DX_PI) * maxSkill1Radius;
		animController->SetTrackSpeed(0, (float)selectedAnimationLength / skillCastingTime);
		if (currentAnimationTime >= skillCastingTime)
		{
			ChangeCharacterState(CharacterState::CHARACTER_IDLE);
		}
	}
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
						pos = prevPos;
						moveSpeed *= 0.5f;
						ChangeCharacterState(CharacterState::CHARACTER_IDLE);
						break;
					}

				}

			}
			else
			{
				moveSpeed *= 0.5f;
				ChangeCharacterState(CharacterState::CHARACTER_IDLE);
			}
		}
	break;
	}
	default:
		break;
	}
	SetPosition(pos);
}