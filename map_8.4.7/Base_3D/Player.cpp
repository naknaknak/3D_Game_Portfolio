#include "stdafx.h"
#include "Player.h"

/* �÷��̾� Ŭ����
// ���Ϳ� �ٸ� �κ��� ��ų, ���۰� �����̴�.
*/

//��� �� int �ƴ� �͵�
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
//�ʱ�ȭ �Լ�
//���� �ε�
//�ִϸ��̼� �̸�, ��ȣ�� �ִ� �ִϸ��̼� �ʱ�ȭ
//HP, ����(�ٿ�� ����) �ʱ�ȭ�� ����
void Player::Initialize(char* path, char* filename)
{
	SkinnedMesh::Initialize( path,  filename);

	InitializeAnimation();
	forwardBoundingSphere.center = boundingSphere.center;
	forwardBoundingSphere.radius = boundingSphere.radius;
	
	hp = 100;

}

//Update�Լ�
//�ٸ� ĳ���Ϳ� ���� �����ϴ�. State�� Update�ϰ� ���� state���� ���� SRT�� �ݿ���Ŵ
void Player::Update()
{
	
	rotationAngle = GameManager::GetCamera()->GetRotateY();

	D3DXMatrixRotationY(&rotation, rotationAngle);
	
	//rotation����� ProcessState���� ���⋚���� �̸� rotation�� ������ش�.
	//postion�� processState���� �ٲ�� ������ Translation�� ���߿� �Ѵ�.
	ProcessState(currentState);
	
	D3DXMatrixTranslation(&translate, position.x, position.y, position.z);
	D3DXMatrixScaling(&scale, scaleFactor, scaleFactor, scaleFactor);

	world = scale*rotation*translate;
	
	
}

//Render�Լ�. ����Ŭ�������� ������ϴ� �κи� �߰��Ǿ� �ִ�.
void Player::Render( )
{
	SkinnedMesh::Render( );
	Debugging( );
}
//ȭ�鿡 ���� ��ġ, �ִϸ��̼� �̸��� ����ִ� �Լ�
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
//�ִϸ��̼��� �ٲ��ִ� �Լ�
//state������ �ִϸ��̼��� ���� �����̱� ������ state������ ���⼭ �ٲ��ش�.
//FSM�� Ŭ������ ���ν�Ű�鼭 ���� �ٲ� ����
void Player::ChangeCharacterState(CharacterState state)
{
	currentState = state;
	SetAnimationName(animationNames[state].c_str(), &selectedAnimationLength);
	animController->SetTrackPosition(0, 0.0f);
	animController->SetTrackSpeed(0, 1.5f);
	selectedAnimationLength *= 0.67f;
	currentAnimationTime = 0.0f;
}


