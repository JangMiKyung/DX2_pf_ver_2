#include "stdafx.h"
#include "cXMesh_Static.h"


LPD3DXEFFECT cXMesh_Static::sStaticMeshEffect = NULL;
LPDIRECT3DTEXTURE9		cXMesh_Static::sDefaultDiffuseTex = NULL;
LPDIRECT3DTEXTURE9		cXMesh_Static::sDefaultNormalTex = NULL;
LPDIRECT3DTEXTURE9		cXMesh_Static::sDefaultSpecularTex = NULL;
LPDIRECT3DTEXTURE9		cXMesh_Static::sDefaultEmissionTex = NULL;


cXMesh_Static::cXMesh_Static()
{
	if (sStaticMeshEffect == NULL){
		sStaticMeshEffect = RESOURCE_FX->GetResource("../../Resources/Shaders/StaticMesh2.fx");
	}

	if (sDefaultDiffuseTex == NULL)
		sDefaultDiffuseTex = RESOURCE_TEXTURE->GetResource("../../Resources/Meshes/diffuseDefault.png");

	if (sDefaultNormalTex == NULL)
		sDefaultNormalTex = RESOURCE_TEXTURE->GetResource("../../Resources/Meshes/normalDefault.png");

	if (sDefaultSpecularTex == NULL)
		sDefaultSpecularTex = RESOURCE_TEXTURE->GetResource("../../Resources/Meshes/specularDefault.png");

	if (sDefaultEmissionTex == NULL)
		sDefaultEmissionTex = RESOURCE_TEXTURE->GetResource("../../Resources/Meshes/emissionDefault.png");
}


cXMesh_Static::~cXMesh_Static()
{
}

HRESULT cXMesh_Static::Init(std::string filePath, const D3DXMATRIXA16* matCorrection /*= NULL */)
{

	LPD3DXBUFFER		pAdjacency = NULL;		//���� ����
	LPD3DXBUFFER		pMaterial = NULL;		//���� ����

	//XFile �� ���� �޽��� �ҷ����� �Լ�
	if (FAILED(D3DXLoadMeshFromX(
		filePath.c_str(),			//X ���� �ε� ���...
		D3DXMESH_MANAGED,			//�޽� �ɼ� �ϴ� D3DXMESH_MANAGED ��....
		Device,						//����̽�
		&pAdjacency,				//���� �������� ����
		&pMaterial,					//���� �������� ����
		NULL,						//Effect 
		&this->dwMaterialsNum,		//�޽����� ���Ǵ� ���� ����
		&this->pMesh				//�޽�
		))){
		SAFE_RELEASE(pAdjacency);
		SAFE_RELEASE(pMaterial);
		return E_FAIL;
	}

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


	//Texture �ε��� �ʿ��� ���ڿ���..
	std::string texFilePath;	//���� ���...
	std::string texExp;			//���� Ȯ���� ��
	std::string texFile;		//���� ��	

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

			//tex ���ϰ�δ� Mesh ���ϰ�� + texture �����̸�
			texFilePath = path + pMaterials[i].pTextureFilename;

			//Texture �ε��ϰ� Ǫ��
			vecDiffuseTex.push_back(
				RESOURCE_TEXTURE->GetResource(texFilePath));

			//���� �̸��� Ȯ���� ��ġ
			int dotIndex = texFilePath.find_last_of(".");

			//���� ��� Ȯ���ڸ� ������.
			texFile = texFilePath.substr(0, dotIndex);
			texExp = texFilePath.substr(dotIndex + 1, filePath.length());

			//
			// �븻�� ���� _N �� �ٴ´�.
			//
			texFilePath = texFile + "_N." + texExp;
			LPDIRECT3DTEXTURE9 pNorTex = RESOURCE_TEXTURE->GetResource(texFilePath);
			//���ٸ�...
			if (pNorTex == NULL)
				this->vecNormalTex.push_back(sDefaultNormalTex);
			else
				this->vecNormalTex.push_back(pNorTex);

			//
			// ����ŧ�� ���� _S �� �ٴ´�.
			//
			texFilePath = texFile + "_S." + texExp;
			LPDIRECT3DTEXTURE9 pSpecTex = RESOURCE_TEXTURE->GetResource(texFilePath);
			//���ٸ�...
			if (pSpecTex == NULL)
				this->vecSpecularTex.push_back(sDefaultSpecularTex);
			else
				this->vecSpecularTex.push_back(pSpecTex);


			//
			// �̹̼� ���� _E �� �ٴ´�.
			//
			texFilePath = texFile + "_E." + texExp;
			LPDIRECT3DTEXTURE9 pEmiTex = RESOURCE_TEXTURE->GetResource(texFilePath);
			//���ٸ�...
			if (pEmiTex == NULL)
				this->vecEmissionTex.push_back(sDefaultEmissionTex);
			else
				this->vecEmissionTex.push_back(pEmiTex);
		}

		else
		{
			this->vecDiffuseTex.push_back(sDefaultDiffuseTex);
			this->vecNormalTex.push_back(sDefaultNormalTex);
			this->vecSpecularTex.push_back(sDefaultSpecularTex);
			this->vecEmissionTex.push_back(sDefaultEmissionTex);
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
	//��������� �޾Ҵٸ�..
	if (matCorrection){
		this->MeshCorrection(matCorrection);
	}
	else{

		//��������� ������ ����ó�����ؾ� Bound ������ ������ �ִ�.
		D3DXMATRIXA16 matIden;
		D3DXMatrixIdentity(&matIden);
		this->MeshCorrection(&matIden);
	}
}

