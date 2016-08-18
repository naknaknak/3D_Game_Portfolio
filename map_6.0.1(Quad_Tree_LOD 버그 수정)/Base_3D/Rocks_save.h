#pragma once

#include "Rock_location.h"

class Rocks_save
{
public:
	Rocks_save( );
	~Rocks_save( );

	void Initialize( );
	void Destroy( );
	void Update( );
	void AddRock(Rock_location* rl);
	void Render( );

private:
	std::vector<Rock_location*> vRocks;

};

