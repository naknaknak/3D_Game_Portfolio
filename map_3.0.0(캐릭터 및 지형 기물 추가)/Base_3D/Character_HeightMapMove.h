#pragma once
#include "Character.h"
#include "HeightMap.h"


class Character_HeightMapMove:
	public Character
{
public:
	Character_HeightMapMove();
	~Character_HeightMapMove();
public:
	void GroundCheck();
	void SetHeightMap(HeightMap* hm) { this->hm = hm; }

private:
	HeightMap* hm = nullptr;
};

