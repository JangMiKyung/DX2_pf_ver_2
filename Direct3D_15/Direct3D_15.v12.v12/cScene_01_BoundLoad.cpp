#include "stdafx.h"
#include "cScene_01_BoundLoad.h"
#include "cSetBoundObject.h"
#include "cCamera.h"
#include <fstream>

#include "cLight_Direction.h"
#include "cLight_Point.h"

cScene_01_BoundLoad::cScene_01_BoundLoad()
{
}


cScene_01_BoundLoad::~cScene_01_BoundLoad()
{
}

HRESULT cScene_01_BoundLoad::Scene_Init()
{
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.01f, 0.01f, 0.01f);
	houseObject = new cBaseObject();
	houseObject->SetMesh(RESOURCE_STATICXMESH->GetResource("../../Resources/Meshes/House2/House2.X", &matScale));
	houseObject->SetActive(true);

	LoadBound();

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

	this->pTrans = new cTransform();

	return S_OK;
}

void cScene_01_BoundLoad::Scene_Release()
{
	for (int i = 0; i < this->lights.size(); i++)
		SAFE_DELETE(this->lights[i]);

	SAFE_DELETE(this->houseObject);

	for (int i = 0; i < this->boundObjects.size(); i++)
	{
		SAFE_DELETE(this->boundObjects[i]);
	}

	SAFE_DELETE(this->pTrans);
}

void cScene_01_BoundLoad::Scene_Update(float timeDelta)
{
	this->pTrans->DefaultControl2(timeDelta);
}

void cScene_01_BoundLoad::Scene_Render1()
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
	this->pTrans->RenderGimozo();

	D3DXVECTOR3 worldPos = this->pTrans->GetWorldPosition();

	D3DXVECTOR2 screenPos;
	if (this->pMainCamera->GetWorldPosToScreenPos(&screenPos, &worldPos))
	{
		DXFONT_MGR->PrintTextOutline("����", screenPos.x, screenPos.y, 0xff00ffff, 0xffff00ff);
	}
	
}

void cScene_01_BoundLoad::LoadBound()
{
	fstream file;
	file.open("BoundData.txt", fstream::in); //fstream::in -> ���� �б� ���

	std::vector<std::string> strLine;

	//���� ������ �д´�.
	while (file.eof() == false) //���� ���� ������ trur �� ��ȯ
	{
		std::string line;
		file >> line;			//file ���� ���ڿ��� �о� line�� ����
		strLine.push_back(line);
	}
	file.close(); //�پ� ���� ��Ʈ�� �ݴ´�.

	//�о�� ���� ��� ����
	for (int i = 0; i < strLine.size(); i++)
	{
		if (strLine[i].size() == 0)
			continue;

		//���� ������ ���⿡ ���Եȴ�.
		char cStr[2048];
		strcpy(cStr, strLine[i].c_str());

		char* pc;

		//[3.6819,2.71903,13.0518]
		//[-0.621861,-0.277318,-0.272702,0.679719]
		//[1,1,1][0,0,0][7.13382,5.48357,6.06699]

		//��ġ
		D3DXVECTOR3 pos;
		pc = strtok(cStr, "[],"); //ó��¥������ ¥�� ���ڿ��� �ּҸ� �ִ´�.
		pos.x = atof(pc);
		pc = strtok(NULL, "[],"); //�� �������ʹ� NULL(©���� ���ڿ� ��ġ)
		pos.y = atof(pc);
		pc = strtok(NULL, "[],");
		pos.z = atof(pc);


		//�����
		D3DXQUATERNION quat;
		pc = strtok(NULL, "[],");
		quat.x = atof(pc);
		pc = strtok(NULL, "[],");
		quat.y = atof(pc);
		pc = strtok(NULL, "[],");
		quat.z = atof(pc);
		pc = strtok(NULL, "[],");
		quat.w = atof(pc);

		//������
		D3DXVECTOR3 scale;
		pc = strtok(NULL, "[],");
		scale.x = atof(pc);
		pc = strtok(NULL, "[],");
		scale.y = atof(pc);
		pc = strtok(NULL, "[],");
		scale.z = atof(pc);

		//�ٿ�� ���ü���
		D3DXVECTOR3 localCenter;
		pc = strtok(NULL, "[],");
		localCenter.x = atof(pc);
		pc = strtok(NULL, "[],");
		localCenter.y = atof(pc);
		pc = strtok(NULL, "[],");
		localCenter.z = atof(pc);

		//�ٿ�� ��������
		D3DXVECTOR3 localHalf;
		pc = strtok(NULL, "[],");
		localHalf.x = atof(pc);
		pc = strtok(NULL, "[],");
		localHalf.y = atof(pc);
		pc = strtok(NULL, "[],");
		localHalf.z = atof(pc);

		//���� ������ ������Ʈ ����
		cBaseObject* pNewBound = new cBaseObject;
		pNewBound->SetActive(true);
		pNewBound->pTransform->SetWorldPosition(pos);
		pNewBound->pTransform->SetRotateWorld(quat);
		pNewBound->pTransform->SetScale(scale);
		pNewBound->BoundBox.SetBound(&localCenter, &localHalf);

		//Ǫ��
		this->boundObjects.push_back(pNewBound);

	}
}