void cXMesh_Static::Release()
{
	SAFE_RELEASE(pMesh);
}

void cXMesh_Static::Render(const cTransform* Trans)
{
	//������� ����
	D3DXMATRIXA16 matWorld = Trans->GetFinalMatrix();
	sStaticMeshEffect->SetMatrix("matWorld", &matWorld);

	//���� ���� �ϴ�...
	D3DXVECTOR4 lightDir(1, -1, 1, 1);
	sStaticMeshEffect->SetVector("vLightDir", &lightDir);

	//����Ʈ �÷� �ϴ� ���
	D3DXVECTOR4 lightColor(1, 1, 1, 1);
	sStaticMeshEffect->SetVector("vLightColor", &lightColor);


	//Effect �� �׸��� ����
	sStaticMeshEffect->SetTechnique("Mesh");

	UINT passNum;
	sStaticMeshEffect->Begin(&passNum, 0);

	for (UINT i = 0; i < passNum; i++)
	{
		sStaticMeshEffect->BeginPass(i);

		//����¼���ŭ ���� �糤��...
		for (int m = 0; m < this->dwMaterialsNum; m++){

			//�ؽ��� ����
			sStaticMeshEffect->SetTexture("Diffuse_Tex", this->vecDiffuseTex[m]);
			sStaticMeshEffect->SetTexture("Normal_Tex", this->vecNormalTex[m]);
			sStaticMeshEffect->SetTexture("Specular_Tex", this->vecSpecularTex[m]);
			sStaticMeshEffect->SetTexture("Emission_Tex", this->vecEmissionTex[m]);
			//�����Ŀ�
			sStaticMeshEffect->SetFloat("fSpecPower", this->vecMaterials[m].Power);

			//Begin �� ������ ���� ���� �ٲ�� ������ ���� ����
			sStaticMeshEffect->CommitChanges();

			this->pMesh->DrawSubset(m);
		}

		sStaticMeshEffect->EndPass();
	}

	sStaticMeshEffect->End();

}

