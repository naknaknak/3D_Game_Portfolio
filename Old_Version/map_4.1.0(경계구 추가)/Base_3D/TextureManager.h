#pragma once

class TextureManager
{
private:
	TextureManager();
	TextureManager(const TextureManager& other);
	~TextureManager();

public :
	static TextureManager& Get()
	{
		static TextureManager instance;
		return instance;
	}

	void Destroy();

	static inline LPDIRECT3DTEXTURE9 GetTexture(const char* filename, D3DXIMAGE_INFO* imageInfo = nullptr){ return TextureManager::Get().GetInstanceTexture(filename, imageInfo); }
	static inline LPDIRECT3DTEXTURE9 GetTexture(const char* path, const char* fileName) { return TextureManager::Get().GetInstanceTexture(path, fileName); }
public:
	const char* TEXTURE_DIRECTORY = "./Data/Texture/";
	const char* TEXTURE_DIRECTORY1 = "./Data/Texture/terrain_alpine/";
	const char* TEXTURE_DIRECTORY2 = "./Data/Texture/field_tiles/";
	const char* TEXTURE_DIRECTORY3 = "./Data/Texture/X_file_texture/";
	const char* TEXTURE_DIRECTORY4 = "./Data/Texture/filed_texture/tree/";
	const char* TEXTURE_DIRECTORY5 = "./Data/Texture/filed_texture/rock/";
	//const char* TEXTURE_DIRECTORY = "./Data/Texture/";v
	//std::map<std::string, LPDIRECT3DTEXTURE9> textureMap;
	std::map<std::string, std::pair<LPDIRECT3DTEXTURE9, D3DXIMAGE_INFO>> textureDataMap;
	LPDIRECT3DTEXTURE9 GetInstanceTexture(
		const char* filename, D3DXIMAGE_INFO* imageInfo = nullptr);
	LPDIRECT3DTEXTURE9 GetInstanceTexture(const char* path, const char* fileName);
};

