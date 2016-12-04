#include "stdafx.h"
#include "cTerrain.h"
#include "cQuadTree.h"
#include "cCamera.h"
#include "cLight_Direction.h"

cTerrain::cTerrain()
{
}


cTerrain::~cTerrain()
{
}

//초기화 
HRESULT cTerrain::Init(
	const char* szHeightMapName,		//높이맵 이름
	const char* szTile_0,				// 타일 이미지 0
	const char* szTile_1,				// 타일 이미지 1
	const char* szTile_2,				// 타일 이미지 2
	const char* szTile_3,				// 타일 이미지 3
	const char* szTileSplat,			// 타일 이미지 컨트롤 ( RGB Black )
	float cellSize,						// 하나의 셀크기
	float heightScale,					// 높이 스케일
	int smoothLevel,					// 스무싱 처리 횟수 
	int tileNum							// 타일링 갯수
	)
{
	//스케일값 대입
	m_fHeightScale = heightScale;
	m_fCellScale = cellSize;

	//높이맵 불러온다
	m_pHeightMap = RESOURCE_TEXTURE->GetResource(szHeightMapName);
	if (m_pHeightMap == NULL)
		return E_FAIL;

	//높이 맵에 대한 이미지 정보를 불러온다. ( 로딩된 Texture 정보를 얻는다 )
	D3DSURFACE_DESC sd;
	m_pHeightMap->GetLevelDesc(
		0,				//얻어올 밉맵체인 레벨 ( 
		&sd				//얻어올 정보릐 D3DSURFACE_DESC 구조체 포인터
		);

	//가로세로 정점 수를 구한다.
	m_nVerNumX = sd.Width + 1;		//가로 정점갯수는 높이 맵에 가로 해상도 + 1 과 같다. ( 이유는 쿼드트리쓰려면 정점갯수가 2의N승 + 1 이여야 하기 때문에 )
	m_nVerNumZ = sd.Height + 1;		//세로 정점잿수는 높이 맵에 세로 해상도 + 1 과 같다. ( 이유는 쿼드트리쓰려면 정점갯수가 2의N승 + 1 이여야 하기 때문에 )
	m_nTotalVerNum = m_nVerNumX * m_nVerNumZ;		//총 정점 갯수


	//가로세로 셀수
	m_nCellNumX = m_nVerNumX - 1;
	m_nCellNumZ = m_nVerNumZ - 1;
	m_nTotalCellNum = m_nCellNumX * m_nCellNumZ;


	//총 삼각형수는
	m_nTotalTri = m_nTotalCellNum * 2;


	//터레인을 만든다.
	if (FAILED(CreateTerrain(
		smoothLevel,			//스무싱 레벨 ( 이값이 클수록 지형이 부드러워진다 )
		tileNum					//타일 Texture 가 몇개로 나누어질건지 갯수
		)))
	{
		Release();
		return E_FAIL;
	}

	//터레인 크기
	m_fTerrainSizeX = m_nCellNumX * m_fCellScale;
	m_fTerrainSizeZ = m_nCellNumZ * m_fCellScale;

	//터레인 범위
	m_fTerrainStartX = m_pTerrainVertices[0].pos.x;
	m_fTerrainStartZ = m_pTerrainVertices[0].pos.z;
	m_fTerrainEndX = m_pTerrainVertices[m_nTotalVerNum - 1].pos.x;
	m_fTerrainEndZ = m_pTerrainVertices[m_nTotalVerNum - 1].pos.z;

	//쿼드트리를 만든다.
	m_pQuadTree = new cQuadTree;
	m_pQuadTree->Init(m_pTerrainVertices, m_nVerNumX);


	//터레인 Texture 로딩
	m_pTexTile_0 = RESOURCE_TEXTURE->GetResource(szTile_0);
	m_pTexTile_1 = RESOURCE_TEXTURE->GetResource(szTile_1);
	m_pTexTile_2 = RESOURCE_TEXTURE->GetResource(szTile_2);
	m_pTexTile_3 = RESOURCE_TEXTURE->GetResource(szTile_3);
	m_pTexSlat = RESOURCE_TEXTURE->GetResource(szTileSplat);

	//Terrain Effect 로딩
	m_pTerrainEffect = RESOURCE_FX->GetResource("../../Resources/Shaders/TerrainBase.fx");


}


