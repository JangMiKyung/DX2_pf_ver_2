#pragma once

#include "cBaseObject.h"

class cEnemyPlane : public cBaseObject
{
private:
	float nextGenTime;
	float speed;

public:
	cEnemyPlane();
	~cEnemyPlane();

	virtual void BaseObjectEnable();
	virtual void BaseObjectUpdate(float timeDelta);
	virtual void BaseObjectNoActiveUpdate(float timeDelta);

};

