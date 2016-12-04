#pragma once

#include "cBaseObject.h"

class cPlayerPlane : public cBaseObject
{
public:
	cPlayerPlane();
	~cPlayerPlane();

	virtual void BaseObjectUpdate(float timeDelta);
};