//해제
void cTerrain::Release()
{
	SAFE_RELEASE(m_pTerrainVB);
	SAFE_RELEASE(m_pTerrainIB);
	SAFE_RELEASE(m_pTerrainDecl);
	SAFE_DELETE_ARR(m_pTerrainVertices);

	SAFE_DELETE(m_pQuadTree);
}


//랜더
void cTerrain::Render(cCamera* pCam, cLight_Direction* pDirectionLight)
{
	//월드 행렬셋팅
	D3DXMATRIXA16 matInd;
	D3DXMatrixIdentity(&matInd);
	m_pTerrainEffect->SetMatrix("matWorld", &matInd);

	//뷰 행렬셋팅
	m_pTerrainEffect->SetMatrix("matViewProjection", &pCam->GetViewProjectionMatrix());

	//Texture 셋팅
	m_pTerrainEffect->SetTexture("Terrain0_Tex", m_pTexTile_0);
	m_pTerrainEffect->SetTexture("Terrain1_Tex", m_pTexTile_1);
	m_pTerrainEffect->SetTexture("Terrain2_Tex", m_pTexTile_2);
	m_pTerrainEffect->SetTexture("Terrain3_Tex", m_pTexTile_3);
	m_pTerrainEffect->SetTexture("TerrainControl_Tex", m_pTexSlat);

	//광원 셋팅
	D3DXVECTOR3 dirLight = pDirectionLight->pTransform->GetForward();
	m_pTerrainEffect->SetVector("worldLightDir", &D3DXVECTOR4(dirLight, 1));

	UINT passNum = 0;
	m_pTerrainEffect->Begin(&passNum, 0);

	for (UINT i = 0; i < passNum; i++){

		m_pTerrainEffect->BeginPass(i);

		Device->SetStreamSource(0, m_pTerrainVB, 0, sizeof(TERRAINVERTEX));
		Device->SetVertexDeclaration(this->m_pTerrainDecl);
		Device->SetIndices(m_pTerrainIB);
		Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_nTotalVerNum, 0, m_nTotalTri);

		m_pTerrainEffect->EndPass();

	}
	m_pTerrainEffect->End();

}

//Ray 히트 위치를 얻는다.
bool cTerrain::IsIntersectRay(D3DXVECTOR3* pOut, const LPRay pRay)
{
	vector<D3DXVECTOR3> hits;

	//최상단 쿼드 트리부터 Ray Check 들어간다.
	this->m_pQuadTree->GetRayHits(&hits, pRay);

	//히트된게 없다
	if (hits.size() == 0)
		return false;

	//먼저 처음 위치와 처음 위치에대한 거리 대입
	D3DXVECTOR3 hitPos = hits[0];
	D3DXVECTOR3 dir = hitPos - pRay->origin;
	float distSq = D3DXVec3LengthSq(&dir);

	for (int i = 1; i < hits.size(); ++i)
	{
		dir = hits[i] - pRay->origin;
		float newDist = D3DXVec3LengthSq(&dir);

		if (newDist < distSq)
		{
			distSq = newDist;
			hitPos = hits[i];
		}
	}


	//여기까지온다면 제일 거리가 가까운 Hit 위치가 나온다
	*pOut = hitPos;

	return true;
}

