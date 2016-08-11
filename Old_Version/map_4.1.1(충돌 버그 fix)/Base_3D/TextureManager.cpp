#include "stdafx.h"
#include "TextureManager.h"


TextureManager::TextureManager( )
{
}


TextureManager::~TextureManager( )
{
}

LPDIRECT3DTEXTURE9 TextureManager::GetInstanceTexture(const char* filename, D3DXIMAGE_INFO* imageInfo /*= nullptr*/)
{
	auto iter = textureDataMap.find(filename);
	if (textureDataMap.find(filename) == textureDataMap.end( ))
	{
		bool isFaile = true;
		while (isFaile)
		{
			static char fullPath[256];
			static int count = 0;
			static const char* str;

			switch (count++)
			{
				case 0:
					str = TEXTURE_DIRECTORY;
					break;
				case 1:
					str = TEXTURE_DIRECTORY1;
					break;
				case 2:
					str = TEXTURE_DIRECTORY2;
					break;
				case 3:
					str = TEXTURE_DIRECTORY3;
					break;
				case 4:
					str = TEXTURE_DIRECTORY4;
					break;
				case 5:
					str = TEXTURE_DIRECTORY5;
					break;
				default:
					isFaile = false;
					count = 0;
					break;
			}
			strcpy_s(fullPath, str);
			strcat_s(fullPath, filename);

			if (SUCCEEDED(D3DXCreateTextureFromFileExA(
				GameManager::GetDevice( ),
				fullPath,
				D3DX_DEFAULT_NONPOW2,
				D3DX_DEFAULT_NONPOW2,
				D3DX_DEFAULT,
				0,
				D3DFMT_UNKNOWN,
				D3DPOOL_MANAGED,
				D3DX_FILTER_NONE,
				D3DX_DEFAULT,
				0,
				&textureDataMap[filename].second,
				nullptr,
				&textureDataMap[filename].first)))
			{
				isFaile = false;
				count = 0;
			}
		}
	}
	if (imageInfo)
	{
		*imageInfo = textureDataMap[filename].second;
	}
	return textureDataMap[filename].first;
}
LPDIRECT3DTEXTURE9 TextureManager::GetInstanceTexture(const char* path, const char* fileName)
{
	if (textureDataMap.find(fileName) == textureDataMap.end( ))
	{
		char fullPath[512];
		strcpy_s(fullPath, path);
		strcat_s(fullPath, fileName);
		LPDIRECT3DTEXTURE9 tex_first;
		D3DXIMAGE_INFO imgInfo_second;
		if (FAILED(D3DXCreateTextureFromFileExA(
			GameManager::GetDevice( ),
			fullPath,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_DEFAULT,
			0,
			&imgInfo_second,
			nullptr,
			&tex_first)))
		{
			return nullptr;
		}
		textureDataMap[fileName].first = tex_first;
		textureDataMap[fileName].second;
	}
	return textureDataMap[fileName].first;
}
//LPDIRECT3DTEXTURE9 TextureManager::GetInstanceTexture(const char* filename, D3DXIMAGE_INFO* imageInfo /*= nullptr*/)
//{
//	if (textureDataMap.find(filename) == textureDataMap.end())
//	{
//		char fullPath[256];
//		strcpy_s(fullPath, TEXTURE_DIRECTORY);
//		strcat_s(fullPath, filename);
//
//		D3DXCreateTextureFromFileExA(
//			GameManager::GetDevice(), 
//			fullPath, 
//			D3DX_DEFAULT_NONPOW2,
//			D3DX_DEFAULT_NONPOW2,
//			D3DX_DEFAULT,
//			0,
//			D3DFMT_UNKNOWN,
//			D3DPOOL_MANAGED,
//			D3DX_FILTER_NONE,
//			D3DX_DEFAULT,
//			0,
//			&textureDataMap[filename].second,
//			nullptr,
//			&textureDataMap[filename].first);
//	}
//	if (imageInfo)
//	{
//		*imageInfo = textureDataMap[filename].second;
//	}
//	return textureDataMap[filename].first;
//}

void TextureManager::Destroy( )
{
	for (auto iter = textureDataMap.begin( ); iter != textureDataMap.end( ); ++iter)
	{
		SAFE_RELEASE(iter->second.first);
	}
	textureDataMap.clear( );
}