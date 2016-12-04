#include "stdafx.h"
#include "cResourceMgr_XStaticMesh.h"


cResourceMgr_XStaticMesh::cResourceMgr_XStaticMesh()
{
}


cResourceMgr_XStaticMesh::~cResourceMgr_XStaticMesh()
{
}

cXMesh_Static* cResourceMgr_XStaticMesh::LoadResource(std::string filePath, void* pParam /*= NULL */)
{
	cXMesh_Static* pNewMesh = new cXMesh_Static();
	if (FAILED(pNewMesh->Init(filePath, (D3DXMATRIXA16*)pParam)))
	{
		SAFE_DELETE(pNewMesh);
		return NULL;
	}

	return pNewMesh;
}

void cResourceMgr_XStaticMesh::ReleaseResource(cXMesh_Static* data)
{
	data->Release();
	SAFE_DELETE(data);
}