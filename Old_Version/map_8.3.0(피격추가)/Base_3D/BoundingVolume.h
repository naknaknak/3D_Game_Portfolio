#pragma once

enum BoundingVolumeType
{
	SPHERE = 0,
	AABB,
};

struct BoundingSphere
{
	BoundingSphere(){};
	BoundingSphere(D3DXVECTOR3& _center, float _radius)
	{
		radius = _radius;
		center = _center;
	}

	float radius = 0.0f;
	D3DXVECTOR3 center = D3DXVECTOR3(0, 0, 0);
};

struct BoundingBox
{
	BoundingBox(){};
	BoundingBox(D3DXVECTOR3 _min, D3DXVECTOR3 _max)
	{
		min = _min;
		max = _max;
	}
	D3DXVECTOR3 min = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	D3DXVECTOR3 max = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
};