//�������̰� �ϴ� �κ��� �� ���ľ��ҵ�.
//���� State���� ������ �ϰ� State transition�� ��� �ִ�.
void Player::ProcessState(CharacterState state)
{
	//�����̱� ���� �ʿ��� ������
	D3DXVECTOR3 pos = position;
	
	D3DXVECTOR3 direction;
	D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0);
	D3DXVECTOR3 forward = D3DXVECTOR3(0, 0, 1);
	D3DXVECTOR3 right;

	//�ٶ󺸴� ����, �������� �̸� �������´�
	direction = D3DXVECTOR3(rotation._31, rotation._32, rotation._33);
	right = D3DXVECTOR3(-rotation._11, -rotation._12,- rotation._13);
	
	isDead = false;
	//State transition, state stay������ ��� �ִ� �κ�
	switch (state)
	{
		//Idle, Move�� Ư���� State. changeState�� �ٲ��� �ʴ´�.
	case CharacterState::CHARACTER_IDLE:
	case CharacterState::CHARACTER_MOVE:
	{
		//�ٴ��浹�˻���� �Ѵ�
		if (hm->GetHeight(pos, pos.x, pos.z) != false)
		{
			//�ʵ� �ȿ� �ִٸ�
			float tick = (float)GameManager::GetTick();
			bool move = false;
			//Ű �Է��� �޾� �����δ�. �� �� �� tick ���� �ڽŰ� �繰�� Collisition detection�Ѵ�.
			if ((GetAsyncKeyState('W') & 0x8000) != 0)
			{
				//�̵�, ���� ƽ�� ���ؼ� �浹�˻�
				pos -= (-direction * moveSpeed * tick);
				forwardBoundingSphere.center = pos;
				
				//�浹�˻�. ������ �˻�. ���� ���߿� �Ϸ��� ���ܵ�.
				//�浹���ϸ� state�� MOVE�� �ٲ��ָ鼭 ������, �浹�ϸ� ���ڸ��� �����ִ´�.
				bool collisionTrees = CollideTrees();
				bool collisionRocks = false;
				
				if (collisionTrees || collisionRocks)
				{
					//���ư���
					pos = position;
					break;
				}
				else
				{
					//�浹���ϸ� �����δ�.
					if (currentState == CharacterState::CHARACTER_IDLE)
					{
						ChangeCharacterState(CharacterState::CHARACTER_MOVE);
					}
					move = true;
				}
			}//w key
			else if ((GetAsyncKeyState('S') & 0x8000) != 0)
			{
				//��������� ���� �浹�ڵ�. �������� �浹�κ��� �����Ƿ� �ٲ�� ��.
				//�ٸ� ���ο��� ����� ����
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
				//s�� ����
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
				//s�� ����
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

			}//d, wasd ��

			//move����, �� ƽ���� �������� ���� IDLE�� �ִϸ��̼��� �ٲ��� ����
			if ((!move) && currentState == CharacterState::CHARACTER_MOVE)
			{
				ChangeCharacterState(CharacterState::CHARACTER_IDLE);
			}


			//�ٸ� state��� �ٲٴ� �κ�
			if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0)//����, ���콺 ����
			{
				ChangeCharacterState(CharacterState::CHARACTER_ATTACK);
			}
			else if ((GetAsyncKeyState('1') & 0x8000) != 0)//1��ų 1
			{
				ChangeCharacterState(CharacterState::CHARACTER_SKILL1);
			}
			else if ((GetAsyncKeyState(VK_LSHIFT) & 0x8000) != 0)//�޸��� ���� ����Ʈ
			{
				moveSpeed *= SPRINT_MULTYPLICATION;//�ӵ��� �����ְ� �ִϸ��̼��� �ٲ۴�
				ChangeCharacterState(CharacterState::CHARACTER_SPRINT);
			}
			if ((GetAsyncKeyState('C') & 0x0001) != 0)//ȸ�� C
			{
				//�̵��������� �ִϸ��̼��� �ٸ��ɷ� �ٲ۴�. 4�������� ȸ�ǰ���.
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
			if (isHit)//�ǰ�
			{
				if (hp > 0)//����ִ�
				{
					hp -= indamage;//HP�� ����
					ChangeCharacterState(CharacterState::CHARACTER_HIT); //��������� ���⼭ �ٽ��Ѵ�.
				}
				else
				{
					ChangeCharacterState(CharacterState::CHARACTER_DEAD); //����
				}
			
			}
			if (GetAsyncKeyState('X') & 0x8000 != 0)//����Ű X
			{
				ChangeCharacterState(CharacterState::CHARACTER_JUMP);
			}
		}
	}
	break;//idlemove ��
	//����
	case CharacterState::CHARACTER_ATTACK:
	{
		//�ִϸ��̼� �ð��� ���
		double tick = GameManager::GetTick();
		currentAnimationTime += tick;
		//��������. sphere�� ������ �Ѵ�. 
		BoundingSphere attackSphere;
		attackSphere.center = position + meshCenter + forward*0.5f;
		attackSphere.radius = ATTACK_RANGE;
		//�ִϸ��̼� ������, �ȳ������� �������� �浹ó��
		if (currentAnimationTime >= selectedAnimationLength)
		{
			ChangeCharacterState(CharacterState::CHARACTER_IDLE);
		}
		else
		{
			bool hit = HitMonsters(attackSphere,ATTACK_DAMAGE);
			
		}//������ ����
		if (isHit)
		{
			//���� ����
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
	break;//attack ��
	//1��ų. ����� ��������(������)���� �����س���. ���� Ÿ�� ��� �̸� ����� �� �� ������.
	//����� sin�Լ��� ���� �ð��� ���� Ŀ���ٰ� �پ��.
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
	if (isHit)//��ų���ٰ� �¾Ҵ�.
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
	//skill1 ��
	//�޸���
	case CharacterState::CHARACTER_SPRINT:
	{
		//�̵��� �⺻������ ��������� ���Ϳ� �浹������ ���Ѵ�.
		if (hm->GetHeight(pos, pos.x, pos.z) != false)
		{
			//�̵�
			float tick = (float)GameManager::GetTick();
			bool move = false;
			if ((GetAsyncKeyState('W') & 0x8000) != 0)
			{

				//�ڷδ� �Ȱ���
				pos -= (-direction * moveSpeed * tick);
				if ((GetAsyncKeyState('A') & 0x8000) != 0)//�밢�� �������� �ܼ��� �ӵ��� �����ش�.
				{
					pos -= (-right * moveSpeed * tick);
				}
				else if ((GetAsyncKeyState('D') & 0x8000) != 0)
				{
					pos += (-right * moveSpeed * tick);
				}
				
				forwardBoundingSphere.center = pos;
				bool collision = false;
				//�浹�ϸ� IDLE��
				for (auto iter = trees.begin(); iter != trees.end(); ++iter)
				{
					collision = Collision::IsSphereToSphere(forwardBoundingSphere, (*iter)->GetBoundingSphere());
					if (collision)
					{
						//Ż���ڵ�
						pos = position;
						moveSpeed /= SPRINT_MULTYPLICATION ;
						ChangeCharacterState(CharacterState::CHARACTER_IDLE);
						break;
					}

				}

			}//w�� ���ͼ� ������ ���� ��
			//������ �� �� �� state�� Ż���Ѵ�.
			else
			{
				//Ż���ڵ�
				moveSpeed /= SPRINT_MULTYPLICATION ;
				ChangeCharacterState(CharacterState::CHARACTER_IDLE);
			}
		}
		if (isHit)//�޸��ٰ� ������
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
	break;//sprint ��
	//�ǰ�. ���� ��� ���¿��� �� �ǰݻ��·� �̵�����
	case CharacterState::CHARACTER_HIT:
	{
		//�ð�����
		float tick = GameManager::GetTick();
		currentAnimationTime += tick;
		//�ʹ� �ִϸ��̼��� ���� ����
		animController->SetTrackSpeed(0, 0.2f);
		//���� ���̿� �ִϸ��̼� ������ �ð��� ��
		if (currentAnimationTime >= selectedAnimationLength*5.0f)
		{
			//�����ڵ�. IDLE�� ����
			isHit = false;
			ChangeCharacterState(CharacterState::CHARACTER_IDLE);
		}
		else {
			//�ִϸ��̼� �����߿��� ȸ�ǰ����ϴ�. ȸ���ڵ带 ����.
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

			}//��� �´� ��츦 ����ؼ� 1/2�ð����� ����Ÿ��. ������ �´´�
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
			else isInvisible = true;//����Ÿ�ӿ� ����. ���� ���Ǿ�� �浹���� ��ü�� Disable��
		}

	}
	break;
	//ȸ�� ����
	case CharacterState::CHARACTER_DODGE_L:
	{
		alpha = 0.2f;
		float tick = (float)GameManager::GetTick();
		currentAnimationTime += tick;
		D3DXMATRIXA16 dodgeRot;
		//�ణ ������ ���鼭 ȸ���ϰ� �������.
		if (currentAnimationTime >= selectedAnimationLength)
		{
			isInvisible = false;
			alpha = 1.0f;

			ChangeCharacterState(CharacterState::CHARACTER_IDLE);
		}
		else
		{
			//Y�� ȸ������ �ð��� ���� �ø���, �� ȸ�� ������ right, �� ������⿡ ���Ѵ�. �׸��� �� �������� �̵��Ѵ�.
			D3DXMatrixRotationY(&dodgeRot, currentAnimationTime * DODGE_ROTATION_FACTOR);
			D3DXVec3TransformCoord(&right, &right, &dodgeRot);
			pos += (right * dodgeSpeed * tick);
		}
	}
	break;
	//���ʰ� ����
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
	//������ ȸ���Ѵ�. 
	case CharacterState::CHARACTER_DODGE_F:
	{
		alpha = 0.2f;//�������ϰ� �� �����̾����� ���� �ȵ�.
		float tick = (float)GameManager::GetTick();
		currentAnimationTime += tick;
		D3DXMATRIXA16 dodgeRot;

		if (currentAnimationTime >= selectedAnimationLength)
		{
			isInvisible = false;
			alpha = 1.0f; //���İ��� �ȵ�. ��������
			ChangeCharacterState(CharacterState::CHARACTER_IDLE);
		}
		else
		{
			pos += direction*dodgeSpeed*tick;
	}
	}
	break;
	//�ڷ� ȸ���Ѵ�.
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
	//����
	case CharacterState::CHARACTER_JUMP:
	{
		float floorY;
		if (hm->GetHeight(floorY, pos.x, pos.z) != false)
		{
			float tick = (float)GameManager::GetTick();
			currentAnimationTime += tick;
			//��ġ����� ������ġ + �ӵ�*tick
			pos.y = position.y+(jumpSpeed - currentAnimationTime*GRAVITY_ACCEL*jumpConstant)*tick;
			if (currentAnimationTime >= selectedAnimationLength)
			{
				//���� �ִϸ��̼Ǻ��� ������ �ִϸ��̼��� �����Ѵ�
				animController->SetTrackSpeed(0, 0.0f);
			}
			if (pos.y < floorY)
			{
				//�ٴں��� �������� ����
				pos.y = floorY;
				ChangeCharacterState(CharacterState::CHARACTER_IDLE);
			}
			else
			{
				//���������� �����ڵ带 ����
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
				}//���� ���� if ��
			}
		}
	}
	break;
	//����
	case CharacterState::CHARACTER_DEAD:
	{
		isDead = true;
		float tick = GameManager::GetTick();
		currentAnimationTime += tick;
		//�״� �ִϸ��̼��� ���������� ���� ����
		if (currentAnimationTime > selectedAnimationLength)
		{
			ChangeCharacterState(CharacterState::CHARACTER_DIED);
		}

	}
	break;
	//�ִϸ��̼� �����ӵ� ���� ����. �����δ� ������ 1�����Ӹ� ���� �־���.
	case CharacterState::CHARACTER_DIED:
		isDead = true;
		break;
	default:
		break;
		
	}
	//���ݱ��� �ٲ� ���� State���� ���� position�� ������Ʈ����.
	SetPosition(pos);
}
//������ �浹�˻��ϴ� �ڵ带 ���� ������. loop���鼭 �浹�˻�
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
//���� �浹�˻��ϴ� �Լ�. ����� ������ ����.
bool Player::CollideRocks()
{
	return false;
}
//���Ϳ� �浹�˻��ϴ� �Լ�. ������ �ǰ��� �Ǳ⶧���� �׸� ���� ���� ������.
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
//���͸� ������ �Լ�. ���������� �����Ѵ�.
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
					//�̹� �ǰ����� ���� ���� �ʰ�, �ǰ����� �ƴ϶�� �´´�. 
					bool monsterHit = (*iter)->GetIsHit();
					if (monsterHit)
					{
						
					}
					else
					{
						//������ �ǰ��Լ��� ȣ��. �� �Լ��� ĳ���� Ŭ������ �ִ�.
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