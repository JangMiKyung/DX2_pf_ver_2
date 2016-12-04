#pragma once

#include "cBaseObject.h"

class cSetBoundObject : public cBaseObject
{
public:
	D3DXVECTOR3 center;
	D3DXVECTOR3 halfSize;

public:
	cSetBoundObject();
	~cSetBoundObject();

	virtual void BaseObjectUpdate(float timeDelta) override;
};

