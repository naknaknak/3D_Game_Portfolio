#pragma once
#include "SkinnedMesh.h"

// X���Ͽ��� �ҷ��� ���� ������ ������ ���� �޴���
class SkinnedMeshManager
{
private:
	SkinnedMeshManager();
	SkinnedMeshManager(const SkinnedMeshManager& other);
	~SkinnedMeshManager();

public:
	static SkinnedMeshManager& Get()
	{
		static SkinnedMeshManager instance;
		return instance;
	}

	void Destroy();

	SkinnedMesh* GetOriginal(char* path, char* fileName);

private:
	const char* XFILE_DIRECTORY = "./Data/X_file/";

	std::map<std::string, SkinnedMesh*> orignalSkinnedMeshMap;
};

