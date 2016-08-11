#pragma once
#include <vector>
#include <map>
#include "FVF.h"
#include "MaterialTexture.h"
#include "ObjFileGroup.h"


class ObjLoader
{
public:
	ObjLoader();
	~ObjLoader();

	static void LoadObjFileData(OUT std::vector<ObjFileGroup*>& group, const char* fileName, OUT std::vector<D3DXVECTOR3> &positionList, bool isblend = false);

	static void LoadObj_OnlyVertex(OUT std::vector<D3DXVECTOR3>& vertex, const char* fileName);

private:
	static const char* OBJ_DIRECTORY;
	static void NewMtl(FILE * fp, MaterialTexture * newMT);
	static void LoadMtlFileData(OUT std::map<std::string, MaterialTexture*>& materialTextures, const char* fileName);
	//
	//std::map<int, std::vector<D3DXVECTOR3>> positionList;
	//int count_ListNum = 0;
};

