#pragma once
#include "ObjFileGroup.h"


class Obj
{
public:
	Obj( );
	~Obj( );

	void Initialize(const  char* mapFileName, D3DXVECTOR3& pos, const  char* groundFileName = nullptr, bool isblender = false);
	void Destroy( );
	void Update( );
	void Render( );

	bool GroundCheck(IN OUT D3DXVECTOR3& groundPos) const;

	inline std::vector<D3DXVECTOR3>& GetVertexList( ) { return vertexList; }
	inline void SetPosition(D3DXVECTOR3& pos) { position = pos; }

private:

	D3DXMATRIXA16 world;
	D3DXMATRIXA16 rotation;

	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0);

	std::vector<ObjFileGroup*> objGroups;
	std::vector<D3DXVECTOR3> objGround;
	std::vector<D3DXVECTOR3> vertexList;
};

