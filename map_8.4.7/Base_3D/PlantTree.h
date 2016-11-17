#pragma once
#include "HeightMap.h"
#include "Tree.h"

class PlantTree
{
public:
	PlantTree( );
	~PlantTree( );

	void Initialize( );
	void Destroy( );
	void Update( );
	void Render( );

	std::vector<Tree*> GetTrees( ) { return trees; }
	inline void SetHeightMap(HeightMap* inputMap) { hm = inputMap; }

	inline void SetWireMode(bool wireMode) { this->wireMode = wireMode; }
	inline void SetIsColliderBoxDraw(bool IsColliderBoxDraw) { this->IsColliderBoxDraw = IsColliderBoxDraw; }
	inline void SetIsColliderSphereDraw(bool isColliderSphereDraw) { this->isColliderSphereDraw = isColliderSphereDraw; }

private:
	std::vector<D3DXVECTOR3> vRawPosition;
	std::vector<Tree*> trees;

	HeightMap* hm = nullptr;

	bool wireMode = false;
	bool IsColliderBoxDraw = false;
	bool isColliderSphereDraw = false;
};

