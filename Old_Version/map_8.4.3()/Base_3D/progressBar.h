#pragma once

class progressBar
{
private:
	D3DXMATRIXA16 world;
	D3DXMATRIXA16 matParent;
	D3DXMATRIXA16 matIdentity;

	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0); // 위치
	D3DXVECTOR2 size = D3DXVECTOR2(0, 0);		 // 크기

	LPDIRECT3DTEXTURE9 progressBarBack = nullptr;	
	LPDIRECT3DTEXTURE9 progressBarFront = nullptr;

	float fGauge_Width = 0.f;
	float fGauge_Height = 0.f;

	bool bWidth = false;
//private:
public:
	progressBar( );
	~progressBar( );

	//static progressBar* Get( )
	//{
	//	static progressBar inst;
	//	return &inst;
	//}

	HRESULT init(char * progressBarBack, char * progressBarFront, D3DXVECTOR3& pos);
	void Destroy( );
	void Update( );
	void Render( );


	//체력바 게이지
	void Gauge_Width(float& currentGauge, float maxGauge);
	void Gauge_Height(float& currentGauge, float maxGauge);

	void SetTexture(char * progressBarBack, char * progressBarFront);
	inline void SetParent(D3DXMATRIXA16& myParent) { matParent = myParent; }
	inline const D3DXMATRIX& GetWorld( ) { return world; }

};

