#pragma once

enum HEIGHT_FLAG
{
	HM_DEFAULT = 0,
	HM_ALPHA,
	HM_NORMAL,
	HM_NUMBER
};
class HeightMap
{
public:

public:
	//로우 파일 구분을 위한 추상클래스
	class RawFileSection
	{
	public:
		RawFileSection( ) {}
		virtual ~RawFileSection( ) {}
		virtual void Initialize(OUT std::vector<D3DXVECTOR3> &vertex, OUT int& mapSize, char* mapFileName, char* textureFileName, int bitNum = 8) = 0;
		virtual void Destroy( ) = 0;
		virtual void Render( ) = 0;
	};
public:
	HeightMap( );
	~HeightMap( );

	void Initialize(char* mapFileName, char* textureFileName, int bitNum = 8, HEIGHT_FLAG hf = HEIGHT_FLAG::HM_DEFAULT);
	void Destroy( );
	void Render( );

	bool GetHeight(float& outHeight, float x, float z);
	bool GetHeight(D3DXVECTOR3 & pos, float x, float z);
	bool OnTheGround(D3DXVECTOR3 & pos, const D3DXVECTOR3 & p0, const D3DXVECTOR3 & p1, const D3DXVECTOR3 & p2);

	std::vector<D3DXVECTOR3>* GetVertex( ) { return &vertex; }

protected:
	std::vector<D3DXVECTOR3> vertex; //
	RawFileSection* rfsection = nullptr;
	int mapSize = -1; //raw파일의 픽셀에 한변의 길이
};

