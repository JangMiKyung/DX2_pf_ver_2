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

		//여기까지온다면 filePath 경로가 포함되어있지 않다
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
	LPCSTR Name,					//프레임 이름 ( 불려려 들어온 본 프레임의 이름이 들어온다 )
	LPD3DXFRAME* ppNewFrame			//새롭게 할당되어 나가는 프레임 ( 우리는 여기에다 BONE 할당하여 매개변수로 받은 이름을 복사하여 할당시켜주어야 한다 )
	)
{
	//ppNewFrame 으로 넘겨줄 새로운 BONE 생성
	BONE*		newBone = new BONE;
	ZeroMemory(newBone, sizeof(BONE));

	//본이름 복사
	if (Name != NULL)
	{
		int len = strlen(Name);
		newBone->Name = new char[len + 1];
		strcpy_s(newBone->Name, len + 1, Name);
	}

	//매트릭스 정보 초기화
	D3DXMatrixIdentity(&newBone->TransformationMatrix);
	D3DXMatrixIdentity(&newBone->CombinedTransformationMatrix);


	newBone->pBoneTransform = NULL;
	newBone->pApplyTransform = NULL;

	//리턴값에 새로운 본 주소 대입
	*ppNewFrame = newBone;

	return S_OK;
}

//
// 빈프레임( 본노드 ) 에 메시를 할당
//
HRESULT BONE_HIERARCHY::CreateMeshContainer(THIS_
	LPCSTR Name,										//할당 되는 메시의 이름
	CONST D3DXMESHDATA* pMeshData,						//할당 되는 메시의 데이터 
	CONST D3DXMATERIAL* pMaterials,						//할당 되는 메시의 메터리얼
	CONST D3DXEFFECTINSTANCE* pEffectInstance,			//Effect 를 사용하는 메시인 경우 Effect 인스턴스 
	DWORD NumMaterials,									//할당 되는 메시의 재질수 
	CONST DWORD* pAdjacency,							//할당 되는 메시의 인접 정보 버퍼
	LPD3DXSKININFO pSkinInfo,							//할당 되는 메시의 스키닝 정보 ( 본의 움직임에 따른 스키닝 정보 )
	LPD3DXMESHCONTAINER* ppNewMeshContainer				//새롭게 할당되어 나가는 메시 ( 우리는 여기에다 위의 매개변수로 받은 정보들로 BONE_MESH 를 할당하여 전달 해 주어야 한다 )
	)
{
	//ppNewMeshContainer 로 넘겨줄 새로운 BONEMESH 생성
	BONE_MESH* boneMesh = new BONE_MESH;
	ZeroMemory(boneMesh, sizeof(BONE_MESH));

	//매개변수로 받은 메쉬 데이터의 Device 를 얻는다.
	LPDIRECT3DDEVICE9 d3dDevice = NULL;
	pMeshData->pMesh->GetDevice(&d3dDevice);

	//메시데이터를 물린다.
	boneMesh->MeshData.pMesh = pMeshData->pMesh;
	boneMesh->MeshData.Type = D3DXMESHTYPE_MESH;

	//메시데이터의 레퍼런스 카운트 증가
	pMeshData->pMesh->AddRef();
	//레퍼런스 카운트를 명시적으로 해주지 않는다면 매개변수로 받은 pMeshData 가 함수 호출후 해제 되어 버린다!
	//즉 함수 호출후에도 내부적으로 참조수가 0 이면 자동으로 해제되는 식이다.
	//따라서 새로만들어진 BONEMESH 맴버데이터에 pMeshData 포인터를 물고있으니
	//레퍼런스 카운트 수를 증가 시켜 함수가 끈난뒤에도 해제를 막는다.

	//이름 복사
	if (Name != NULL)
	{
		int len = strlen(Name);
		boneMesh->Name = new char[len + 1];
		strcpy_s(boneMesh->Name, len + 1, Name);
	}

	//인접정보 복사 ( 인접 정보는 면단 3개씩 지니게 된다 왜냐면 폴리곤의 인접수는 3개니깐 )
	DWORD numAdjacency = 3 * (boneMesh->MeshData.pMesh->GetNumFaces());		//인접정보의 수
	boneMesh->pAdjacency = new DWORD[numAdjacency];							//인접 정보의 수만큼 증가
	memcpy(boneMesh->pAdjacency, pAdjacency, sizeof(DWORD) * numAdjacency);


	std::string texFilePath;
	std::string texExp;			//파일 확장자 명
	std::string texFile;		//파일 명`

	//재질정보 
	std::string texPath;

	boneMesh->NumMaterials = NumMaterials;
	boneMesh->pMaterials = new D3DXMATERIAL[NumMaterials];

	for (DWORD i = 0; i < NumMaterials; i++)
	{
		//메터리얼 정보 복사
		memcpy(&boneMesh->pMaterials[i], &pMaterials[i], sizeof(D3DXMATERIAL));

		//메터리얼정보 푸쉬
		D3DMATERIAL9 mtrl;
		memcpy(&mtrl, &pMaterials[i].MatD3D, sizeof(D3DMATERIAL9));
		mtrl.Ambient = mtrl.Diffuse;		//재질 Ambient
		boneMesh->Materials.push_back(mtrl);

		//Texture 가 존재한다면...
		if (pMaterials[i].pTextureFilename != NULL)
		{
			//이름 깊은 복사
			int len = strlen(pMaterials[i].pTextureFilename);
			boneMesh->pMaterials[i].pTextureFilename = new char[len + 1];
			strcpy_s(boneMesh->pMaterials[i].pTextureFilename, len + 1, pMaterials[i].pTextureFilename);


			texPath = this->m_pFilePath + pMaterials[i].pTextureFilename;

			/*
			//Texture 로딩
			LPDIRECT3DTEXTURE9 pTex;
			if( FAILED( D3DXCreateTextureFromFile(
			d3dDevice, texPath.c_str(), &pTex ) ) ){
			//Texture 푸쉬
			boneMesh->Textures.push_back( NULL );

			}

			else
			{
			//Texture 푸쉬
			boneMesh->Textures.push_back( pTex );
			}*/

			boneMesh->DiffuseTexs.push_back(
				RESOURCE_TEXTURE->GetResource(texPath));



			//파일 이름과 확장자 위치
			int dotIndex = texPath.find_last_of(".");

			//파일 명과 확장자를 나눈다.
			texFile = texPath.substr(0, dotIndex);
			texExp = texPath.substr(dotIndex + 1, texPath.length());

			//
			// 노말말 맵은 _N 이 붙는다.
			//
			texFilePath = texFile + "_N." + texExp;

			LPDIRECT3DTEXTURE9 pNorTex =
				RESOURCE_TEXTURE->GetResource(texFilePath);
			//만약 Texture 가 없다면...
			if (pNorTex == NULL){
				pNorTex = RESOURCE_TEXTURE->GetResource("../Resources/Meshes/normalDefault.png");
			}


			//
			// Emission 맵은 _E 이 붙는다.
			//
			texFilePath = texFile + "_E." + texExp;

			LPDIRECT3DTEXTURE9 pEmiTex =
				RESOURCE_TEXTURE->GetResource(texFilePath);
			//만약 Texture 가 없다면...
			if (pEmiTex == NULL){
				pEmiTex = RESOURCE_TEXTURE->GetResource("../Resources/Meshes/emissionDefault.png");
			}


			//
			// Specular 맵은 _S 이 붙는다.
			//
			texFilePath = texFile + "_S." + texExp;

			LPDIRECT3DTEXTURE9 pSpcTex =
				RESOURCE_TEXTURE->GetResource(texFilePath);
			//만약 Texture 가 없다면...
			if (pSpcTex == NULL){
				pSpcTex = RESOURCE_TEXTURE->GetResource("../Resources/Meshes/specularDefault.png");
			}
			//Texture 푸쉬
			boneMesh->NormalTexs.push_back(pNorTex);
			boneMesh->SpecularTexs.push_back(pSpcTex);
			boneMesh->EmissionTexs.push_back(pEmiTex);
		}

		//Texture 정보가 없다면..
		else
		{
			//Texture의 이름은 존재하지 않는다.
			boneMesh->pMaterials[i].pTextureFilename = NULL;

			//NULL 값이라도 대입해야 한다
			boneMesh->DiffuseTexs.push_back(NULL);
			boneMesh->NormalTexs.push_back(NULL);
			boneMesh->SpecularTexs.push_back(NULL);
			boneMesh->EmissionTexs.push_back(NULL);
		}
	}


	//속성테이블을 얻는다.
	boneMesh->MeshData.pMesh->GetAttributeTable(NULL, &boneMesh->NumAttributesGroup);			//속성 그룹 갯수를 얻는다.

	//boneMesh->MeshData.pMesh->GetAttributeTable( boneMesh->AttributeTable, NULL );				//속성 테이블을 얻는다.


	//스키닝 정보 가 있다면....
	if (pSkinInfo != NULL)
	{
		//스키닝 정보를 물어놓는다
		boneMesh->pSkinInfo = pSkinInfo;

		//스킨정보 레퍼런스 카운트 증가 ( 위의 메시데이터의 레퍼런스 카운트와 동일한 과정 )
		pSkinInfo->AddRef();

		//이메시에 사용되는 본의 수를 얻는다.
		int numBone = pSkinInfo->GetNumBones();

		//사용되는 본의 수만큼 행렬 동적할당
		boneMesh->pBoneOffsetMatices = new D3DXMATRIXA16[numBone];
		//본 Offset 행렬 셋팅
		for (int i = 0; i < numBone; i++)
			boneMesh->pBoneOffsetMatices[i] = *(boneMesh->pSkinInfo->GetBoneOffsetMatrix(i));

		//Effect 에서 사용되는 행렬 팔래트 쵀대 수를 얻는다.
		UINT iPaletteSize = 0;
		m_pSkinnedMesh->m_pSkinnedEffect->GetInt("MATRIX_PALETTE_SIZE", (INT*)&iPaletteSize);

		//기본적으로 적용되는 본수 만큼 팔래트를 할당하는데 Effect 의 최대 MATRIX_PALETTE_SIZE 보다 적용되는 본의 
		//수가 많다면 팔래트 수는 Effect MATRIX_PALETTE_SIZE 수로 고정 한다.
		boneMesh->NumPaletteEntries = min(iPaletteSize, boneMesh->pSkinInfo->GetNumBones());


		//위의 작업에이어 Mesh에  적용되는 bone의 수가 Effect 에서 지원되는 MATRIX_PALETTE_SIZE 수보다 많은 경우
		//boneMesh->NumPaletteEntries 수에 맞추어서 Mesh 의 SubSet 을 나눌 필요가 있다.
		//아래의 함수는 그러한 작업 들을 WorkingMesh 에 해주고 또한 WorkingMesh 에 
		//정점 가중치 개념을 추가해준다.


		boneMesh->pSkinInfo->ConvertToIndexedBlendedMesh(
			boneMesh->MeshData.pMesh,		//원본 메쉬
			D3DXMESH_MANAGED | D3DXMESHOPT_VERTEXCACHE,	//메쉬 옵션
			boneMesh->NumPaletteEntries,		//본 팔래트 수를 넘긴다.
			boneMesh->pAdjacency,			//인접 버퍼를 넘긴다.
			NULL,							//갱신된 인접 버퍼를 받아온다 ( 필요없으면 NULL )
			NULL,							//갱신된 폴리곤(감각인덱스) 버퍼를 받아온다 ( 필요없으면 NULL )
			NULL,							//갱신된 정점 버퍼를 받아온다 ( 필요없으면 NULL )
			&boneMesh->MaxNumFaceInfls,		//한 버텍스에 영향을 주는 본의 최대 갯수를 얻는더.
			&boneMesh->NumAttributesGroup,	//본의 컴비네이션 속성그룹의 수를 얻는다ㅓ.
			&boneMesh->BufBoneCombos,		//본의 컴비네이션 버퍼를 얻는다.
			&boneMesh->WorkingMesh			//원본메쉬에서 작업된 메쉬
			);
		//본의 컴비네이션 은 메쉬의 Subset 별로 어떠한 본에 영향을 받는지에 대한 정보를 지니게 된다.
		//본매트릭스 인덱스 = 본컴비네이션[Subset].BoneID[팔래트]

		//cMesh_Skinned 에 작업에 사용되는 팔래트 행렬 수가 부족하면 
		//다시 재할당 한다.
		if (m_pSkinnedMesh->m_dwWorkingPaletteSize < boneMesh->NumPaletteEntries)
		{
			if (m_pSkinnedMesh->m_pmWorkingPalette)
				delete[] m_pSkinnedMesh->m_pmWorkingPalette;

			m_pSkinnedMesh->m_dwWorkingPaletteSize = boneMesh->NumPaletteEntries;
			m_pSkinnedMesh->m_pmWorkingPalette = new D3DXMATRIXA16[boneMesh->NumPaletteEntries];
		}
	}

	//밖으로 리턴될 LPD3DXMESHCONTAINER 에 본메쉬 대입
	*ppNewMeshContainer = boneMesh;

	//얻어온 Device 해재
	SAFE_RELEASE(d3dDevice);

	return S_OK;

}



//
// 프레임 ( 본노드 ) 을 제거
//
HRESULT BONE_HIERARCHY::DestroyFrame(THIS_
	LPD3DXFRAME pFrameToFree							//해제될 본프레임의 주소( 실제 CreateFrame 에서 할당된 BONE 객체의 주소가 들어온다 ) 
	)
{
	if (pFrameToFree)
	{
		//Transform 작살냄
		//SAFE_DELETE( ( (BONE*)pFrameToFree )->pBoneTransform );

		//이름 해제
		SAFE_DELETE_ARR(pFrameToFree->Name);
		SAFE_DELETE(pFrameToFree);
	}

	return S_OK;
}


//
// 프레임 ( 본노드 ) 메시데이터를 제거
//
HRESULT BONE_HIERARCHY::DestroyMeshContainer(THIS_
	LPD3DXMESHCONTAINER pMeshContainerToFree			//해제될 본메시 데이터의 주소 ( 실제 CreateMeshContainer 에서 할당된 BONE_MESH 객체의 주소가 들어온다 ) 
	)
{
	//boneMesh 로 형변환
	BONE_MESH* boneMesh = (BONE_MESH*)pMeshContainerToFree;


	//텍스쳐 해제
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
