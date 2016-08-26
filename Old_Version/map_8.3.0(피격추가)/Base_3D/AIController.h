#pragma once
class AIController
{
public:
	AIController();
	~AIController();
	void InitializeCooltime();
	void UpdatePositon(CharacterState state, D3DXVECTOR3* position);
	void UpdateRotation(CharacterState state, float* rotationAngleY);
protected:
	//dijkstra dijk;
};

