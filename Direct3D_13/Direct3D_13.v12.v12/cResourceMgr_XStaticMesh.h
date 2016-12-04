#pragma once

#include "cResourceMgr.h"
#include "cXMesh_Static.h"

class cResourceMgr_XStaticMesh : public cResourceMgr<cXMesh_Static*,
	cResourceMgr_XStaticMesh>
{
public:
	cResourceMgr_XStaticMesh();
	~cResourceMgr_XStaticMesh();

	virtual cXMesh_Static* LoadResource(std::string filePath, void* pParam = NULL);

	virtual void ReleaseResource(cXMesh_Static* data);
};

#define RESOURCE_STATICXMESH cResourceMgr_XStaticMesh::GetInstance()