#include "stdafx.h"
#include "cScene_01_Correct.h"


cScene_01_Correct::cScene_01_Correct()
{
}


cScene_01_Correct::~cScene_01_Correct()
{
}


HRESULT cScene_01_Correct::Scene_Init()
{
	//�ε� ���....
	//std::string filePath = "../Resources/Meshes/Fighter/Fighter.x";		//X ���� ��θ�
	std::string filePath = "../../Resources/Meshes/GunShip/gunship.x";		//X ���� ��θ�

	LPD3DXBUFFER		pAdjacency;		//���� ����
	LPD3DXBUFFER		pMaterial;		//���� ����

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
	for (DWORD i = 0; i < this->dwMaterialsNum; i++)
	{

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

	//
	// �޽� ���� ó��
	//

	//���� �ִ� �Ž����� ������ ���� ��ķ� �������� 
	//��� ������ Ʋ�������.
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale, 0.01f, 0.01f, 0.01f);

	D3DXMATRIXA16 matRotate;
	D3DXMatrixRotationY(&matRotate, 180.0f * ONE_RAD);

	D3DXMATRIXA16 matCorrection = matScale * matRotate;

	//���� Element �� ��� ���� ������ ����
	D3DVERTEXELEMENT9 pVerElement[MAX_FVF_DECL_SIZE];

	//pVerElement �迭���� ������ ��� ������ ���� �ȴ�.
	pMesh->GetDeclaration(pVerElement);

	//���� Position Offset;
	int positionOffset = -1;

	//���� Normal Offset;
	int normalOffet = -1;

	//���� Tangent Offset;
	int tangentOffet = -1;

	//���� Binormal Offset;
	int binormalOffet = -1;

	//�ϴ� �����糤��..
	for (DWORD i = 0; i < MAX_FVF_DECL_SIZE; i++)
	{

		//�������� �����ٸ�....
		if (pVerElement[i].Type == D3DDECLTYPE_UNUSED)
		{
			//Ż��
			break;
		}

		//������ġ ������ �����ٸ�....
		if (pVerElement[i].Usage == D3DDECLUSAGE_POSITION)
		{
			positionOffset = pVerElement[i].Offset;
		}

		//�����븻 ������ �����ٸ�...
		else if (pVerElement[i].Usage == D3DDECLUSAGE_NORMAL)
		{
			normalOffet = pVerElement[i].Offset;
		}

		//����ź��Ʈ ������ �����ٸ�...
		else if (pVerElement[i].Usage == D3DDECLUSAGE_TANGENT)
		{
			tangentOffet = pVerElement[i].Offset;
		}

		//�������̳븻 ������ �����ٸ�...
		else if (pVerElement[i].Usage == D3DDECLUSAGE_BINORMAL)
		{
			binormalOffet = pVerElement[i].Offset;
		}
	}

	//���ؽ� ����
	DWORD verNum = pMesh->GetNumVertices();

	//�޽��� ���� FVF ����Ȥ�� Decl ������ �̿��Ͽ� �����ϳ��� ũ�⸦ �˾Ƴ���.
	//DWORD verSize = D3DXGetFVFVertexSize( pMesh->GetFVF() );
	DWORD verSize = D3DXGetDeclVertexSize(pVerElement, 0);

	//�޽��� ���ؽ� ���۸� Lock �Ѵ�
	void* p = NULL;
	pMesh->LockVertexBuffer(0, &p);

	//���ؽ� ����Ŭ ���� �糤��....
	for (DWORD i = 0; i < verNum; i++){

		//���ؽ� ���� �ּ�
		char* pVertex = ((char*)p + (i * verSize));

		//���� ��ġ�� �ִٸ�...
		if (positionOffset != -1){

			D3DXVECTOR3* pos = (D3DXVECTOR3*)(pVertex + positionOffset);

			D3DXVec3TransformCoord(
				pos,
				pos,
				&matCorrection);

		}

		//�븻������ �ִٸ�..
		if (normalOffet != -1){
			D3DXVECTOR3* nor = (D3DXVECTOR3*)(pVertex + normalOffet);

			D3DXVec3TransformNormal(
				nor,
				nor,
				&matCorrection);

			D3DXVec3Normalize(nor, nor);

		}


		//tangent ������ �ִٸ�.
		if (tangentOffet != -1){
			D3DXVECTOR3* tangent = (D3DXVECTOR3*)(pVertex + tangentOffet);

			D3DXVec3TransformNormal(
				tangent,
				tangent,
				&matCorrection);

			D3DXVec3Normalize(tangent, tangent);

		}

		//binormal ������ �ִٸ�
		if (binormalOffet != -1){
			D3DXVECTOR3* binor = (D3DXVECTOR3*)(pVertex + binormalOffet);

			D3DXVec3TransformNormal(
				binor,
				binor,
				&matCorrection);

			D3DXVec3Normalize(binor, binor);
		}

	}

	pMesh->UnlockVertexBuffer();

	//������ ����
	Device->SetRenderState(D3DRS_LIGHTING, false);

	//�ε��� ����� ���۴� ����
	SAFE_RELEASE(pAdjacency);

	return S_OK;
}

void cScene_01_Correct::Scene_Release()
{
	SAFE_RELEASE(pMesh);
}

void cScene_01_Correct::Scene_Update(float timeDelta)
{
	this->meshTrans.DefaultControl2(timeDelta);
}

void cScene_01_Correct::Scene_Render1()
{
	this->meshTrans.SetDeviceWorld(Device);

	for (DWORD i = 0; i < this->dwMaterialsNum; i++)
	{
		//Texture ����
		Device->SetTexture(0, vecTextures[i]);

		this->pMesh->DrawSubset(i);
	}

	//�ٱ׸��� ���� Texture �� �������ִ� ����
	Device->SetTexture(0, NULL);
}