#include "stdafx.h"
#include "cBaseObject.h"


cBaseObject::cBaseObject()
	: bActive( false )
{
}


cBaseObject::~cBaseObject()
{
}


//활성화 셋팅
void cBaseObject::SetActive(bool bActive)
{
	//활성화 되어있는 상태에서 비활성화 셋팅시
	if (bActive == false && this->bActive)
	{
		this->bActive = false;
		this->BaseObjectDisable();
	}

	//비활성화 되어있는 상태에서 활성화 셋팅시
	else if (bActive && this->bActive == false)
	{
		this->bActive = true;
		this->BaseObjectEnable();
	}
}

//셋팅된 메쉬에 따라 바운드박스를 재계산 한다.
void cBaseObject::ComputeBoundBox()
{
	if (this->pMesh != NULL)
	{
		//셋팅된 메쉬가 cXMesh_Static 라면...
		cXMesh_Static* pStaticMesh = dynamic_cast< cXMesh_Static* >(this->pMesh);
		if (pStaticMesh != NULL)
		{
			this->BoundBox.localCenter = pStaticMesh->Bound_Center;
			this->BoundBox.halfSize = pStaticMesh->Bound_HalfSize;
			this->BoundBox.localMinPos = pStaticMesh->Bound_Min;
			this->BoundBox.localMaxPos = pStaticMesh->Bound_Max;
			this->BoundBox.radius = pStaticMesh->Bound_Radius;
		}
	}
}

void cBaseObject::BaseObjectRender()
{
	this->pMesh->Render(this->Transform);

	this->BoundBox.RenderGizmo(this->Transform);
}