#pragma once
class QuadTree
{
	/// 쿼드트리에 보관되는 4개의 코너값에 대한 상수값
	enum CornerType { CORNER_TL, CORNER_TR, CORNER_BL, CORNER_BR, CORNER_NUM };
	enum QuadLocation { FRUSTUM_COMPLETELY_IN, FRUSTUM_PARTIALLY_IN, FRUSTUM_OUT };
	/// 이웃 노드 처리용 상수값
	enum { EDGE_UP, EDGE_DN, EDGE_LT, EDGE_RT, EDGE_NUM };

private:
	QuadTree* m_pChild[4];		/// 쿼드트리의 4개의 자식노드
	QuadTree* m_pParent;		/// Triangle Crack(Popping)을 막기 위해서 사용한다.
	QuadTree* m_pNeighbor[4];	/// Triangle Crack(Popping)을 막기 위해서 사용한다.

	int m_nCenter;				/// 쿼드트리에 보관할 첫 번째 값
	int m_nCorner[4];			/// 쿼드트리에 보관할 두번째 값
								/// TopLeft(TL)			TopRight(TR)
								///            0---------1
								///            |         | 
								///            |         |
								///            2---------3
								/// BottomLeft(BL)		BottomRight(BR)
	bool m_bCulled;				/// 절두체에서 컬링된 노드인가?
	float m_fRadius;			/// 노드를 감싸는 경계구의 반지름


private:
	/// 자식노드를 추가한다.
	QuadTree* _AddChild(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);

	/// 4개의 코너값을 셋팅한다.
	bool _SetCorners(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);

	/// 쿼드트리를 4개의 하위 트리로 부분분할(subdivide)한다.
	bool _SubDivide( );

	///출력할 폴리곤의 인덱스를 생성한다.
	int _GenTriIndex(int nTriangles, LPVOID pIB, D3DXVECTOR3* pHeightMapVertex, float fLODRatio);

	/// 현재 노드가 절두체에 포함되는가?
	int _IsInFrustum(D3DXVECTOR3* pHeightMapVertex);
	
	/// _IsInFrustum() 함수의 결과에 따라 절두체 컬링 수행
	void _FrustumCull(D3DXVECTOR3* pHeightMapVertex);

	/// 두 점 사이의 거리를 구한다.
	float _GetDistance(D3DXVECTOR3* pv1, D3DXVECTOR3* pv2)
	{
		return D3DXVec3Length(&(*pv2 - *pv1));
	}

	/// 카메라와 현재 노드와의 거리값을 기준으로 LOD 값을 구한다.
	int  _GetLODLevel(D3DXVECTOR3* pHeightMapVertex, D3DXVECTOR3* pCamera, float fLODRatio)
	{
		float d = _GetDistance((D3DXVECTOR3*)(pHeightMapVertex + m_nCenter), pCamera);
		return max((int)(d * fLODRatio), 1);
	}

	/// 모든 자식노드의 m_Culled 값을 FALSE로 한다.
	void _AllInFrustum( );

	/// 이웃노드를 만든다(삼각형 찢어짐 방지용)
	void _BuildNeighborNode(QuadTree* pRoot, D3DXVECTOR3* pHeightMapVertex, int cx);

	/// 쿼드트리를 만든다.(Build() 함수에서 불린다.)
	bool _BuildQuadTree(D3DXVECTOR3* pHeightMapVertex);

	/// 쿼드트리를 검색해서 4개 코너값과 일치하는 노드를 찾는다.
	QuadTree* _FindNode(D3DXVECTOR3* pHeightMapVertex, int _0, int _1, int _2, int _3);

	/// 4개 방향(상단, 하단, 좌측, 우측)의 이웃노드 인덱스를 구한다.
	int _GetNodeIndex(int ed, int cx, int& _0, int& _1, int& _2, int& _3);


public:
	QuadTree( );
	~QuadTree( );

	BOOL IsInRect(RECT * rc, POINT pt);

	/// 최초의 루트노드 생성자
	QuadTree(int cx, int cy);
	
	/// 하위 자식노드 생성자
	QuadTree(QuadTree* pParent);
	
	/// 메모리에서 쿼드트리를 삭제한다.
	void Destroy( );

	/// 쿼드트리를 구축한다
	bool Build(D3DXVECTOR3* pHeightMapVertex);

	/// 삼각형의 인덱스를 만들고, 출력할 삼각형의 개수를 반환한다. 
	int GenerateIndex(LPVOID pIB, D3DXVECTOR3* pHeightMapVertex, float fLODRatio);

	/// 현재 노드가 컬링된 것인가?
	bool IsCulled( ) { return m_bCulled; }
	
	/// 현재 노드가  LOD 등급으로 볼 때 출력이 가능한가??
	bool _IsVisible(D3DXVECTOR3* pHeightMapVertex, D3DXVECTOR3* pCamera, float fLODRatio)
	{
		return (m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL] <= _GetLODLevel(pHeightMapVertex, pCamera, fLODRatio));
	}

	/// 코너 인덱스 값을 얻어온다.
	void GetCorner(int& _0, int& _1, int& _2, int& _3)
	{
		_0 = m_nCorner[0];
		_1 = m_nCorner[1];
		_2 = m_nCorner[2];
		_3 = m_nCorner[3];
	}



};

