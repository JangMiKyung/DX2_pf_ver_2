#pragma once

class cXMesh_Skinned;

/*
//Hierachy 구조로 되어있는 본 프레임 정보
typedef struct _D3DXFRAME
{
LPSTR                   Name;						//프레임 이름
D3DXMATRIX              TransformationMatrix;		//변환 행렬 ( 에니메이션이 진행되면 에니메이션에 따른 본에 행렬정보가 갱신된다 )

LPD3DXMESHCONTAINER     pMeshContainer;				//해당 본에 영향을 받게 되는 메시 데이터 ( 여기에 BONE_MESH가 참조될 예정 )

struct _D3DXFRAME       *pFrameSibling;				//이웃 노드
struct _D3DXFRAME       *pFrameFirstChild;			//첫번째 자식노드 ( 첫번째 자식노드로 부터 계속해서 내려가는 링크드 리스트 구조로 되어있다 )
} D3DXFRAME, *LPD3DXFRAME;
*/

struct BONE : public D3DXFRAME
{
	D3DXMATRIXA16 CombinedTransformationMatrix;
	cTransform* pBoneTransform;
	cTransform* pApplyTransform;
};



/*
//각 프레임에 물려있는 메시데이터 정보
typedef struct _D3DXMESHCONTAINER
{
LPSTR                   Name;					//메시의 데이터 이름

D3DXMESHDATA            MeshData;				//메시 데이터

LPD3DXMATERIAL          pMaterials;				//메시의 재질 배열 포인터
LPD3DXEFFECTINSTANCE    pEffects;				//메시의 Effect
DWORD                   NumMaterials;			//메시의 재질수
DWORD                  *pAdjacency;				//메시의 인접 정보

LPD3DXSKININFO          pSkinInfo;				//메시의 스키닝 정보

struct _D3DXMESHCONTAINER *pNextMeshContainer;	//하나의 프레임에 다수의 메시데이터가 물려있는 경우 다음 메시데이터의 정보
} D3DXMESHCONTAINER, *LPD3DXMESHCONTAINER;
*/



//
// D3DXMESHCONTAINER 를 상속받은 BONE_MESH 구조체 ( 본에 물리는 메시 정보 )
//
struct BONE_MESH : public D3DXMESHCONTAINER
{
	//LPD3DXMESH						OriginalMesh;			//변환전 메시 데이터

	LPD3DXMESH							WorkingMesh;			//정점셰이더를 사용하기위해 최대 팔래트수 MATRIX_PALETTE_SIZE 대로 서브셋으로 나눠진 메쉬 
	std::vector<D3DMATERIAL9>			Materials;				//메시의 재질 데이터
	std::vector<LPDIRECT3DTEXTURE9>		DiffuseTexs;			//로드된 XFile 에서 사용되는 Texture 들
	std::vector<LPDIRECT3DTEXTURE9>		NormalTexs;
	std::vector<LPDIRECT3DTEXTURE9>		SpecularTexs;
	std::vector<LPDIRECT3DTEXTURE9>		EmissionTexs;

	//DWORD								NumAttributesGroup;		//메시의 속성 그룹수 ( 해당 메시에 
	//D3DXATTRIBUTERANGE*				AttributeTable;			//메시의 속성 테이블 ( 해당 본에 적용된 Mesh 의 Subset, MaterialID 같은 정보를 담고 있다 )

	D3DXMATRIXA16**						ppBoneMatrixPtrs;		//본들의 행렬 [ 포인터 배열 ]
	D3DXMATRIXA16*						pBoneOffsetMatices;		//자신의 기본 행렬 배열
	//D3DXMATRIXA16*					pCurrentBoneMatrices;	//현제 Animation 이 적용된 행렬

	DWORD								NumPaletteEntries;		//현본메쉬의 행렬팔래트 수
	DWORD								MaxNumFaceInfls;		//해당 메시에 적용되는 정점하나당 최대 가중치 갯수
	DWORD								NumAttributesGroup;		//메시의 속성 그룹수 ( 해당 메시에 메터리얼정보가 몇개있니? )
	LPD3DXBUFFER						BufBoneCombos;			//본컴비네이션 ( 메시에 적용되는 본 ID 정보와 메터리얼 정보 )
};


//
// ID3DXAllocateHierarchy 클레스( 인터페이스) 를 상속받는 BONE_HIRERARCHY 클레스
// ID3DXAllocateHierarchy 는 순수가상 멤버함수로 되어있어
// 해당 맴버 함수를 작동하는 것에 대한 재정의를 목표로 한다
//
class BONE_HIERARCHY : public ID3DXAllocateHierarchy
{
private:
	std::string		m_pFileName;		//로딩되는 파일 명
	std::string		m_pFilePath;		//로딩되는 파일 경로

	cXMesh_Skinned*  m_pSkinnedMesh;		//자신이 로딩하는 스킨드 메시

public:
	void SetLoadPath(std::string filePath);
	void SetSkinnedMesh(cXMesh_Skinned* pSkinnedMesh);

public:
	//
	// 빈 프레임 ( 본노드 ) 할당
	//
	//virtual HRESULT CreateFrame( THIS_ LPCSTR Name,LPD3DXFRAME* ppNewFrame )	override	
	STDMETHOD(CreateFrame)(THIS_
		LPCSTR Name,					//프레임 이름 ( 불려려 들어온 본 플레임의 이름이 들어온다 )
		LPD3DXFRAME* ppNewFrame			//새롭게 할당되어 나가는 프레임 ( 우리는 여기에다 BONE 할당하여 매개변수로 받은 이름을 복사하여 할당시켜주어야 한다 )
		) override;


	//
	// 빈프레임( 본노드 ) 에 메시를 할당
	//
	//virtual HRESULT CreateMeshContainer( THIS_ LPCSTR Name,LPD3DXFRAME* ppNewFrame )	override
	STDMETHOD(CreateMeshContainer)(THIS_
		LPCSTR Name,										//할당 되는 메시의 이름
		CONST D3DXMESHDATA* pMeshData,						//할당 되는 메시의 데이터 
		CONST D3DXMATERIAL* pMaterials,						//할당 되는 메시의 메터리얼
		CONST D3DXEFFECTINSTANCE* pEffectInstance,			//Effect 를 사용하는 메시인 경우 Effect 인스턴스 
		DWORD NumMaterials,									//할당 되는 메시의 재질수 
		CONST DWORD* pAdjacency,							//할당 되는 메시의 인접 정보 버퍼
		LPD3DXSKININFO pSkinInfo,							//할당 되는 메시의 스키닝 정보 ( 본의 움직임에 따른 스키닝 정보 )
		LPD3DXMESHCONTAINER* ppNewMeshContainer				//새롭게 할당되어 나가는 메시 ( 우리는 여기에다 위의 매개변수로 받은 정보들로 BONE_MESH 를 할당하여 전달 해 주어야 한다 )
		) override;


	//
	// 프레임 ( 본노드 ) 을 제거
	//
	STDMETHOD(DestroyFrame)(THIS_
		LPD3DXFRAME pFrameToFree							//해제될 본프레임의 주소( 실제 CreateFrame 에서 할당된 BONE 객체의 주소가 들어온다 ) 
		) override;


	//
	// 프레임 ( 본노드 ) 메시데이터를 제거
	//
	STDMETHOD(DestroyMeshContainer)(THIS_
		LPD3DXMESHCONTAINER pMeshContainerToFree			//해제될 본메시 데이터의 주소 ( 실제 CreateMeshContainer 에서 할당된 BONE_MESH 객체의 주소가 들어온다 ) 
		) override;

};

