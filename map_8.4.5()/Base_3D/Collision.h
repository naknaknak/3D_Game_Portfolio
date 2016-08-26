#pragma once
#include "BoundingVolume.h"

class Collision
{
public:
	Collision();
	~Collision();

	static void MinMaxVertices(OUT D3DXVECTOR3& vecMin, OUT D3DXVECTOR3& vecMax, const std::vector<D3DXVECTOR3> pos, const D3DXMATRIXA16 * matRot = nullptr);

	static bool IsSphereToSphere(BoundingSphere& s1, BoundingSphere& s2);
	static bool IsBoxToBox(BoundingBox& b1, BoundingBox& b2);
	static bool IsBoxToSphere(BoundingBox& b, BoundingSphere& s);
};

