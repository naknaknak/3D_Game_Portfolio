#pragma once
#include "BoundingVolume.h"

class Collider_Box
{
public:
	Collider_Box( );
	~Collider_Box( );

	void Initialize(const  D3DXMATRIXA16 * world = nullptr);
	void Destroy( );
	void Update( );
	void Render( );

	inline void SetParent(D3DXMATRIXA16* _parent) { parent = _parent; }
	inline void SetWireRender(bool wire) { wireMode = wire; }
	inline void SetPosition(D3DXVECTOR3& pos) { position = pos; }
	inline BoundingBox& GetBoundingBox( ) { return currentBoundingBox; }

	inline void SetMaterialColor(D3DXCOLOR& color) { material.Ambient = color; material.Diffuse = color; }

	//
	D3DXVECTOR3 GetMax( ) { return vecMax; };
	D3DXVECTOR3 GetMin( ) { return vecMin; };
	//
	void SetMinMax(const D3DXVECTOR3 vecMin, const D3DXVECTOR3 vecMax) { this->vecMin = vecMin; this->vecMax = vecMax; }

protected:
	LPD3DXMESH aabbMesh = nullptr;
	D3DMATERIAL9 material;
	BoundingBox boundingBox = BoundingBox(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 1, 1));
	BoundingBox currentBoundingBox = boundingBox;

	D3DXMATRIXA16 world;
	D3DXMATRIXA16* parent = nullptr;	//rotation Á¦¿Ü
	D3DXMATRIXA16 scale, trans;
	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);

	D3DXVECTOR3 vecMax = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 vecMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);

	float baseHeight = 0.0f;
	bool wireMode = false;
};

