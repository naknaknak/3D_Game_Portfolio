#pragma once
#include "Monster.h"
class Minion :
	public Monster
{
public:
	Minion();
	virtual ~Minion();
	virtual void Update() override;
	virtual void Initialize(char* path, char* filename) override;

	virtual void ChangeCharacterState(CharacterState state);
	virtual void InitializeCoolTime();
};