//해당 X, Z 위치의 지형의 높이를 얻는다.
float cTerrain::GetHeight(float x, float z)
{
	//터레인 범위을 넘어가면 0.0 값을 리턴한다
	if (x < m_fTerrainStartX || x > m_fTerrainEndX ||
		z > m_fTerrainStartZ || z < m_fTerrainEndZ)
	{
		return 0.0f;
	}

	//Terrain 의 좌상단 0 을 기준으로 월드 Terrain 의 상태적 위치를 찾자
	float pX = x - m_fTerrainStartX;
	float pZ = -(z + m_fTerrainEndZ);

	//해당 위치가 어느 셀에 포함되는지 파악
	float invCell = 1.0f / m_fCellScale;
	pX *= invCell;
	pZ *= invCell;

	//해당 위치의 셀 인덱스
	int idxX = static_cast<int>(pX);
	int idxZ = static_cast<int>(pZ);

	//셀의 네군데 정점을 얻는다.
	// lt-----rt
	//  |    /|
	//  |   / |
	//  |  /  |
	//  | /   |
	//  |/    |
	// lb-----rb

	D3DXVECTOR3 lt = m_pTerrainVertices[idxZ * m_nVerNumX + idxX].pos;
	D3DXVECTOR3 rt = m_pTerrainVertices[idxZ * m_nVerNumX + idxX + 1].pos;
	D3DXVECTOR3 lb = m_pTerrainVertices[(idxZ + 1) * m_nVerNumX + idxX].pos;
	D3DXVECTOR3 rb = m_pTerrainVertices[(idxZ + 1) * m_nVerNumX + idxX + 1].pos;

	//해당 셸에서의 delta 량을 구한다.
	float dX = pX - static_cast<float>(idxX);
	float dZ = pZ - static_cast<float>(idxZ);

	float fHeight = 0.0f;

	//해당 점이 좌상단에 있니?
	if (dX < 1.0f - dZ)
	{
		float deltaU = rt.y - lt.y;	//lt 정점에서 rt 정점까지의 y 위치의 변위량
		float deltaV = lb.y - lt.y; //lt 정점에서 lb 정점까지의 y 위치의 변위량

		fHeight = lt.y + (deltaU * dX) + (deltaV * dZ);
	}

	//해당 점이 우하단이 있니?
	else
	{
		float deltaU = lb.y - rb.y;
		float deltaV = rt.y - rb.y;


		//우에서 좌로 하에서 상으로 보간방향이 바뀌었기 때문에
		//delta 량을 역수로 취한다.
		dX = 1.0f - dX;
		dZ = 1.0f - dZ;

		fHeight = rb.y + (deltaU * dX) + (deltaV * dZ);
	}

	return fHeight;
}

//해당 X, Z 위치의 경사 벡터를 얻는다.
bool cTerrain::GetSlant(D3DXVECTOR3* pOut, float gravityPower, float x, float z)
{
	//터레인 범위을 넘어가면 0.0 값을 리턴한다
	if (x < m_fTerrainStartX || x > m_fTerrainEndX ||
		z > m_fTerrainStartZ || z < m_fTerrainEndZ)
	{
		return false;
	}


	//Terrain 의 좌상단 0 을 기준으로 월드 Terrain 의 상태적 위치를 찾자
	float pX = x - m_fTerrainStartX;
	float pZ = -(z + m_fTerrainEndZ);


	//해당 위치가 어느 셀에 포함되는지 파악
	float invCell = 1.0f / m_fCellScale;
	pX *= invCell;
	pZ *= invCell;


	//해당 위치의 셀 인덱스
	int idxX = static_cast<int>(pX);
	int idxZ = static_cast<int>(pZ);

	//셀의 네군데 정점을 얻는다.
	// lt-----rt
	//  |    /|
	//  |   / |
	//  |  /  |
	//  | /   |
	//  |/    |
	// lb-----rb

	D3DXVECTOR3 lt = m_pTerrainVertices[idxZ * m_nVerNumX + idxX].pos;
	D3DXVECTOR3 rt = m_pTerrainVertices[idxZ * m_nVerNumX + idxX + 1].pos;
	D3DXVECTOR3 lb = m_pTerrainVertices[(idxZ + 1) * m_nVerNumX + idxX].pos;
	D3DXVECTOR3 rb = m_pTerrainVertices[(idxZ + 1) * m_nVerNumX + idxX + 1].pos;

	//해당 셸에서의 delta 량을 구한다.
	float dX = pX - static_cast<float>(idxX);
	float dZ = pZ - static_cast<float>(idxZ);


	float fHeight = 0.0f;

	//폴리곤의 노말 벡터
	D3DXVECTOR3 normal;

	//해당 점이 좌상단에 있니?
	if (dX < 1.0f - dZ)
	{
		//해당폴리곤의 법선 벡터를 구한다.
		D3DXVECTOR3 edge1 = rt - lt;
		D3DXVECTOR3 edge2 = lb - lt;
		D3DXVec3Cross(&normal, &edge1, &edge2);
	}

	//해당 점이 우하단이 있니?
	else
	{
		//해당폴리곤의 법선 벡터를 구한다.
		D3DXVECTOR3 edge1 = rt - lb;
		D3DXVECTOR3 edge2 = rb - lb;
		D3DXVec3Cross(&normal, &edge1, &edge2);
	}

	//노말은 정규화
	D3DXVec3Normalize(&normal, &normal);

	//중력 방향
	D3DXVECTOR3 gravity(0, -gravityPower, 0);

	//경사면의 우측
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &normal, &gravity);

	//우측벡터에서 법선벡터를 외적한 결과가 경사면의 경사 방향이 된다.
	D3DXVec3Cross(pOut, &right, &normal);

	return true;
}

