#include "stdafx.h"
#include "BoneHierachy.h"
#include "cXMesh_Skinned.h"
#include "cTransform.h"


void BONE_HIERARCHY::SetLoadPath(std::string filePath)
{
	int pathIndex = filePath.find_last_of("/");
	if (pathIndex == -1)
	{
		pathIndex = filePath.find_last_of("\\");

		//��������´ٸ� filePath ��ΰ� ���ԵǾ����� �ʴ�
		if (pathIndex == -1)
		{
			m_pFileName = filePath;
			m_pFilePath.clear();
			return;
		}
	}
	m_pFileName = filePath.substr(pathIndex + 1, filePath.size());
	m_pFilePath = filePath.substr(0, pathIndex + 1);
}

void BONE_HIERARCHY::SetSkinnedMesh(cXMesh_Skinned* pSkinnedMesh)
{
	this->m_pSkinnedMesh = pSkinnedMesh;
}


HRESULT BONE_HIERARCHY::CreateFrame(THIS_
	LPCSTR Name,					//������ �̸� ( �ҷ��� ���� �� �������� �̸��� ���´� )
	LPD3DXFRAME* ppNewFrame			//���Ӱ� �Ҵ�Ǿ� ������ ������ ( �츮�� ���⿡�� BONE �Ҵ��Ͽ� �Ű������� ���� �̸��� �����Ͽ� �Ҵ�����־�� �Ѵ� )
	)
{
	//ppNewFrame ���� �Ѱ��� ���ο� BONE ����
	BONE*		newBone = new BONE;
	ZeroMemory(newBone, sizeof(BONE));

	//���̸� ����
	if (Name != NULL)
	{
		int len = strlen(Name);
		newBone->Name = new char[len + 1];
		strcpy_s(newBone->Name, len + 1, Name);
	}

	//��Ʈ���� ���� �ʱ�ȭ
	D3DXMatrixIdentity(&newBone->TransformationMatrix);
	D3DXMatrixIdentity(&newBone->CombinedTransformationMatrix);


	newBone->pBoneTransform = NULL;
	newBone->pApplyTransform = NULL;

	//���ϰ��� ���ο� �� �ּ� ����
	*ppNewFrame = newBone;

	return S_OK;
}

