#pragma once
#include "BoundingVolume.h"

class Collider_Sphere
{
public:
	Collider_Sphere();
	~Collider_Sphere();

	void Initialize(std::vector<D3DXVECTOR3>& pos, D3DXMATRIXA16* trans = nullptr, D3DXMATRIXA16* rot = nullptr);
	void MinMaxVertices(std::vector<D3DXVECTOR3>& pos, const D3DXMATRIXA16 * matRot = nullptr);
	void Destroy();
	void Update();
	void Render();

	inline void SetWireRender(bool wire){ wireMode = wire; }
	inline BoundingSphere& GetBoundingSphere(){ return boundingSphere; }

	inline void SetMaterialColor(D3DXCOLOR& color){ material.Ambient = color; material.Diffuse = color; }

protected:
	BoundingSphere boundingSphere = BoundingSphere(D3DXVECTOR3(0, 0, 0), 1.0f);
	LPD3DXMESH sphereMesh = nullptr;

	D3DXMATRIXA16 world;
	D3DXMATRIXA16* parent = nullptr;

	bool wireMode = false;

	D3DMATERIAL9 material;
	//
	D3DXVECTOR3 vecMax = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 vecMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
};

