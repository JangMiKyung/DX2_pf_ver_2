#pragma once
#include "cTransform.h"

class cTransform;

class cNode
{

public:
	cTransform pos;
	cNode* Next;
	cNode* Pre;

public :
	cNode(D3DXVECTOR3 vec)
	{

		pos.SetWorldPosition(vec.x, vec.y, vec.z);
		//pos.SetLocalPosition(vec.x, vec.y, vec.z);
	}
	void SetNextRotate(D3DXVECTOR3 vec)
	{
		pos.LookPosition(vec * - 1, D3DXVECTOR3(0,1,0));
	}


};