//
// ��������( ����� ) �� �޽ø� �Ҵ�
//
HRESULT BONE_HIERARCHY::CreateMeshContainer(THIS_
	LPCSTR Name,										//�Ҵ� �Ǵ� �޽��� �̸�
	CONST D3DXMESHDATA* pMeshData,						//�Ҵ� �Ǵ� �޽��� ������ 
	CONST D3DXMATERIAL* pMaterials,						//�Ҵ� �Ǵ� �޽��� ���͸���
	CONST D3DXEFFECTINSTANCE* pEffectInstance,			//Effect �� ����ϴ� �޽��� ��� Effect �ν��Ͻ� 
	DWORD NumMaterials,									//�Ҵ� �Ǵ� �޽��� ������ 
	CONST DWORD* pAdjacency,							//�Ҵ� �Ǵ� �޽��� ���� ���� ����
	LPD3DXSKININFO pSkinInfo,							//�Ҵ� �Ǵ� �޽��� ��Ű�� ���� ( ���� �����ӿ� ���� ��Ű�� ���� )
	LPD3DXMESHCONTAINER* ppNewMeshContainer				//���Ӱ� �Ҵ�Ǿ� ������ �޽� ( �츮�� ���⿡�� ���� �Ű������� ���� ������� BONE_MESH �� �Ҵ��Ͽ� ���� �� �־�� �Ѵ� )
	)
{
	//ppNewMeshContainer �� �Ѱ��� ���ο� BONEMESH ����
	BONE_MESH* boneMesh = new BONE_MESH;
	ZeroMemory(boneMesh, sizeof(BONE_MESH));

	//�Ű������� ���� �޽� �������� Device �� ��´�.
	LPDIRECT3DDEVICE9 d3dDevice = NULL;
	pMeshData->pMesh->GetDevice(&d3dDevice);

	//�޽õ����͸� ������.
	boneMesh->MeshData.pMesh = pMeshData->pMesh;
	boneMesh->MeshData.Type = D3DXMESHTYPE_MESH;

	//�޽õ������� ���۷��� ī��Ʈ ����
	pMeshData->pMesh->AddRef();
	//���۷��� ī��Ʈ�� ��������� ������ �ʴ´ٸ� �Ű������� ���� pMeshData �� �Լ� ȣ���� ���� �Ǿ� ������!
	//�� �Լ� ȣ���Ŀ��� ���������� �������� 0 �̸� �ڵ����� �����Ǵ� ���̴�.
	//���� ���θ������ BONEMESH �ɹ������Ϳ� pMeshData �����͸� ����������
	//���۷��� ī��Ʈ ���� ���� ���� �Լ��� �����ڿ��� ������ ���´�.

	//�̸� ����
	if (Name != NULL)
	{
		int len = strlen(Name);
		boneMesh->Name = new char[len + 1];
		strcpy_s(boneMesh->Name, len + 1, Name);
	}

	//�������� ���� ( ���� ������ ��� 3���� ���ϰ� �ȴ� �ֳĸ� �������� �������� 3���ϱ� )
	DWORD numAdjacency = 3 * (boneMesh->MeshData.pMesh->GetNumFaces());		//���������� ��
	boneMesh->pAdjacency = new DWORD[numAdjacency];							//���� ������ ����ŭ ����
	memcpy(boneMesh->pAdjacency, pAdjacency, sizeof(DWORD) * numAdjacency);


	std::string texFilePath;
	std::string texExp;			//���� Ȯ���� ��
	std::string texFile;		//���� ��`

	//�������� 
	std::string texPath;

	boneMesh->NumMaterials = NumMaterials;
	boneMesh->pMaterials = new D3DXMATERIAL[NumMaterials];

	for (DWORD i = 0; i < NumMaterials; i++)
	{
		//���͸��� ���� ����
		memcpy(&boneMesh->pMaterials[i], &pMaterials[i], sizeof(D3DXMATERIAL));

		//���͸������� Ǫ��
		D3DMATERIAL9 mtrl;
		memcpy(&mtrl, &pMaterials[i].MatD3D, sizeof(D3DMATERIAL9));
		mtrl.Ambient = mtrl.Diffuse;		//���� Ambient
		boneMesh->Materials.push_back(mtrl);

		//Texture �� �����Ѵٸ�...
		if (pMaterials[i].pTextureFilename != NULL)
		{
			//�̸� ���� ����
			int len = strlen(pMaterials[i].pTextureFilename);
			boneMesh->pMaterials[i].pTextureFilename = new char[len + 1];
			strcpy_s(boneMesh->pMaterials[i].pTextureFilename, len + 1, pMaterials[i].pTextureFilename);


			texPath = this->m_pFilePath + pMaterials[i].pTextureFilename;

			/*
			//Texture �ε�
			LPDIRECT3DTEXTURE9 pTex;
			if( FAILED( D3DXCreateTextureFromFile(
			d3dDevice, texPath.c_str(), &pTex ) ) ){
			//Texture Ǫ��
			boneMesh->Textures.push_back( NULL );

			}

			else
			{
			//Texture Ǫ��
			boneMesh->Textures.push_back( pTex );
			}*/

			boneMesh->DiffuseTexs.push_back(
				RESOURCE_TEXTURE->GetResource(texPath));



			//���� �̸��� Ȯ���� ��ġ
			int dotIndex = texPath.find_last_of(".");

			//���� ��� Ȯ���ڸ� ������.
			texFile = texPath.substr(0, dotIndex);
			texExp = texPath.substr(dotIndex + 1, texPath.length());

			//
			// �븻�� ���� _N �� �ٴ´�.
			//
			texFilePath = texFile + "_N." + texExp;

			LPDIRECT3DTEXTURE9 pNorTex =
				RESOURCE_TEXTURE->GetResource(texFilePath);
			//���� Texture �� ���ٸ�...
			if (pNorTex == NULL){
				pNorTex = RESOURCE_TEXTURE->GetResource("../Resources/Meshes/normalDefault.png");
			}


			//
			// Emission ���� _E �� �ٴ´�.
			//
			texFilePath = texFile + "_E." + texExp;

			LPDIRECT3DTEXTURE9 pEmiTex =
				RESOURCE_TEXTURE->GetResource(texFilePath);
			//���� Texture �� ���ٸ�...
			if (pEmiTex == NULL){
				pEmiTex = RESOURCE_TEXTURE->GetResource("../Resources/Meshes/emissionDefault.png");
			}


			//
			// Specular ���� _S �� �ٴ´�.
			//
			texFilePath = texFile + "_S." + texExp;

			LPDIRECT3DTEXTURE9 pSpcTex =
				RESOURCE_TEXTURE->GetResource(texFilePath);
			//���� Texture �� ���ٸ�...
			if (pSpcTex == NULL){
				pSpcTex = RESOURCE_TEXTURE->GetResource("../Resources/Meshes/specularDefault.png");
			}
			//Texture Ǫ��
			boneMesh->NormalTexs.push_back(pNorTex);
			boneMesh->SpecularTexs.push_back(pSpcTex);
			boneMesh->EmissionTexs.push_back(pEmiTex);
		}

		//Texture ������ ���ٸ�..
		else
		{
			//Texture�� �̸��� �������� �ʴ´�.
			boneMesh->pMaterials[i].pTextureFilename = NULL;

			//NULL ���̶� �����ؾ� �Ѵ�
			boneMesh->DiffuseTexs.push_back(NULL);
			boneMesh->NormalTexs.push_back(NULL);
			boneMesh->SpecularTexs.push_back(NULL);
			boneMesh->EmissionTexs.push_back(NULL);
		}
	}


	//�Ӽ����̺��� ��´�.
	boneMesh->MeshData.pMesh->GetAttributeTable(NULL, &boneMesh->NumAttributesGroup);			//�Ӽ� �׷� ������ ��´�.

	//boneMesh->MeshData.pMesh->GetAttributeTable( boneMesh->AttributeTable, NULL );				//�Ӽ� ���̺��� ��´�.


	//��Ű�� ���� �� �ִٸ�....
	if (pSkinInfo != NULL)
	{
		//��Ű�� ������ ������´�
		boneMesh->pSkinInfo = pSkinInfo;

		//��Ų���� ���۷��� ī��Ʈ ���� ( ���� �޽õ������� ���۷��� ī��Ʈ�� ������ ���� )
		pSkinInfo->AddRef();

		//�̸޽ÿ� ���Ǵ� ���� ���� ��´�.
		int numBone = pSkinInfo->GetNumBones();

		//���Ǵ� ���� ����ŭ ��� �����Ҵ�
		boneMesh->pBoneOffsetMatices = new D3DXMATRIXA16[numBone];
		//�� Offset ��� ����
		for (int i = 0; i < numBone; i++)
			boneMesh->pBoneOffsetMatices[i] = *(boneMesh->pSkinInfo->GetBoneOffsetMatrix(i));

		//Effect ���� ���Ǵ� ��� �ȷ�Ʈ ���� ���� ��´�.
		UINT iPaletteSize = 0;
		m_pSkinnedMesh->m_pSkinnedEffect->GetInt("MATRIX_PALETTE_SIZE", (INT*)&iPaletteSize);

		//�⺻������ ����Ǵ� ���� ��ŭ �ȷ�Ʈ�� �Ҵ��ϴµ� Effect �� �ִ� MATRIX_PALETTE_SIZE ���� ����Ǵ� ���� 
		//���� ���ٸ� �ȷ�Ʈ ���� Effect MATRIX_PALETTE_SIZE ���� ���� �Ѵ�.
		boneMesh->NumPaletteEntries = min(iPaletteSize, boneMesh->pSkinInfo->GetNumBones());


		//���� �۾����̾� Mesh��  ����Ǵ� bone�� ���� Effect ���� �����Ǵ� MATRIX_PALETTE_SIZE ������ ���� ���
		//boneMesh->NumPaletteEntries ���� ���߾ Mesh �� SubSet �� ���� �ʿ䰡 �ִ�.
		//�Ʒ��� �Լ��� �׷��� �۾� ���� WorkingMesh �� ���ְ� ���� WorkingMesh �� 
		//���� ����ġ ������ �߰����ش�.


		boneMesh->pSkinInfo->ConvertToIndexedBlendedMesh(
			boneMesh->MeshData.pMesh,		//���� �޽�
			D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,	//�޽� �ɼ�
			boneMesh->NumPaletteEntries,		//�� �ȷ�Ʈ ���� �ѱ��.
			boneMesh->pAdjacency,			//���� ���۸� �ѱ��.
			NULL,							//���ŵ� ���� ���۸� �޾ƿ´� ( �ʿ������ NULL )
			NULL,							//���ŵ� ������(�����ε���) ���۸� �޾ƿ´� ( �ʿ������ NULL )
			NULL,							//���ŵ� ���� ���۸� �޾ƿ´� ( �ʿ������ NULL )
			&boneMesh->MaxNumFaceInfls,		//�� ���ؽ��� ������ �ִ� ���� �ִ� ������ ��´�.
			&boneMesh->NumAttributesGroup,	//���� �ĺ���̼� �Ӽ��׷��� ���� ��´٤�.
			&boneMesh->BufBoneCombos,		//���� �ĺ���̼� ���۸� ��´�.
			&boneMesh->WorkingMesh			//�����޽����� �۾��� �޽�
			);
		//���� �ĺ���̼� �� �޽��� Subset ���� ��� ���� ������ �޴����� ���� ������ ���ϰ� �ȴ�.
		//����Ʈ���� �ε��� = ���ĺ���̼�[Subset].BoneID[�ȷ�Ʈ]

		//cMesh_Skinned �� �۾��� ���Ǵ� �ȷ�Ʈ ��� ���� �����ϸ� 
		//�ٽ� ���Ҵ� �Ѵ�.
		if (m_pSkinnedMesh->m_dwWorkingPaletteSize < boneMesh->NumPaletteEntries)
		{
			if (m_pSkinnedMesh->m_pmWorkingPalette)
				delete[] m_pSkinnedMesh->m_pmWorkingPalette;

			m_pSkinnedMesh->m_dwWorkingPaletteSize = boneMesh->NumPaletteEntries;
			m_pSkinnedMesh->m_pmWorkingPalette = new D3DXMATRIXA16[boneMesh->NumPaletteEntries];
		}
	}

	//������ ���ϵ� LPD3DXMESHCONTAINER �� ���޽� ����
	*ppNewMeshContainer = boneMesh;

	//���� Device ����
	SAFE_RELEASE(d3dDevice);

	return S_OK;

}



