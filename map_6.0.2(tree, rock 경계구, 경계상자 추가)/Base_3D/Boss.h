#pragma once
#include "Monster.h"
class Boss :
	public Monster
{
public:
	Boss();
	virtual ~Boss();
	virtual void Update() override;
protected:
	void ProcessState() override;
	float hp = 250.0f;
};

