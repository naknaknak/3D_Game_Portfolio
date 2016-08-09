#include "stdafx.h"
#include "SkinnedCharacter.h"


SkinnedCharacter::SkinnedCharacter()
{
}


SkinnedCharacter::~SkinnedCharacter()
{
}
void SkinnedCharacter::InitializeAnimation()
{
	animationNames[CharacterState::CHARACTER_IDLE] = "Idle";
	animationNames[CharacterState::CHARACTER_MOVE] = "Move";
	animationNames[CharacterState::CHARACTER_ATTACK] = "Attack";
	animationNames[CharacterState::CHARACTER_JUMP] = "Jump";
	animationNames[CharacterState::CHARACTER_DODGE] = "Dodge";
	animationNames[CharacterState::CHARACTER_DEAD] = "Dead";
	animationNames[CharacterState::CHARACTER_DIED] = "Died";
	animationNames[CharacterState::CHARACTER_SPRINT] = "Sprint";
	animationNames[CharacterState::CHARACTER_SKILL1] = "Skill1";
}