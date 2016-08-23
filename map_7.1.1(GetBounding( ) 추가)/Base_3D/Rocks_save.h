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

	inline void SetWireMode(bool wireMode) { this->wireMode = wireMode; }
	inline void SetIsColliderBoxDraw(bool IsColliderBoxDraw) { this->IsColliderBoxDraw = IsColliderBoxDraw; }
	inline void SetIsColliderSphereDraw(bool isColliderSphereDraw) { this->isColliderSphereDraw = isColliderSphereDraw; }

private:
	std::vector<Rock_location*> vRocks;

	bool wireMode = false;
	bool IsColliderBoxDraw = true;
	bool isColliderSphereDraw = true;
};