//
// ������ ( ����� ) �� ����
//
HRESULT BONE_HIERARCHY::DestroyFrame(THIS_
	LPD3DXFRAME pFrameToFree							//������ ���������� �ּ�( ���� CreateFrame ���� �Ҵ�� BONE ��ü�� �ּҰ� ���´� ) 
	)
{
	if (pFrameToFree)
	{
		//Transform �ۻ쳿
		//SAFE_DELETE( ( (BONE*)pFrameToFree )->pBoneTransform );

		//�̸� ����
		SAFE_DELETE_ARR(pFrameToFree->Name);
		SAFE_DELETE(pFrameToFree);
	}

	return S_OK;
}


//
// ������ ( ����� ) �޽õ����͸� ����
//
HRESULT BONE_HIERARCHY::DestroyMeshContainer(THIS_
	LPD3DXMESHCONTAINER pMeshContainerToFree			//������ ���޽� �������� �ּ� ( ���� CreateMeshContainer ���� �Ҵ�� BONE_MESH ��ü�� �ּҰ� ���´� ) 
	)
{
	//boneMesh �� ����ȯ
	BONE_MESH* boneMesh = (BONE_MESH*)pMeshContainerToFree;


	//�ؽ��� ����
	UINT numTex = boneMesh->DiffuseTexs.size();
	for (UINT i = 0; i < numTex; i++)
	{
		if (boneMesh->pMaterials[i].pTextureFilename != NULL)
		{
			SAFE_DELETE_ARR(boneMesh->pMaterials[i].pTextureFilename);
		}
	}


	SAFE_DELETE_ARR(boneMesh->Name);
	SAFE_DELETE_ARR(boneMesh->pMaterials);
	SAFE_DELETE_ARR(boneMesh->pAdjacency);

	SAFE_RELEASE(boneMesh->MeshData.pMesh);
	SAFE_RELEASE(boneMesh->WorkingMesh);
	SAFE_RELEASE(boneMesh->pSkinInfo);

	SAFE_DELETE_ARR(boneMesh->ppBoneMatrixPtrs);
	SAFE_DELETE_ARR(boneMesh->pBoneOffsetMatices);
	SAFE_RELEASE(boneMesh->BufBoneCombos);

	SAFE_DELETE(boneMesh);

	return S_OK;
}
