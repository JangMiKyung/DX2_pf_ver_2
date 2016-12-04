#include "stdafx.h"
#include "cTerrain.h"


cTerrain::cTerrain()
{
}


cTerrain::~cTerrain()
{
}

//초기화
HRESULT cTerrain::Init(int vertexNum, float cellSize)
{
	//정점수 
	this->nVerNumX = vertexNum;
	this->nVerNumZ = vertexNum;
	this->nTotalVerNum = this->nVerNumX * this->nVerNumZ;

	//셀수
	this->nCellNumX = this->nVerNumX - 1;
	this->nCellNumZ = this->nVerNumZ - 1;
	this->nTotalCellNum = this->nCellNumX * this->nCellNumZ;

	//셀사이즈
	this->fCellScale = cellSize;

	//폴리곤 갯수
	this->nTotalTri = this->nTotalCellNum * 2;

	//터레인 사이즈
	this->fTerrainSizeX = this->nCellNumX * cellSize;
	this->fTerrainSizeZ = this->nCellNumZ * cellSize;


	//
	// 터레인 정보 생성
	//
	//정점 수대로 정점 위치 정보를 만든다.
	vertexPoses = new D3DXVECTOR3[nTotalVerNum];

	//시작 지점은 일단 0. 0. 0  위치로
	D3DXVECTOR3 startPos(0, 0, 0);

	for (int z = 0; z < this->nVerNumZ; z++)
	{
		for (int x = 0; x < this->nVerNumX; x++)
		{
			//정점인덱스
			int idx = z * this->nVerNumX + x;

			//정점위치
			vertexPoses[idx].x = startPos.x + (x * fCellScale);
			vertexPoses[idx].z = startPos.z + (z * fCellScale);
			vertexPoses[idx].y = cos(x * 10 * ONE_RAD) * 3.0f;
		}
	}



	//
	// 정점인덱스
	//
	pIndices = new TERRAININDEX[this->nTotalTri];

	for (int z = 0; z < this->nCellNumZ; z++)
	{
		//수평라인
		int nowZ = z;
		int nextZ = z + 1;

		for (int x = 0; x < this->nCellNumX; x++)
		{
			//현제 수직라인
			int nowX = x;
			int nextX = x + 1;

			// lt-----rt
			//  |    /|
			//  |   / |
			//  |  /  |
			//  | /   |
			//  |/    |
			// lb-----rb

			//모서리 정점 인덱스
			DWORD lt = nextZ * this->nVerNumX + nowX;
			DWORD rt = nextZ * this->nVerNumX + nextX;
			DWORD lb = nowZ * this->nVerNumX + nowX;
			DWORD rb = nowZ * this->nVerNumX + nextX;


			//Tri 인덱스 배열 인덱스
			int idx = (z * this->nCellNumX + x) * 2;


			//Cell 폴리곤 1
			//lb, lt, rt, 
			pIndices[idx]._0 = lb;
			pIndices[idx]._1 = lt;
			pIndices[idx]._2 = rt;

			//Cell 폴리곤 1
			//lb, rt, rb, 
			pIndices[idx + 1]._0 = lb;
			pIndices[idx + 1]._1 = rt;
			pIndices[idx + 1]._2 = rb;
		}
	}

	//
	// 정점 구조체 선언
	//
	/*

	//Position
	vertElement[0].Stream = 0;							//Stream 은 0
	vertElement[0].Offset = 0;							//메모리의 시작 Byte 단위 0
	vertElement[0].Type = D3DDECLTYPE_FLOAT3;			//자료의 타입
	vertElement[0].Method = D3DDECLMETHOD_DEFAULT;		//일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[0].Usage = D3DDECLUSAGE_POSITION;		//정보 타입
	vertElement[0].UsageIndex = 0;						//UsageIndex 일단 0
	*/

	D3DVERTEXELEMENT9 vertElement[] = {
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		D3DDECL_END()
	};

	Device->CreateVertexDeclaration(
		vertElement,			//앞에서 만든 D3DVERTEXELEMENT9 배열 포인터
		&this->terrainDecl			//얻어올 LPDIRECT3DVERTEXDECLARATION9 포인터
		);



	//
	// 정점 버퍼를 만든다.
	//
	if (FAILED(Device->CreateVertexBuffer(
		sizeof(TERRAINVERTEX) * this->nTotalVerNum,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_DEFAULT,
		&this->terrainVb,
		0))) return E_FAIL;

	LPTERRAINVERTTEX pVertices = NULL;

	if (SUCCEEDED(this->terrainVb->Lock(0, 0, (void**)&pVertices, 0)))
	{
		for (int i = 0; i < this->nTotalVerNum; i++)
		{
			//정점 위치 대입
			pVertices[i].pos = this->vertexPoses[i];
		}

		this->terrainVb->Unlock();
	}

	//
	// 인덱스 버퍼
	//
	if (FAILED(Device->CreateIndexBuffer(
		sizeof(TERRAININDEX) * this->nTotalTri,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,
		&this->terrainIb,
		NULL))) return E_FAIL;

	void* pI = NULL;

	if (SUCCEEDED(this->terrainIb->Lock(0, 0, &pI, 0)))
	{
		memcpy(pI, this->pIndices, sizeof(TERRAININDEX) * this->nTotalTri);

		this->terrainIb->Unlock();
	}


	return S_OK;
}

//해제
void cTerrain::Release()
{
	SAFE_DELETE_ARR(this->vertexPoses);
	SAFE_DELETE_ARR(this->pIndices);

	SAFE_RELEASE(this->terrainIb);
	SAFE_RELEASE(this->terrainVb);
}

//렌더
void cTerrain::Render()
{
	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	//월드 행렬 일단 초기화
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	Device->SetTransform(D3DTS_WORLD, &matWorld);


	Device->SetStreamSource(0, this->terrainVb, 0, sizeof(TERRAINVERTEX));
	Device->SetIndices(this->terrainIb);
	Device->SetVertexDeclaration(this->terrainDecl);
	Device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,
		0,
		this->nTotalVerNum,
		0,
		this->nTotalTri);

	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}