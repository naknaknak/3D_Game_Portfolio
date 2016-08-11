#pragma once
#include "BoundingVolume.h"

class Collider_Box
{
public:
	Collider_Box( );
	~Collider_Box( );

	void Initialize(std::vector<D3DXVECTOR3>& pos, const  D3DXMATRIXA16 * matScale = nullptr, const  D3DXMATRIXA16 * matRot = nullptr, const D3DXMATRIXA16 * matTrans = nullptr);
	void MinMaxVertices(std::vector<D3DXVECTOR3>& pos, const D3DXMATRIXA16* matRot = nullptr);
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

protected:
	BoundingBox boundingBox = BoundingBox(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 1, 1));
	BoundingBox currentBoundingBox = boundingBox;
	LPD3DXMESH aabbMesh = nullptr;

	float baseHeight = 0.0f;

	D3DXMATRIXA16 world;
	D3DXMATRIXA16* parent = nullptr;	//rotation Á¦¿Ü

	bool wireMode = false;

	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);

	D3DMATERIAL9 material;
	//
	D3DXVECTOR3 vecMax = D3DXVECTOR3(0, 0, 0);
	D3DXVECTOR3 vecMin = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	D3DXMATRIXA16 scale, trans;
};

