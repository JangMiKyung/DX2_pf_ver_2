#include "stdafx.h"
#include "cScene_00_BoundSet.h"

#include "cSetBoundObject.h"
#include "cCamera.h"
#include <fstream>

#include "cLight_Direction.h"
#include "cLight_Point.h"


cScene_00_BoundSet::cScene_00_BoundSet()
	: selectBound(NULL)
{
}


cScene_00_BoundSet::~cScene_00_BoundSet()
{
}

HRESULT cScene_00_BoundSet::Scene_Init()
{
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.01f, 0.01f, 0.01f);
	houseObject = new cBaseObject();
	houseObject->SetMesh(RESOURCE_STATICXMESH->GetResource("../../Resources/Meshes/House2/House2.X", &matScale));
	houseObject->SetActive(true);

	//����Ʈ Ǫ��
	cLight_Direction* pLight1 = new cLight_Direction();
	pLight1->Color = D3DXCOLOR(1, 1, 1, 1);
	pLight1->Intensity = 0.1f;

	cLight_Point* pLight2 = new cLight_Point();
	pLight2->Color = D3DXCOLOR(1, 0, 0, 0);
	pLight2->minRange = 5.0f;
	pLight2->maxRange = 10.0f;
	pLight2->distancePow = 10.0f;

	cLight_Point* pLight3 = new cLight_Point();
	pLight3->Color = D3DXCOLOR(0, 1, 0, 0);
	pLight3->minRange = 5.0f;
	pLight3->maxRange = 10.0f;
	pLight3->distancePow = 10.0f;
	pLight3->pTransform->SetWorldPosition(3, 3, 0);
	pLight3->Intensity = 0.5f;

	this->lights.push_back(pLight1);
	this->lights.push_back(pLight2);
	this->lights.push_back(pLight3);


	return S_OK;
}

void cScene_00_BoundSet::Scene_Release()
{
	for (int i = 0; i < this->lights.size(); i++)
		SAFE_DELETE(this->lights[i]);

	SAFE_DELETE(this->houseObject);

	for (int i = 0; i < this->boundObjects.size(); i++)
	{
		SAFE_DELETE(this->boundObjects[i]);
	}
}

void cScene_00_BoundSet::Scene_Update(float timeDelta)
{
	if (KEY_MGR->IsOnceDown(VK_RETURN))
	{
		cSetBoundObject* pNewBound = new cSetBoundObject();
		pNewBound->SetActive(true);
		pNewBound->Update(timeDelta);
		this->boundObjects.push_back(pNewBound);


		this->selectBound = pNewBound;
	}

	if (this->selectBound != NULL)
	{
		this->selectBound->Update(timeDelta);
	}

	if (KEY_MGR->IsOnceDown(VK_LBUTTON))
	{
		Ray ray;
		POINT ptMouse = GetMousePos();
		D3DXVECTOR2 screenPos(ptMouse.x, ptMouse.y);
		this->pMainCamera->ComputeRay(&ray, &screenPos);


		vector<cSetBoundObject*> hitBounds;
		vector<float>	hitdistances;

		//���� �ִ� ��� ����üũ
		for (int i = 0; i < this->boundObjects.size(); i++)
		{
			D3DXVECTOR3 hitPos;
			if (PHYSICS_MGR->IsRayHitBound(
				&ray,
				&this->boundObjects[i]->BoundBox,
				this->boundObjects[i]->pTransform,
				&hitPos,
				NULL)){

				//�浹�� ���̶��...
				hitBounds.push_back(this->boundObjects[i]);

				//ī�޶�κ����� �Ÿ� ������ Ǫ��
				hitdistances.push_back(D3DXVec3LengthSq(
					&(hitPos - this->pMainCamera->GetWorldPosition())));

			}
		}


		//��Ʈ �� ���� �ִٸ�...
		if (hitBounds.size() > 0)
		{
			//�ϴ� ù��° 
			cSetBoundObject* pTarget = hitBounds[0];
			float nearest = hitdistances[0];

			for (int i = 1; i < hitBounds.size(); i++)
			{
				//����
				if (nearest > hitdistances[i])
				{
					nearest = hitdistances[i];
					pTarget = hitBounds[i];
				}

			}

			this->selectBound = pTarget;
		}
	}


	//���õȰ� ����.....
	if (KEY_MGR->IsOnceDown(VK_BACK))
	{
		if (this->selectBound != NULL)
		{
			//���Ϳ��� ������
			for (vector<cSetBoundObject*>::iterator iter = this->boundObjects.begin();
				iter != this->boundObjects.end();
				++iter)
			{
				if (*iter == this->selectBound)
				{
					this->boundObjects.erase(iter);
					break;
				}
			}

			delete this->selectBound;
			this->selectBound = NULL;
		}
	}

	if (KEY_MGR->IsStayDown(VK_LCONTROL) && KEY_MGR->IsOnceDown('S'))
	{
		this->SaveBoundsInfo();
	}
}

void cScene_00_BoundSet::Scene_Render1()
{
	//���� �׸���.

	//����Ǵ� LightMatrix
	D3DXMATRIXA16 matLights[10];
	for (int i = 0; i < this->lights.size(); i++)
		matLights[i] = this->lights[i]->GetLightMatrix();

	//���̴��� ������ ����
	cXMesh_Static::sStaticMeshEffect->SetMatrixArray("matLights", matLights, 10);
	cXMesh_Static::sStaticMeshEffect->SetInt("LightNum", this->lights.size());

	cXMesh_Static::SetCamera(this->pMainCamera);
	this->houseObject->Render();



	//�ٿ�带 �׸���.
	for (int i = 0; i < this->boundObjects.size(); i++)
		this->boundObjects[i]->Render();

	if (this->selectBound != NULL)
	{
		this->selectBound->pTransform->RenderGimozo();
	}
}

void cScene_00_BoundSet::SaveBoundsInfo()
{
	fstream file;
	file.open("BoundData.txt", fstream::out); //fstream::out ���� ���� ���

	for (int i = 0; i < this->boundObjects.size(); i++)
	{
		cSetBoundObject* pBounds = this->boundObjects[i];

		//��ġ �� ��´�.
		D3DXVECTOR3 pos = pBounds->pTransform->GetWorldPosition();

		//����� ��´�.
		D3DXQUATERNION quat = pBounds->pTransform->GetWorldRotateQuaternion();

		//������ ��´�.
		D3DXVECTOR3 scale = pBounds->pTransform->GetScale();

		//�ٿ�� ����
		D3DXVECTOR3 boundCenter = pBounds->center;
		D3DXVECTOR3 boundHalf = pBounds->halfSize;


		file <<
			"[" << pos.x << "," << pos.y << "," << pos.z << "]" <<  //Pos ����.
			"[" << quat.x << "," << quat.y << "," << quat.z << "," << quat.w << "]" <<		//����� ����
			"[" << scale.x << "," << scale.y << "," << scale.z << "]" <<	//�����Ͼ���.
			"[" << boundCenter.x << "," << boundCenter.y << "," << boundCenter.z << "]" <<	//�ٿ�� ���� ����.
			"[" << boundHalf.x << "," << boundHalf.y << "," << boundHalf.z << "]" <<
			endl;
	}
	file.close(); //�پ����� ��Ʈ���� Ŭ����


	LOG_MGR->AddLog("SaveComplete!!!");
}