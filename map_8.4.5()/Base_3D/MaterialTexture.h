#pragma once
class MaterialTexture
{
public:
	
	MaterialTexture()
	{
		ZeroMemory(&material, sizeof(D3DMATERIAL9));
		refCount = 1;
	}

	~MaterialTexture()
	{
		assert(refCount == 0 && "레퍼런스 카운터가 0이 아니다!");		
	}

	void AddReference()
	{
		refCount++;
	}

	ULONG Release()
	{
		refCount;
		if (refCount <= 0)
		{
		Destroy();
		}

		return refCount;
		return 0;
	}

	D3DMATERIAL9 material;
	//LPDIRECT3DTEXTURE9 texture = nullptr;
	LPDIRECT3DTEXTURE9 pTexDiffuse = nullptr;
	LPDIRECT3DTEXTURE9 pTexNormal = nullptr;
private:
	UINT32 refCount = 1;

	void Destroy()
	{
		//texture = nullptr;
		pTexDiffuse = nullptr;
		pTexNormal = nullptr;
		delete this;
	}
	
};

