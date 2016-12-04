#include "stdafx.h"
#include "cResourceMgr_XSkinnedMesh.h"


cResourceMgr_XSkinnedMesh::cResourceMgr_XSkinnedMesh()
{
}


cResourceMgr_XSkinnedMesh::~cResourceMgr_XSkinnedMesh()
{
}

cXMesh_Skinned* cResourceMgr_XSkinnedMesh::LoadResource(std::string filePath, void* pParam /*= NULL */)
{
	cXMesh_Skinned* pNewMesh = new cXMesh_Skinned();
	if (FAILED(pNewMesh->Init(filePath, (D3DXMATRIXA16*)pParam)))
	{
		SAFE_DELETE(pNewMesh);
		return NULL;
	}

	return pNewMesh;
}

void cResourceMgr_XSkinnedMesh::ReleaseResource(cXMesh_Skinned* data)
{
	data->Release();
	SAFE_DELETE(data);
}
