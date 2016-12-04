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

	//라이트 푸쉬
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
	//집을 그린다.

	//적용되는 LightMatrix
	D3DXMATRIXA16 matLights[10];
	for (int i = 0; i < this->lights.size(); i++)
		matLights[i] = this->lights[i]->GetLightMatrix();

	//셰이더에 라이팅 셋팅
	cXMesh_Static::sStaticMeshEffect->SetMatrixArray("matLights", matLights, 10);
	cXMesh_Static::sStaticMeshEffect->SetInt("LightNum", this->lights.size());

	cXMesh_Static::SetCamera(this->pMainCamera);
	this->houseObject->Render();



	//바운드를 그린다.
	for (int i = 0; i < this->boundObjects.size(); i++)
		this->boundObjects[i]->Render();
	this->pTrans->RenderGimozo();

	D3DXVECTOR3 worldPos = this->pTrans->GetWorldPosition();

	D3DXVECTOR2 screenPos;
	if (this->pMainCamera->GetWorldPosToScreenPos(&screenPos, &worldPos))
	{
		DXFONT_MGR->PrintTextOutline("설현", screenPos.x, screenPos.y, 0xff00ffff, 0xffff00ff);
	}
	
}

void cScene_01_BoundLoad::LoadBound()
{
	fstream file;
	file.open("BoundData.txt", fstream::in); //fstream::in -> 파일 읽기 모드

	std::vector<std::string> strLine;

	//파일 끝까지 읽는다.
	while (file.eof() == false) //파일 끝을 만나면 trur 값 반환
	{
		std::string line;
		file >> line;			//file 한줄 문자열을 읽어 line에 대입
		strLine.push_back(line);
	}
	file.close(); //다쓴 파일 스트림 닫는다.

	//읽어온 파일 대로 셋팅
	for (int i = 0; i < strLine.size(); i++)
	{
		if (strLine[i].size() == 0)
			continue;

		//한줄 라인이 여기에 대입된다.
		char cStr[2048];
		strcpy(cStr, strLine[i].c_str());

		char* pc;

		//[3.6819,2.71903,13.0518]
		//[-0.621861,-0.277318,-0.272702,0.679719]
		//[1,1,1][0,0,0][7.13382,5.48357,6.06699]

		//위치
		D3DXVECTOR3 pos;
		pc = strtok(cStr, "[],"); //처음짜를때는 짜를 문자열의 주소를 넣는다.
		pos.x = atof(pc);
		pc = strtok(NULL, "[],"); //그 다음부터는 NULL(짤려진 문자열 위치)
		pos.y = atof(pc);
		pc = strtok(NULL, "[],");
		pos.z = atof(pc);


		//사원수
		D3DXQUATERNION quat;
		pc = strtok(NULL, "[],");
		quat.x = atof(pc);
		pc = strtok(NULL, "[],");
		quat.y = atof(pc);
		pc = strtok(NULL, "[],");
		quat.z = atof(pc);
		pc = strtok(NULL, "[],");
		quat.w = atof(pc);

		//스케일
		D3DXVECTOR3 scale;
		pc = strtok(NULL, "[],");
		scale.x = atof(pc);
		pc = strtok(NULL, "[],");
		scale.y = atof(pc);
		pc = strtok(NULL, "[],");
		scale.z = atof(pc);

		//바운드 로컬센터
		D3DXVECTOR3 localCenter;
		pc = strtok(NULL, "[],");
		localCenter.x = atof(pc);
		pc = strtok(NULL, "[],");
		localCenter.y = atof(pc);
		pc = strtok(NULL, "[],");
		localCenter.z = atof(pc);

		//바운드 로컬하프
		D3DXVECTOR3 localHalf;
		pc = strtok(NULL, "[],");
		localHalf.x = atof(pc);
		pc = strtok(NULL, "[],");
		localHalf.y = atof(pc);
		pc = strtok(NULL, "[],");
		localHalf.z = atof(pc);

		//위의 정보로 오브젝트 생성
		cBaseObject* pNewBound = new cBaseObject;
		pNewBound->SetActive(true);
		pNewBound->pTransform->SetWorldPosition(pos);
		pNewBound->pTransform->SetRotateWorld(quat);
		pNewBound->pTransform->SetScale(scale);
		pNewBound->BoundBox.SetBound(&localCenter, &localHalf);

		//푸쉬
		this->boundObjects.push_back(pNewBound);

	}
}