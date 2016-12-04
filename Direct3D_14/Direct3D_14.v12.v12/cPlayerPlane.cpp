#include "stdafx.h"
#include "cPlayerPlane.h"


cPlayerPlane::cPlayerPlane()
{
}


cPlayerPlane::~cPlayerPlane()
{
}


void cPlayerPlane::BaseObjectUpdate(float timeDelta)
{
	if (KEY_MGR->IsStayDown(VK_LSHIFT))
	{
		float RotDelta = 90.0f * ONE_RAD * timeDelta;

		if (KEY_MGR->IsStayDown(VK_LEFT))
			this->Transform.RotateWorld(0, -RotDelta, 0);

		if (KEY_MGR->IsStayDown(VK_RIGHT))
			this->Transform.RotateWorld(0, RotDelta, 0);
	}
	else
	{
		float moveDelta = 5.0f * timeDelta;

		if (KEY_MGR->IsStayDown(VK_LEFT))
			this->Transform.MovePositionWorld(-moveDelta, 0, 0);

		if (KEY_MGR->IsStayDown(VK_RIGHT))
			this->Transform.MovePositionWorld(moveDelta, 0, 0);


	}
}