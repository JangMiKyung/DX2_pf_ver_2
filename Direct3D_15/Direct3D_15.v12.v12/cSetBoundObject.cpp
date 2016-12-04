#include "stdafx.h"
#include "cSetBoundObject.h"
#include "cTransform.h"

cSetBoundObject::cSetBoundObject()
{
	this->center = D3DXVECTOR3(0, 0, 0);
	this->halfSize = D3DXVECTOR3(1, 1, 1);
}


cSetBoundObject::~cSetBoundObject()
{
}

void cSetBoundObject::BaseObjectUpdate(float timeDelta)
{
	this->pTransform->DefaultControl2(timeDelta);

	if (KEY_MGR->IsStayDown('T'))
	{
		this->halfSize.z += timeDelta;
	}

	else if (KEY_MGR->IsStayDown('G'))
	{
		this->halfSize.z -= timeDelta;
		if (this->halfSize.z < 0.f)
			this->halfSize.z = 0.f;
	}

	if (KEY_MGR->IsStayDown('F'))
	{
		this->halfSize.x += timeDelta;
	}

	else if (KEY_MGR->IsStayDown('H'))
	{
		this->halfSize.x -= timeDelta;
		if (this->halfSize.x < 0.0f)
			this->halfSize.x = 0.0f;
	}

	if (KEY_MGR->IsStayDown('U'))
	{
		this->halfSize.y += timeDelta;
	}

	else if (KEY_MGR->IsStayDown('J'))
	{
		this->halfSize.y -= timeDelta;
		if (this->halfSize.y < 0.0f)
			this->halfSize.y = 0.0f;
	}

	this->BoundBox.SetBound(&this->center, &this->halfSize);
}