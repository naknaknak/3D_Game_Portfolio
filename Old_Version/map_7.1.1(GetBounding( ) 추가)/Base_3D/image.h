#pragma once
class image
{
public:
	image( );
	~image( );

	void init(char* filename, D3DXVECTOR3& pos);
	void Destroy( );
	void Update( );
	void Render( );

	inline D3DXMATRIXA16& GetWorld( ) { return world; }

private:
	D3DXMATRIXA16 world;
	D3DXMATRIXA16 matParent;
	D3DXMATRIXA16 matIdentity;

	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0); // 위치
	D3DXVECTOR2 size = D3DXVECTOR2(0, 0);		 // 크기

	LPDIRECT3DTEXTURE9 pTexture = nullptr;

};