//터레인 정점 만들기
HRESULT cTerrain::CreateTerrain(int smooth, int tileNum)
{
	//
	// 정점 위치 구한다. ------------------------------------------------------------
	//
	//타일링 갯수에 따른 간격 ( 정점당 uv 간격 )
	float tileIntervalX = static_cast<float>(tileNum) / m_nCellNumX;
	float tileIntervalY = static_cast<float>(tileNum) / m_nCellNumZ;


	//터레인 정점 가지고 있어야 한다.
	m_pTerrainVertices = new TERRAINVERTEX[m_nTotalVerNum];


	//텍스쳐의 pixel 정보를 얻기 위해 Texture 를 lock 한다.
	D3DLOCKED_RECT lockRect;
	m_pHeightMap->LockRect(
		0,					//lock 을 할 밉맵 레벨 0
		&lockRect,			//lock 으로 얻어온 D3DLOCKED_RECT 정보 구조체
		0,					//lock 을 일부 영역만 하고 싶은 경우 사각형RECT 구조체의 포인터를 주는데 전체 할꺼면 NULL
		0					//lock 플레그 일단 0
		);

	//lockRect->Pitch	lock 을 한 영역 이미지의 가로 byte 크기 ( 얻어온 바이트크기는 다음을 성립한다 pitch % 4 == 0 ) < 3byte 컬러시 pitch byte 구하는 공식 ( 가로 픽셀수 * 3 + 3 ) & ~3 = pitch  >
	//lockRect->pBits	이미지데이터가 시작되는 포인터 주소

	//정정위치와 정점 UV 를 계산했음....
	for (int z = 0; z < m_nVerNumZ; z++){
		for (int x = 0; x < m_nVerNumX; x++){

			D3DXVECTOR3 pos;

			//정점의 x, z 위치 계산
			pos.x = (x - (m_nCellNumX * 0.5)) * m_fCellScale;
			pos.z = (-z + (m_nCellNumZ * 0.5)) * m_fCellScale;

			//가로마지막 라인이라면 ( 이전 왼쪽의 정점 Y 위치와 맞춘다 )
			if (x == m_nVerNumX - 1){
				int idx = z * m_nVerNumX + x - 1;
				pos.y = m_pTerrainVertices[idx].pos.y;
			}

			//세로 마지막 라인이라면 ( 이전 위쪽의 정점 Y 위치와 맞춘다 )
			else if (z == m_nVerNumZ - 1){
				int idx = (z - 1) * m_nVerNumX + x;
				pos.y = m_pTerrainVertices[idx].pos.y;
			}

			else
			{
				//해당 픽셀의 컬러 값을 얻는다.
				DWORD* pStart = (DWORD*)lockRect.pBits;	//(DWORD 형으로 형변환된 lock 된 이미지지의 시작 주소
				DWORD dwColor = *(pStart + (z * (lockRect.Pitch / 4) + x));

				//각 컬러 값을 0 ~ 1 사이의 실수로 나눈다.
				float inv = 1.0f / 255.0f;
				float r = ((dwColor & 0x00ff0000) >> 16) * inv;
				float g = ((dwColor & 0x0000ff00) >> 8) * inv;
				float b = ((dwColor & 0x000000ff)) * inv;

				//높이 맵 값
				float factor = (r + g + b) / 3.0f;

				//높이 값
				pos.y = factor * m_fHeightScale;
			}


			//정점 UV 계산

			//Terrain Tile Splating UV
			D3DXVECTOR2 baseUV;
			baseUV.x = x / static_cast<float>(m_nVerNumX - 1);
			baseUV.y = z / static_cast<float>(m_nVerNumZ - 1);

			//Terrain Tile UV
			D3DXVECTOR2 tileUV;
			tileUV.x = x * tileIntervalX;
			tileUV.y = z * tileIntervalY;

			//버텍스 배열인덱스 계산
			int idx = z * m_nVerNumX + x;

			m_pTerrainVertices[idx].pos = pos;
			m_pTerrainVertices[idx].normal = D3DXVECTOR3(0, 0, 0);	//아래에서 정점 노말 구할때 더해지니 일단 0 벡터로 초기화
			m_pTerrainVertices[idx].baseUV = baseUV;
			m_pTerrainVertices[idx].tileUV = tileUV;
		}
	}

	//텍스쳐의 pixel 정보 Unlock
	m_pHeightMap->UnlockRect(0);


	//터레인 스무싱 
	SmoothTerrain(smooth);


	//
	// 정점 인덱스를 구한다.....
	//
	LPTERRAINTRI pIndices = new tagTERRAINTRI[m_nTotalTri];

	//인덱스 배열 인덱스
	int idx = 0;

	for (DWORD z = 0; z < m_nCellNumZ; z++)
	{
		for (DWORD x = 0; x < m_nCellNumX; x++){

			// lt-----rt
			//  |    /|
			//  |   / |
			//  |  /  |
			//  | /   |
			//  |/    |
			// lb-----rb

			//해당 셀에 대한 정점 인덱스를 얻자
			DWORD lt = z * m_nVerNumX + x;
			DWORD rt = z * m_nVerNumX + x + 1;
			DWORD lb = ((z + 1)* m_nVerNumX) + x;
			DWORD rb = ((z + 1)* m_nVerNumX) + x + 1;

			//셀의 삼각형 하나
			pIndices[idx].dw0 = lt;
			pIndices[idx].dw1 = rt;
			pIndices[idx].dw2 = lb;
			idx++;

			//셀의 삼각형 하나
			pIndices[idx].dw0 = lb;
			pIndices[idx].dw1 = rt;
			pIndices[idx].dw2 = rb;
			idx++;
		}
	}

	//
	// 노말이랑 Binormal 이랑 Tangent 계산하자...
	//
	D3DXVECTOR3* poses = new D3DXVECTOR3[m_nTotalVerNum];		//정점위치
	D3DXVECTOR3* normals = new D3DXVECTOR3[m_nTotalVerNum];
	D3DXVECTOR3* tangents = new D3DXVECTOR3[m_nTotalVerNum];
	D3DXVECTOR3* binormals = new D3DXVECTOR3[m_nTotalVerNum];
	D3DXVECTOR2* uvs = new D3DXVECTOR2[m_nTotalVerNum];
	DWORD* indices = (DWORD*)pIndices;

	//정점위치 및 UV 대입
	for (int i = 0; i < this->m_nTotalVerNum; i++)
	{
		poses[i] = this->m_pTerrainVertices[i].pos;
		uvs[i] = this->m_pTerrainVertices[i].baseUV;
	}


	//노말계산
	ComputeNormal(normals, poses, m_nTotalVerNum, indices, m_nTotalTri * 3);

	//탄젠트 바이노말 계산
	ComputeTangentAngBinormal(
		tangents,
		binormals,
		poses,
		normals,
		uvs,
		indices,
		m_nTotalTri,
		m_nTotalVerNum);

	//계산된거 대입
	for (int i = 0; i < this->m_nTotalVerNum; i++)
	{
		this->m_pTerrainVertices[i].normal = normals[i];
		this->m_pTerrainVertices[i].binormal = binormals[i];
		this->m_pTerrainVertices[i].tangent = tangents[i];
	}

	//
	// 버퍼 생성
	//
	//정점버퍼 만든다.
	Device->CreateVertexBuffer(
		sizeof(TERRAINVERTEX) * m_nTotalVerNum,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&m_pTerrainVB,
		0);

	//만들어진 정점 버퍼를 Lock 하여 터레인 정점 값을 쓴다.
	void* p = NULL;
	m_pTerrainVB->Lock(0, 0, &p, 0);
	memcpy(p, m_pTerrainVertices, sizeof(TERRAINVERTEX) * m_nTotalVerNum);
	m_pTerrainVB->Unlock();

	//인덱스 버퍼 만든다.
	Device->CreateIndexBuffer(
		sizeof(TERRAINTRI) * m_nTotalTri,
		D3DUSAGE_WRITEONLY,				//D3DUSAGE_WRITEONLY 예는 락안하고 고정정으로 버퍼내용이 유지될때 최고성능을 발휘, D3DUSAGE_DYNAMIC 락 을 자주하여 버퍼 내용이 바뀌는 경우
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,				//D3DUSAGE_DYNAMIC 을 쓰려면 무조건 D3DPOOL_DEFAULT
		&m_pTerrainIB,
		0);

	//인덱스 버퍼에 값을 쓴다.
	m_pTerrainIB->Lock(0, 0, &p, 0);
	memcpy(p, pIndices, sizeof(TERRAINTRI) * m_nTotalTri);
	m_pTerrainIB->Unlock();


	//
	// 정점 선언부를 만든다.
	//

	//정점의 형태를 알려주는 배열
	D3DVERTEXELEMENT9 vertElement[7];			//배열을 정점정보 갯수 + 1

	/*

	//Terrain 정점 구조체
	typedef struct tagTERRAINVERTEX{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 binormal;
	D3DXVECTOR3 tangent;
	D3DXVECTOR2 baseUV;			//0~1 UV
	D3DXVECTOR2 tileUV;			//타일 UV
	}TERRAINVERTEX, *LPTERRAINVERTEX;
	*/


	//Position 
	vertElement[0].Stream = 0;							//Stream 은 0
	vertElement[0].Offset = 0;							//메모리의 시작 Byte 단위 0
	vertElement[0].Type = D3DDECLTYPE_FLOAT3;			//자료의 타입
	vertElement[0].Method = D3DDECLMETHOD_DEFAULT;		//일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[0].Usage = D3DDECLUSAGE_POSITION;		//정보 타입
	vertElement[0].UsageIndex = 0;						//UsageIndex 일단 0


	//Normal
	vertElement[1].Stream = 0;					//Stream 은 0
	vertElement[1].Offset = 12;					//메모리의 시작 Byte 단위 0
	vertElement[1].Type = D3DDECLTYPE_FLOAT3;	//자료의 타입
	vertElement[1].Method = D3DDECLMETHOD_DEFAULT;		//일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[1].Usage = D3DDECLUSAGE_NORMAL;		//정보 타입
	vertElement[1].UsageIndex = 0;						//UsageIndex 일단 0

	//BiNormal
	vertElement[2].Stream = 0;					//Stream 은 0
	vertElement[2].Offset = 24;					//메모리의 시작 Byte 단위 0
	vertElement[2].Type = D3DDECLTYPE_FLOAT3;	//자료의 타입
	vertElement[2].Method = D3DDECLMETHOD_DEFAULT;		//일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[2].Usage = D3DDECLUSAGE_BINORMAL;		//정보 타입
	vertElement[2].UsageIndex = 0;						//UsageIndex 일단 0

	//Tangent
	vertElement[3].Stream = 0;					//Stream 은 0
	vertElement[3].Offset = 36;					//메모리의 시작 Byte 단위 0
	vertElement[3].Type = D3DDECLTYPE_FLOAT3;	//자료의 타입
	vertElement[3].Method = D3DDECLMETHOD_DEFAULT;		//일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[3].Usage = D3DDECLUSAGE_TANGENT;		//정보 타입
	vertElement[3].UsageIndex = 0;						//UsageIndex 일단 0


	//BaseUV
	vertElement[4].Stream = 0;						//Stream 은 0
	vertElement[4].Offset = 48;						//메모리의 시작 Byte 단위 0
	vertElement[4].Type = D3DDECLTYPE_FLOAT2;		//자료의 타입
	vertElement[4].Method = D3DDECLMETHOD_DEFAULT;		//일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[4].Usage = D3DDECLUSAGE_TEXCOORD;		//정보 타입
	vertElement[4].UsageIndex = 0;						//UsageIndex 일단 0


	//tileUV
	vertElement[5].Stream = 0;						//Stream 은 0
	vertElement[5].Offset = 56;						//메모리의 시작 Byte 단위 0
	vertElement[5].Type = D3DDECLTYPE_FLOAT2;		//자료의 타입
	vertElement[5].Method = D3DDECLMETHOD_DEFAULT;		//일단 무조건  D3DDECLMETHOD_DEFAULT
	vertElement[5].Usage = D3DDECLUSAGE_TEXCOORD;		//정보 타입
	vertElement[5].UsageIndex = 1;						//UsageIndex 두번째 UV 1 

	//더이상 없어
	D3DVERTEXELEMENT9 end = D3DDECL_END(); //{0xFF,0,D3DDECLTYPE_UNUSED,0,0,0}
	vertElement[6] = end;


	//
	// LPDIRECT3DVERTEXDECLARATION9 생성
	//
	Device->CreateVertexDeclaration(
		vertElement,			//앞에서 만든 D3DVERTEXELEMENT9 배열 포인터
		&m_pTerrainDecl			//얻어올 LPDIRECT3DVERTEXDECLARATION9 포인터
		);


	SAFE_DELETE_ARR(poses);
	SAFE_DELETE_ARR(normals);
	SAFE_DELETE_ARR(tangents);
	SAFE_DELETE_ARR(binormals);
	SAFE_DELETE_ARR(uvs);
	SAFE_DELETE_ARR(indices);

	return S_OK;
}