//������Ĵ�� �޽��� �����Ѵ�.
void cXMesh_Static::MeshCorrection(const D3DXMATRIXA16* pmatCorrection)
{
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
	for (DWORD i = 0; i < MAX_FVF_DECL_SIZE; i++){

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

	//�ٿ�� MinMax ����� ���� �ʱ�ȭ......
	this->Bound_Min = D3DXVECTOR3(0, 0, 0);
	this->Bound_Max = D3DXVECTOR3(0, 0, 0);



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
				pmatCorrection);

			//���� �ּ� ������
			if (Bound_Min.x > pos->x)		Bound_Min.x = pos->x;
			if (Bound_Min.y > pos->y)		Bound_Min.y = pos->y;
			if (Bound_Min.z > pos->z)		Bound_Min.z = pos->z;

			//���� �ִ� ������
			if (Bound_Max.x < pos->x)		Bound_Max.x = pos->x;
			if (Bound_Max.y < pos->y)		Bound_Max.y = pos->y;
			if (Bound_Max.z < pos->z)		Bound_Max.z = pos->z;

			//���� ��ġ Ǫ��
			this->Vertices.push_back(*pos);
		}

		//�븻������ �ִٸ�..
		if (normalOffet != -1){
			D3DXVECTOR3* nor = (D3DXVECTOR3*)(pVertex + normalOffet);

			D3DXVec3TransformNormal(
				nor,
				nor,
				pmatCorrection);

			D3DXVec3Normalize(nor, nor);

			//���� �븻 Ǫ��
			Normals.push_back(*nor);
		}


		//tangent ������ �ִٸ�.
		if (tangentOffet != -1){
			D3DXVECTOR3* tangent = (D3DXVECTOR3*)(pVertex + tangentOffet);

			D3DXVec3TransformNormal(
				tangent,
				tangent,
				pmatCorrection);

			D3DXVec3Normalize(tangent, tangent);

		}

		//binormal ������ �ִٸ�
		if (binormalOffet != -1){
			D3DXVECTOR3* binor = (D3DXVECTOR3*)(pVertex + binormalOffet);

			D3DXVec3TransformNormal(
				binor,
				binor,
				pmatCorrection);

			D3DXVec3Normalize(binor, binor);
		}
	}
	pMesh->UnlockVertexBuffer();

	//Bound �߰� ���
	this->Bound_Center = (this->Bound_Min + this->Bound_Max) * 0.5f;
	this->Bound_Size = D3DXVECTOR3(
		Bound_Max.x - Bound_Min.x,
		Bound_Max.y - Bound_Min.y,
		Bound_Max.z - Bound_Min.z);
	this->Bound_HalfSize = this->Bound_Size * 0.5f;
	this->Bound_Radius = D3DXVec3Length(&(this->Bound_Center - this->Bound_Min));


	//
	// �ε��� ���۸� ��´�
	//
	LPDIRECT3DINDEXBUFFER9 pIndexBuffer;
	this->pMesh->GetIndexBuffer(&pIndexBuffer);

	//���� ����
	this->TriNum = pMesh->GetNumFaces();

	//�ε��� ���ۿ� ���� ������ ��´�.
	D3DINDEXBUFFER_DESC desc;
	pIndexBuffer->GetDesc(&desc);

	if (desc.Format == D3DFMT_INDEX16)
	{
		WORD* p = NULL;
		pIndexBuffer->Lock(0, 0, (void**)&p, 0);

		for (DWORD i = 0; i < TriNum; i++)
		{
			this->Indices.push_back(p[i * 3 + 0]);
			this->Indices.push_back(p[i * 3 + 1]);
			this->Indices.push_back(p[i * 3 + 2]);

		}


		pIndexBuffer->Unlock();

	}

	else if (desc.Format == D3DFMT_INDEX32)
	{
		DWORD* p = NULL;
		pIndexBuffer->Lock(0, 0, (void**)&p, 0);

		for (DWORD i = 0; i < TriNum; i++)
		{
			this->Indices.push_back(p[i * 3 + 0]);
			this->Indices.push_back(p[i * 3 + 1]);
			this->Indices.push_back(p[i * 3 + 2]);
		}

		pIndexBuffer->Unlock();
	}

	//���� �ε��� ���۴� ����
	SAFE_RELEASE(pIndexBuffer);
}


/////////////////////////////////////////////////////////////////////////////////////////

void cXMesh_Static::SetCamera(const cCamera* Camera)
{
	D3DXMATRIXA16 matViewProj = Camera->GetViewProjectionMatrix();

	sStaticMeshEffect->SetMatrix("matViewProjection", &matViewProj);

	D3DXVECTOR3 vEyePos = Camera->GetWorldPosition();

	sStaticMeshEffect->SetVector("vEyePos", &D3DXVECTOR4(vEyePos, 1));
}
