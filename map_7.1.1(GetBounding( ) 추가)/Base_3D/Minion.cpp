#include "stdafx.h"
#include "Minion.h"


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
	///wip
	switch (currentState)
	{
	case CharacterState::CHARACTER_IDLE:
		if (hm->GetHeight(position, position.x, position.z) != false)
		if(isHit = true) ChangeCharacterState(CharacterState::CHARACTER_HIT);
		break;
	case CharacterState::CHARACTER_HIT:
		double tick = GameManager::GetTick();
		currentAnimationTime += tick;
		if (currentAnimationTime >= selectedAnimationLength)
		{
			ChangeCharacterState(CharacterState::CHARACTER_IDLE);
			isHit = false;
		}
		break;
	}
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