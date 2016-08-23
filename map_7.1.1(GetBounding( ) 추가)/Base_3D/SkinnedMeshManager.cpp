#include "stdafx.h"
#include "SkinnedMeshManager.h"


SkinnedMeshManager::SkinnedMeshManager()
{
}


SkinnedMeshManager::~SkinnedMeshManager()
{
}

void SkinnedMeshManager::Destroy()
{
	for ( auto iter = orignalSkinnedMeshMap.begin(); iter != orignalSkinnedMeshMap.end(); ++iter )
	{
		SAFE_DELETE(iter->second);
	}
	orignalSkinnedMeshMap.clear();
}

SkinnedMesh* SkinnedMeshManager::GetOriginal(char* path, char* fileName)
{
	char directory[256];
	strcpy_s(directory, XFILE_DIRECTORY);
	strcat_s(directory, path);

	if ( orignalSkinnedMeshMap.find(fileName) == orignalSkinnedMeshMap.end() )
	{
		SkinnedMesh* skinnedMesh = new SkinnedMesh();
		skinnedMesh->Load(directory, fileName);
		orignalSkinnedMeshMap[fileName] = skinnedMesh;
	}
	return orignalSkinnedMeshMap[fileName];
}
