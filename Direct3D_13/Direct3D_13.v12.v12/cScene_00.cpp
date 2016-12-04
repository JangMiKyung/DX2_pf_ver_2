#include "StdAfx.h"
#include "cScene_00.h"
#include "cTerrain.h"

cScene_00::cScene_00(void)
{
}


cScene_00::~cScene_00(void)
{
}

HRESULT cScene_00::Scene_Init()
{
	//�ε� ���....
	//std::string filePath = "../../Resources/Meshes/Fighter/Fighter.x";		//X ���� ��θ�
	std::string filePath = "../../Resources/Meshes/GunShip/gunship.x";		//X ���� ��θ�

	//Texture �� �ҷ��´ٸ�
	//"../../Resources/Meshes/Fighter/ �̰�ο��� �ҷ��;� �Ѵ�...


	LPD3DXBUFFER		pAdjacency;		//���� ����
	LPD3DXBUFFER		pMaterial;		//���� ����
	DWORD				dwNumMtrl;		//���� �� 

	//XFile �� ���� �޽��� �ҷ����� �Լ�
	D3DXLoadMeshFromX(
		filePath.c_str(),			//X ���� �ε� ���...
		D3DXMESH_MANAGED,			//�޽� �ɼ� �ϴ� D3DXMESH_MANAGED ��....
		Device,						//����̽�
		&pAdjacency,				//���� �������� ����
		&pMaterial,					//���� �������� ����
		NULL,						//Effect 
		&this->dwMaterialsNum,		//�޽����� ���Ǵ� ���� ����
		&this->pMesh				//�޽�
		);

	//�ε� ��ο��� ���ϸ� �����ϰ� ��θ� �޴´�.
	std::string path;
	int lastPathIndex = 0;		//������ \ �� ��ġ

	//filePath.find_first_of( 'A' );		filePath ���ڿ��� 'A' �� ����ִ� ��ġ ���� ( �տ��� ���� �˻� )
	lastPathIndex = filePath.find_last_of('/');		//�ڿ��� ���� �˻�
	if (lastPathIndex == -1){			//��ã���� -1 �� ����
		lastPathIndex = filePath.find_last_of('\\');	//��� ������ �ϴ� / �� ��ã���� \ �� ã�´�.
	}

	//��� ������ �ִٸ�...
	if (lastPathIndex != -1){
		//filePath.substr( /*������ġ*/, /*����*/ );
		path = filePath.substr(0, lastPathIndex + 1);
	}

	//���� ������ ��´�
	LPD3DXMATERIAL	pMaterials = (LPD3DXMATERIAL)pMaterial->GetBufferPointer();

	//����������ŭ for ������ �����糤��.
	for (DWORD i = 0; i < this->dwMaterialsNum; i++){

		//�ε�� ���͸���
		D3DMATERIAL9 mtrl = pMaterials[i].MatD3D;

		//�޽��� ���͸��󿡴� Ambient ������ ������
		//Diffuse ������ �����ϰ� ����..
		mtrl.Ambient = mtrl.Diffuse;

		//���͸��� ���� Ǫ��
		this->vecMaterials.push_back(mtrl);

		//���͸����� Texture ������ �ִٸ�..
		if (pMaterials[i].pTextureFilename != NULL){

			//���ϰ�� 
			std::string texFilePath = path + pMaterials[i].pTextureFilename;

			//Texture �ε��ϰ� Ǫ��
			vecTextures.push_back(
				RESOURCE_TEXTURE->GetResource(texFilePath));

		}

		else
		{
			//������ �ݵ�� null ����( �߿� )
			vecTextures.push_back(NULL);
		}
	}

	//���� ���� ������ �� ����Ͽ� ������ Texture �� �ε� �߱⶧���� 
	//���̻� pMaterial ���� ���۴� �ʿ� ���� �� �޸� ���縦 ���־�� �Ѵ�.
	SAFE_RELEASE(pMaterial);

	//�޽� ����ȭ ( ���� ���۸� �̿��Ͽ� �޽��� ����ȭ �Ѵ� )
	this->pMesh->OptimizeInplace(
		D3DXMESHOPT_ATTRSORT |		//�޽� ����¼������ ���� ( DrawSubset ȿ���� ���δ� )
		D3DXMESHOPT_COMPACT |		//�޽����� ������ �ʴ� ������ �ε��� ����
		D3DXMESHOPT_VERTEXCACHE,	//���� Cache ��Ʈ�� ������.
		(DWORD*)pAdjacency->GetBufferPointer(),		//�������� �Ѱܾ� �Ѵ�.
		NULL,			//����ȭ�� ������ ����� ��� �������� ( ���̻� �ʿ������ NULL )
		NULL,			//����ȭ�� �ε��� ����
		NULL			//����ȭ�� ���� ���� ���� 
		);


	//����ȭ�� �������� ����
	SAFE_RELEASE(pAdjacency);

	this->meshTrans.SetScale(0.05f, 0.05f, 0.05f);
	this->meshLocal.AttachTo(&this->meshTrans);
	this->meshLocal.RotateLocal(0, 180.0f * ONE_RAD, 0);



	//������ ����
	Device->SetRenderState(D3DRS_LIGHTING, false);

	//�ε��� ����� ���۴� ����
	SAFE_RELEASE(pAdjacency);

	return S_OK;
}

void cScene_00::Scene_Release()
{
	SAFE_RELEASE(pMesh);
}

void cScene_00::Scene_Update(float timDelta)
{
	this->meshTrans.DefaultControl2(timDelta);
}

void cScene_00::Scene_Render1()
{
	this->meshLocal.SetDeviceWorld(Device);

	for (DWORD i = 0; i < this->dwMaterialsNum; i++)
	{
		//Texture ����
		Device->SetTexture(0, vecTextures[i]);

		this->pMesh->DrawSubset(i);
	}

	//�ٱ׸��� ���� Texture �� �������ִ� ����
	Device->SetTexture(0, NULL);
}