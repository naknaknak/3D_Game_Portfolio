#pragma once
class UI_Base
{
public:
	UI_Base();
	virtual ~UI_Base();
		
	virtual void Destroy();

	virtual void Update();
	virtual void Render();

	virtual void AddChild(UI_Base* child);
	virtual LRESULT InputProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual UI_Base* GetChildByID(int childID);

	inline const D3DXMATRIX& GetWorld(){ return world; }
	inline int GetID(){ return id; }
	inline D3DXVECTOR2& GetSize(){ return size; }
			
	inline void SetPosition(float x, float y, float z){ position = D3DXVECTOR3(x,y,z); }
	inline void SetSize(float width, float height){ size.x = width; size.y = height; }
	inline void SetID(int _ID){ id = _ID; }
	inline void SetShow(bool show = true){ isShow = show; }
	
protected:

	UI_Base* parent = nullptr;
	std::vector<UI_Base*> childArray;
	
	D3DXMATRIX world;				

	D3DXVECTOR3 position = D3DXVECTOR3(0, 0, 0); // 위치
	D3DXVECTOR2 size = D3DXVECTOR2(0, 0);		 // 크기

	int id = -1;	//UI를 구분하기 위한 유니크 아이디(-1이면 세팅되지 않은것)
	
	bool isShow = true;

	inline void SetParent(UI_Base* myParent){ parent = myParent; }
};

