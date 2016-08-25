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
	animationNames[CharacterState::CHARACTER_ATTACK1] = "Attack1";
	animationNames[CharacterState::CHARACTER_ATTACK2] = "Attack2";
	animationNames[CharacterState::CHARACTER_ATTACK3] = "Attack3";
	animationNames[CharacterState::CHARACTER_JUMP] = "Jump";
	animationNames[CharacterState::CHARACTER_DODGE] = "Dodge";
	animationNames[CharacterState::CHARACTER_DODGE_F] = "Dodge_F";
	animationNames[CharacterState::CHARACTER_DODGE_B] = "Dodge_B";
	animationNames[CharacterState::CHARACTER_DODGE_L] = "Dodge_L";
	animationNames[CharacterState::CHARACTER_DODGE_R] = "Dodge_R";
	animationNames[CharacterState::CHARACTER_TRACE] = "Sprint";
	animationNames[CharacterState::CHARACTER_DEAD] = "Dead";
	animationNames[CharacterState::CHARACTER_DIED] = "Died";
	animationNames[CharacterState::CHARACTER_SPRINT] = "Sprint";
	animationNames[CharacterState::CHARACTER_SKILL1] = "Skill1";
	animationNames[CharacterState::CHARACTER_HIT] = "Hit";//수정할것
}
void SkinnedCharacter::DealDamage(SkinnedCharacter* hitCharacter, int damage)
{
	hitCharacter->Hit(damage);
	
}
void SkinnedCharacter::Hit(int damage)
{
	isHit = true;
	hp -= damage;
	
}