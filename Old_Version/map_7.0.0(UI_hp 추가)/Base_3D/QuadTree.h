#pragma once
class QuadTree
{
	/// ����Ʈ���� �����Ǵ� 4���� �ڳʰ��� ���� �����
	enum CornerType { CORNER_TL, CORNER_TR, CORNER_BL, CORNER_BR, CORNER_NUM };
	enum QuadLocation { FRUSTUM_COMPLETELY_IN, FRUSTUM_PARTIALLY_IN, FRUSTUM_OUT };
	/// �̿� ��� ó���� �����
	enum { EDGE_UP, EDGE_DN, EDGE_LT, EDGE_RT, EDGE_NUM };

private:
	QuadTree* m_pChild[4];		/// ����Ʈ���� 4���� �ڽĳ��
	QuadTree* m_pParent;		/// Triangle Crack(Popping)�� ���� ���ؼ� ����Ѵ�.
	QuadTree* m_pNeighbor[4];	/// Triangle Crack(Popping)�� ���� ���ؼ� ����Ѵ�.

	int m_nCenter;				/// ����Ʈ���� ������ ù ��° ��
	int m_nCorner[4];			/// ����Ʈ���� ������ �ι�° ��
								/// TopLeft(TL)			TopRight(TR)
								///            0---------1
								///            |         | 
								///            |         |
								///            2---------3
								/// BottomLeft(BL)		BottomRight(BR)
	bool m_bCulled;				/// ����ü���� �ø��� ����ΰ�?
	float m_fRadius;			/// ��带 ���δ� ��豸�� ������


private:
	/// �ڽĳ�带 �߰��Ѵ�.
	QuadTree* _AddChild(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);

	/// 4���� �ڳʰ��� �����Ѵ�.
	bool _SetCorners(int nCornerTL, int nCornerTR, int nCornerBL, int nCornerBR);

	/// ����Ʈ���� 4���� ���� Ʈ���� �κк���(subdivide)�Ѵ�.
	bool _SubDivide( );

	///����� �������� �ε����� �����Ѵ�.
	int _GenTriIndex(int nTriangles, LPVOID pIB, D3DXVECTOR3* pHeightMapVertex, float fLODRatio);

	/// ���� ��尡 ����ü�� ���ԵǴ°�?
	int _IsInFrustum(D3DXVECTOR3* pHeightMapVertex);
	
	/// _IsInFrustum() �Լ��� ����� ���� ����ü �ø� ����
	void _FrustumCull(D3DXVECTOR3* pHeightMapVertex);

	/// �� �� ������ �Ÿ��� ���Ѵ�.
	float _GetDistance(D3DXVECTOR3* pv1, D3DXVECTOR3* pv2)
	{
		return D3DXVec3Length(&(*pv2 - *pv1));
	}

	/// ī�޶�� ���� ������ �Ÿ����� �������� LOD ���� ���Ѵ�.
	int  _GetLODLevel(D3DXVECTOR3* pHeightMapVertex, D3DXVECTOR3* pCamera, float fLODRatio)
	{
		float d = _GetDistance((D3DXVECTOR3*)(pHeightMapVertex + m_nCenter), pCamera);
		return max((int)(d * fLODRatio), 1);
	}

	/// ��� �ڽĳ���� m_Culled ���� FALSE�� �Ѵ�.
	void _AllInFrustum( );

	/// �̿���带 �����(�ﰢ�� ������ ������)
	void _BuildNeighborNode(QuadTree* pRoot, D3DXVECTOR3* pHeightMapVertex, int cx);

	/// ����Ʈ���� �����.(Build() �Լ����� �Ҹ���.)
	bool _BuildQuadTree(D3DXVECTOR3* pHeightMapVertex);

	/// ����Ʈ���� �˻��ؼ� 4�� �ڳʰ��� ��ġ�ϴ� ��带 ã�´�.
	QuadTree* _FindNode(D3DXVECTOR3* pHeightMapVertex, int _0, int _1, int _2, int _3);

	/// 4�� ����(���, �ϴ�, ����, ����)�� �̿���� �ε����� ���Ѵ�.
	int _GetNodeIndex(int ed, int cx, int& _0, int& _1, int& _2, int& _3);


public:
	QuadTree( );
	~QuadTree( );

	BOOL IsInRect(RECT * rc, POINT pt);

	/// ������ ��Ʈ��� ������
	QuadTree(int cx, int cy);
	
	/// ���� �ڽĳ�� ������
	QuadTree(QuadTree* pParent);
	
	/// �޸𸮿��� ����Ʈ���� �����Ѵ�.
	void Destroy( );

	/// ����Ʈ���� �����Ѵ�
	bool Build(D3DXVECTOR3* pHeightMapVertex);

	/// �ﰢ���� �ε����� �����, ����� �ﰢ���� ������ ��ȯ�Ѵ�. 
	int GenerateIndex(LPVOID pIB, D3DXVECTOR3* pHeightMapVertex, float fLODRatio);

	/// ���� ��尡 �ø��� ���ΰ�?
	bool IsCulled( ) { return m_bCulled; }
	
	/// ���� ��尡  LOD ������� �� �� ����� �����Ѱ�??
	bool _IsVisible(D3DXVECTOR3* pHeightMapVertex, D3DXVECTOR3* pCamera, float fLODRatio)
	{
		return (m_nCorner[CORNER_TR] - m_nCorner[CORNER_TL] <= _GetLODLevel(pHeightMapVertex, pCamera, fLODRatio));
	}

	/// �ڳ� �ε��� ���� ���´�.
	void GetCorner(int& _0, int& _1, int& _2, int& _3)
	{
		_0 = m_nCorner[0];
		_1 = m_nCorner[1];
		_2 = m_nCorner[2];
		_3 = m_nCorner[3];
	}



};

