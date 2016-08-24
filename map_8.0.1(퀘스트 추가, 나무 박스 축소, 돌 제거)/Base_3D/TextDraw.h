#pragma once
class TextDraw
{
public:
	TextDraw( );
	~TextDraw( );

	void init( );
	void Destroy( );
	void Update( );
	void Render( );

	inline void SetText(std::string _text) { text = _text; }	
	inline void SetTextColor(D3DCOLOR color) { textColor = color; }
	inline void SetDrawSize(D3DXVECTOR2 size) { this->size = size; }
	inline void SetPosition(D3DXVECTOR3& pos) { position = pos; }
	HRESULT SetPositionCenter(D3DXVECTOR3& pos)
	{
		int nlen = text.length( );
		if (nlen < 1) return E_FAIL; 
		if (size.x < 1 || size.y < 1) return E_FAIL;
		position = D3DXVECTOR3(size.x * 0.5 - nlen * 0.5 * 10, size.y * 0.5 - 10, 0);
		
		return S_OK;
	}
	inline void SetTextFormat(DWORD format) { textFormat = format; }
	inline void SetFont(LPD3DXFONT _font) { font = _font; }

	inline void SetParent(D3DXMATRIXA16& myParent) { matParent = myParent; }

	inline D3DXMATRIXA16& GetWorld( ) { return world; }


private:
	std::string text;
	DWORD textFormat = (DT_TOP | DT_LEFT);
	D3DCOLOR textColor = D3DCOLOR_XRGB(255, 255, 255);

	LPD3DXFONT font = nullptr;

	D3DXMATRIXA16 world;
	D3DXMATRIXA16 matParent;
	D3DXMATRIXA16 matIdentity;

	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0); 
	D3DXVECTOR2 size = D3DXVECTOR2(200, 25);		 

};

