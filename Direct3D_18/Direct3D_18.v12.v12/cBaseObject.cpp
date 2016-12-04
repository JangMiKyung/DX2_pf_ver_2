#include "stdafx.h"
#include "cBaseObject.h"
#include "cTransform.h"

cBaseObject::cBaseObject()
	: bActive( false )
{
	this->pTransform = new cTransform;
}


cBaseObject::~cBaseObject()
{
	SAFE_DELETE(pTransform);
}


//Ȱ��ȭ ����
void cBaseObject::SetActive(bool bActive)
{
	//Ȱ��ȭ �Ǿ��ִ� ���¿��� ��Ȱ��ȭ ���ý�
	if (bActive == false && this->bActive)
	{
		this->bActive = false;
		this->BaseObjectDisable();
	}

	//��Ȱ��ȭ �Ǿ��ִ� ���¿��� Ȱ��ȭ ���ý�
	else if (bActive && this->bActive == false)
	{
		this->bActive = true;
		this->BaseObjectEnable();
	}
}

//���õ� �޽��� ���� �ٿ��ڽ��� ���� �Ѵ�.
void cBaseObject::ComputeBoundBox()
{
	if (this->pMesh != NULL)
	{
		//���õ� �޽��� cXMesh_Static ���...
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
	if (this->pMesh != NULL)
		this->pMesh->Render(this->pTransform);

//	this->BoundBox.RenderGizmo(this->pTransform);
}