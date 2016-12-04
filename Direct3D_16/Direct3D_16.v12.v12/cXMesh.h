#pragma once

class cBoundBox;
class cBaseObject;

class cXMesh
{
public:
	cXMesh();
	~cXMesh();

	virtual HRESULT Init(std::string filePath, const D3DXMATRIXA16* matCorrection = NULL) = 0;
	virtual void Release() = 0;
	virtual void Render(const cTransform* Trans) = 0;
};