//터레인 스무싱
void cTerrain::SmoothTerrain(int passed)
{
	if (passed <= 0) return;


	float* smooth = new float[m_nTotalVerNum];

	while (passed > 0){

		passed--;

		for (int z = 0; z < m_nVerNumZ; z++){
			for (int x = 0; x < m_nVerNumX; x++){

				int adjacentSections = 0;		//몇개의 정점과 평균값을 내니?
				float totalSections = 0.0f;		//주변의 정점 높이 총합은 얼마니?


				//왼쪽체크
				if ((x - 1) > 0){
					totalSections += m_pTerrainVertices[(z * m_nVerNumX) + (x - 1)].pos.y;
					adjacentSections++;

					//왼쪽 상단
					if ((z - 1) > 0){
						totalSections += m_pTerrainVertices[((z - 1) * m_nVerNumX) + (x - 1)].pos.y;
						adjacentSections++;
					}

					//왼쪽 하단
					if ((z + 1) < m_nVerNumZ){
						totalSections += m_pTerrainVertices[((z + 1) * m_nVerNumX) + (x - 1)].pos.y;
						adjacentSections++;
					}
				}

				//오른쪽 체크
				if ((x + 1) < m_nVerNumX){
					totalSections += m_pTerrainVertices[(z * m_nVerNumX) + (x + 1)].pos.y;
					adjacentSections++;

					//오른쪽 상단
					if ((z - 1) > 0){
						totalSections += m_pTerrainVertices[((z - 1) * m_nVerNumX) + (x + 1)].pos.y;
						adjacentSections++;
					}

					//오른쪽 하단 
					if ((z + 1) < m_nVerNumZ){
						totalSections += m_pTerrainVertices[((z + 1) * m_nVerNumX) + (x + 1)].pos.y;
						adjacentSections++;
					}
				}


				//상단
				if ((z - 1) > 0)
				{
					totalSections += m_pTerrainVertices[((z - 1) * m_nVerNumX) + x].pos.y;
					adjacentSections++;
				}

				//하단
				if ((z + 1) < m_nVerNumZ)
				{
					totalSections += m_pTerrainVertices[((z + 1) * m_nVerNumX) + x].pos.y;
					adjacentSections++;
				}
				smooth[(z * m_nVerNumX) + x] = (
					m_pTerrainVertices[(z * m_nVerNumX) + x].pos.y +
					(totalSections / adjacentSections)) * 0.5f;
			}
		}


		//위에서 계산된 y 스무싱 적용
		for (int i = 0; i < m_nTotalVerNum; i++){
			m_pTerrainVertices[i].pos.y = smooth[i];
		}
	}

	SAFE_DELETE_ARR(smooth);
}
