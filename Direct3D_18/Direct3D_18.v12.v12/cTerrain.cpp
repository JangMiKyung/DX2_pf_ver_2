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

//�ʱ�ȭ 
HRESULT cTerrain::Init(
	const char* szHeightMapName,		//���̸� �̸�
	const char* szTile_0,				// Ÿ�� �̹��� 0
	const char* szTile_1,				// Ÿ�� �̹��� 1
	const char* szTile_2,				// Ÿ�� �̹��� 2
	const char* szTile_3,				// Ÿ�� �̹��� 3
	const char* szTileSplat,			// Ÿ�� �̹��� ��Ʈ�� ( RGB Black )
	float cellSize,						// �ϳ��� ��ũ��
	float heightScale,					// ���� ������
	int smoothLevel,					// ������ ó�� Ƚ�� 
	int tileNum							// Ÿ�ϸ� ����
	)
{
	//�����ϰ� ����
	m_fHeightScale = heightScale;
	m_fCellScale = cellSize;

	//���̸� �ҷ��´�
	m_pHeightMap = RESOURCE_TEXTURE->GetResource(szHeightMapName);
	if (m_pHeightMap == NULL)
		return E_FAIL;

	//���� �ʿ� ���� �̹��� ������ �ҷ��´�. ( �ε��� Texture ������ ��´� )
	D3DSURFACE_DESC sd;
	m_pHeightMap->GetLevelDesc(
		0,				//���� �Ӹ�ü�� ���� ( 
		&sd				//���� �����l D3DSURFACE_DESC ����ü ������
		);

	//���μ��� ���� ���� ���Ѵ�.
	m_nVerNumX = sd.Width + 1;		//���� ���������� ���� �ʿ� ���� �ػ� + 1 �� ����. ( ������ ����Ʈ�������� ���������� 2��N�� + 1 �̿��� �ϱ� ������ )
	m_nVerNumZ = sd.Height + 1;		//���� ��������� ���� �ʿ� ���� �ػ� + 1 �� ����. ( ������ ����Ʈ�������� ���������� 2��N�� + 1 �̿��� �ϱ� ������ )
	m_nTotalVerNum = m_nVerNumX * m_nVerNumZ;		//�� ���� ����


	//���μ��� ����
	m_nCellNumX = m_nVerNumX - 1;
	m_nCellNumZ = m_nVerNumZ - 1;
	m_nTotalCellNum = m_nCellNumX * m_nCellNumZ;


	//�� �ﰢ������
	m_nTotalTri = m_nTotalCellNum * 2;


	//�ͷ����� �����.
	if (FAILED(CreateTerrain(
		smoothLevel,			//������ ���� ( �̰��� Ŭ���� ������ �ε巯������ )
		tileNum					//Ÿ�� Texture �� ��� ������������ ����
		)))
	{
		Release();
		return E_FAIL;
	}

	//�ͷ��� ũ��
	m_fTerrainSizeX = m_nCellNumX * m_fCellScale;
	m_fTerrainSizeZ = m_nCellNumZ * m_fCellScale;

	//�ͷ��� ����
	m_fTerrainStartX = m_pTerrainVertices[0].pos.x;
	m_fTerrainStartZ = m_pTerrainVertices[0].pos.z;
	m_fTerrainEndX = m_pTerrainVertices[m_nTotalVerNum - 1].pos.x;
	m_fTerrainEndZ = m_pTerrainVertices[m_nTotalVerNum - 1].pos.z;

	//����Ʈ���� �����.
	m_pQuadTree = new cQuadTree;
	m_pQuadTree->Init(m_pTerrainVertices, m_nVerNumX);


	//�ͷ��� Texture �ε�
	m_pTexTile_0 = RESOURCE_TEXTURE->GetResource(szTile_0);
	m_pTexTile_1 = RESOURCE_TEXTURE->GetResource(szTile_1);
	m_pTexTile_2 = RESOURCE_TEXTURE->GetResource(szTile_2);
	m_pTexTile_3 = RESOURCE_TEXTURE->GetResource(szTile_3);
	m_pTexSlat = RESOURCE_TEXTURE->GetResource(szTileSplat);

	//Terrain Effect �ε�
	m_pTerrainEffect = RESOURCE_FX->GetResource("../../Resources/Shaders/TerrainBase.fx");


}


//����
void cTerrain::Release()
{
	SAFE_RELEASE(m_pTerrainVB);
	SAFE_RELEASE(m_pTerrainIB);
	SAFE_RELEASE(m_pTerrainDecl);
	SAFE_DELETE_ARR(m_pTerrainVertices);

	SAFE_DELETE(m_pQuadTree);
}


//����
void cTerrain::Render(cCamera* pCam, cLight_Direction* pDirectionLight)
{
	//���� ��ļ���
	D3DXMATRIXA16 matInd;
	D3DXMatrixIdentity(&matInd);
	m_pTerrainEffect->SetMatrix("matWorld", &matInd);

	//�� ��ļ���
	m_pTerrainEffect->SetMatrix("matViewProjection", &pCam->GetViewProjectionMatrix());

	//Texture ����
	m_pTerrainEffect->SetTexture("Terrain0_Tex", m_pTexTile_0);
	m_pTerrainEffect->SetTexture("Terrain1_Tex", m_pTexTile_1);
	m_pTerrainEffect->SetTexture("Terrain2_Tex", m_pTexTile_2);
	m_pTerrainEffect->SetTexture("Terrain3_Tex", m_pTexTile_3);
	m_pTerrainEffect->SetTexture("TerrainControl_Tex", m_pTexSlat);

	//���� ����
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

//Ray ��Ʈ ��ġ�� ��´�.
bool cTerrain::IsIntersectRay(D3DXVECTOR3* pOut, const LPRay pRay)
{
	vector<D3DXVECTOR3> hits;

	//�ֻ�� ���� Ʈ������ Ray Check ����.
	this->m_pQuadTree->GetRayHits(&hits, pRay);

	//��Ʈ�Ȱ� ����
	if (hits.size() == 0)
		return false;

	//���� ó�� ��ġ�� ó�� ��ġ������ �Ÿ� ����
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


	//��������´ٸ� ���� �Ÿ��� ����� Hit ��ġ�� ���´�
	*pOut = hitPos;

	return true;
}

//�ش� X, Z ��ġ�� ������ ���̸� ��´�.
float cTerrain::GetHeight(float x, float z)
{
	//�ͷ��� ������ �Ѿ�� 0.0 ���� �����Ѵ�
	if (x < m_fTerrainStartX || x > m_fTerrainEndX ||
		z > m_fTerrainStartZ || z < m_fTerrainEndZ)
	{
		return 0.0f;
	}

	//Terrain �� �»�� 0 �� �������� ���� Terrain �� ������ ��ġ�� ã��
	float pX = x - m_fTerrainStartX;
	float pZ = -(z + m_fTerrainEndZ);

	//�ش� ��ġ�� ��� ���� ���ԵǴ��� �ľ�
	float invCell = 1.0f / m_fCellScale;
	pX *= invCell;
	pZ *= invCell;

	//�ش� ��ġ�� �� �ε���
	int idxX = static_cast<int>(pX);
	int idxZ = static_cast<int>(pZ);

	//���� �ױ��� ������ ��´�.
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

	//�ش� �п����� delta ���� ���Ѵ�.
	float dX = pX - static_cast<float>(idxX);
	float dZ = pZ - static_cast<float>(idxZ);

	float fHeight = 0.0f;

	//�ش� ���� �»�ܿ� �ִ�?
	if (dX < 1.0f - dZ)
	{
		float deltaU = rt.y - lt.y;	//lt �������� rt ���������� y ��ġ�� ������
		float deltaV = lb.y - lt.y; //lt �������� lb ���������� y ��ġ�� ������

		fHeight = lt.y + (deltaU * dX) + (deltaV * dZ);
	}

	//�ش� ���� ���ϴ��� �ִ�?
	else
	{
		float deltaU = lb.y - rb.y;
		float deltaV = rt.y - rb.y;


		//�쿡�� �·� �Ͽ��� ������ ���������� �ٲ���� ������
		//delta ���� ������ ���Ѵ�.
		dX = 1.0f - dX;
		dZ = 1.0f - dZ;

		fHeight = rb.y + (deltaU * dX) + (deltaV * dZ);
	}

	return fHeight;
}

//�ش� X, Z ��ġ�� ��� ���͸� ��´�.
bool cTerrain::GetSlant(D3DXVECTOR3* pOut, float gravityPower, float x, float z)
{
	//�ͷ��� ������ �Ѿ�� 0.0 ���� �����Ѵ�
	if (x < m_fTerrainStartX || x > m_fTerrainEndX ||
		z > m_fTerrainStartZ || z < m_fTerrainEndZ)
	{
		return false;
	}


	//Terrain �� �»�� 0 �� �������� ���� Terrain �� ������ ��ġ�� ã��
	float pX = x - m_fTerrainStartX;
	float pZ = -(z + m_fTerrainEndZ);


	//�ش� ��ġ�� ��� ���� ���ԵǴ��� �ľ�
	float invCell = 1.0f / m_fCellScale;
	pX *= invCell;
	pZ *= invCell;


	//�ش� ��ġ�� �� �ε���
	int idxX = static_cast<int>(pX);
	int idxZ = static_cast<int>(pZ);

	//���� �ױ��� ������ ��´�.
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

	//�ش� �п����� delta ���� ���Ѵ�.
	float dX = pX - static_cast<float>(idxX);
	float dZ = pZ - static_cast<float>(idxZ);


	float fHeight = 0.0f;

	//�������� �븻 ����
	D3DXVECTOR3 normal;

	//�ش� ���� �»�ܿ� �ִ�?
	if (dX < 1.0f - dZ)
	{
		//�ش��������� ���� ���͸� ���Ѵ�.
		D3DXVECTOR3 edge1 = rt - lt;
		D3DXVECTOR3 edge2 = lb - lt;
		D3DXVec3Cross(&normal, &edge1, &edge2);
	}

	//�ش� ���� ���ϴ��� �ִ�?
	else
	{
		//�ش��������� ���� ���͸� ���Ѵ�.
		D3DXVECTOR3 edge1 = rt - lb;
		D3DXVECTOR3 edge2 = rb - lb;
		D3DXVec3Cross(&normal, &edge1, &edge2);
	}

	//�븻�� ����ȭ
	D3DXVec3Normalize(&normal, &normal);

	//�߷� ����
	D3DXVECTOR3 gravity(0, -gravityPower, 0);

	//������ ����
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &normal, &gravity);

	//�������Ϳ��� �������͸� ������ ����� ������ ��� ������ �ȴ�.
	D3DXVec3Cross(pOut, &right, &normal);

	return true;
}

//�ͷ��� ���� �����
HRESULT cTerrain::CreateTerrain(int smooth, int tileNum)
{
	//
	// ���� ��ġ ���Ѵ�. ------------------------------------------------------------
	//
	//Ÿ�ϸ� ������ ���� ���� ( ������ uv ���� )
	float tileIntervalX = static_cast<float>(tileNum) / m_nCellNumX;
	float tileIntervalY = static_cast<float>(tileNum) / m_nCellNumZ;


	//�ͷ��� ���� ������ �־�� �Ѵ�.
	m_pTerrainVertices = new TERRAINVERTEX[m_nTotalVerNum];


	//�ؽ����� pixel ������ ��� ���� Texture �� lock �Ѵ�.
	D3DLOCKED_RECT lockRect;
	m_pHeightMap->LockRect(
		0,					//lock �� �� �Ӹ� ���� 0
		&lockRect,			//lock ���� ���� D3DLOCKED_RECT ���� ����ü
		0,					//lock �� �Ϻ� ������ �ϰ� ���� ��� �簢��RECT ����ü�� �����͸� �ִµ� ��ü �Ҳ��� NULL
		0					//lock �÷��� �ϴ� 0
		);

	//lockRect->Pitch	lock �� �� ���� �̹����� ���� byte ũ�� ( ���� ����Ʈũ��� ������ �����Ѵ� pitch % 4 == 0 ) < 3byte �÷��� pitch byte ���ϴ� ���� ( ���� �ȼ��� * 3 + 3 ) & ~3 = pitch  >
	//lockRect->pBits	�̹��������Ͱ� ���۵Ǵ� ������ �ּ�

	//������ġ�� ���� UV �� �������....
	for (int z = 0; z < m_nVerNumZ; z++){
		for (int x = 0; x < m_nVerNumX; x++){

			D3DXVECTOR3 pos;

			//������ x, z ��ġ ���
			pos.x = (x - (m_nCellNumX * 0.5)) * m_fCellScale;
			pos.z = (-z + (m_nCellNumZ * 0.5)) * m_fCellScale;

			//���θ����� �����̶�� ( ���� ������ ���� Y ��ġ�� ����� )
			if (x == m_nVerNumX - 1){
				int idx = z * m_nVerNumX + x - 1;
				pos.y = m_pTerrainVertices[idx].pos.y;
			}

			//���� ������ �����̶�� ( ���� ������ ���� Y ��ġ�� ����� )
			else if (z == m_nVerNumZ - 1){
				int idx = (z - 1) * m_nVerNumX + x;
				pos.y = m_pTerrainVertices[idx].pos.y;
			}

			else
			{
				//�ش� �ȼ��� �÷� ���� ��´�.
				DWORD* pStart = (DWORD*)lockRect.pBits;	//(DWORD ������ ����ȯ�� lock �� �̹������� ���� �ּ�
				DWORD dwColor = *(pStart + (z * (lockRect.Pitch / 4) + x));

				//�� �÷� ���� 0 ~ 1 ������ �Ǽ��� ������.
				float inv = 1.0f / 255.0f;
				float r = ((dwColor & 0x00ff0000) >> 16) * inv;
				float g = ((dwColor & 0x0000ff00) >> 8) * inv;
				float b = ((dwColor & 0x000000ff)) * inv;

				//���� �� ��
				float factor = (r + g + b) / 3.0f;

				//���� ��
				pos.y = factor * m_fHeightScale;
			}


			//���� UV ���

			//Terrain Tile Splating UV
			D3DXVECTOR2 baseUV;
			baseUV.x = x / static_cast<float>(m_nVerNumX - 1);
			baseUV.y = z / static_cast<float>(m_nVerNumZ - 1);

			//Terrain Tile UV
			D3DXVECTOR2 tileUV;
			tileUV.x = x * tileIntervalX;
			tileUV.y = z * tileIntervalY;

			//���ؽ� �迭�ε��� ���
			int idx = z * m_nVerNumX + x;

			m_pTerrainVertices[idx].pos = pos;
			m_pTerrainVertices[idx].normal = D3DXVECTOR3(0, 0, 0);	//�Ʒ����� ���� �븻 ���Ҷ� �������� �ϴ� 0 ���ͷ� �ʱ�ȭ
			m_pTerrainVertices[idx].baseUV = baseUV;
			m_pTerrainVertices[idx].tileUV = tileUV;
		}
	}

	//�ؽ����� pixel ���� Unlock
	m_pHeightMap->UnlockRect(0);


	//�ͷ��� ������ 
	SmoothTerrain(smooth);


	//
	// ���� �ε����� ���Ѵ�.....
	//
	LPTERRAINTRI pIndices = new tagTERRAINTRI[m_nTotalTri];

	//�ε��� �迭 �ε���
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

			//�ش� ���� ���� ���� �ε����� ����
			DWORD lt = z * m_nVerNumX + x;
			DWORD rt = z * m_nVerNumX + x + 1;
			DWORD lb = ((z + 1)* m_nVerNumX) + x;
			DWORD rb = ((z + 1)* m_nVerNumX) + x + 1;

			//���� �ﰢ�� �ϳ�
			pIndices[idx].dw0 = lt;
			pIndices[idx].dw1 = rt;
			pIndices[idx].dw2 = lb;
			idx++;

			//���� �ﰢ�� �ϳ�
			pIndices[idx].dw0 = lb;
			pIndices[idx].dw1 = rt;
			pIndices[idx].dw2 = rb;
			idx++;
		}
	}

	//
	// �븻�̶� Binormal �̶� Tangent �������...
	//
	D3DXVECTOR3* poses = new D3DXVECTOR3[m_nTotalVerNum];		//������ġ
	D3DXVECTOR3* normals = new D3DXVECTOR3[m_nTotalVerNum];
	D3DXVECTOR3* tangents = new D3DXVECTOR3[m_nTotalVerNum];
	D3DXVECTOR3* binormals = new D3DXVECTOR3[m_nTotalVerNum];
	D3DXVECTOR2* uvs = new D3DXVECTOR2[m_nTotalVerNum];
	DWORD* indices = (DWORD*)pIndices;

	//������ġ �� UV ����
	for (int i = 0; i < this->m_nTotalVerNum; i++)
	{
		poses[i] = this->m_pTerrainVertices[i].pos;
		uvs[i] = this->m_pTerrainVertices[i].baseUV;
	}


	//�븻���
	ComputeNormal(normals, poses, m_nTotalVerNum, indices, m_nTotalTri * 3);

	//ź��Ʈ ���̳븻 ���
	ComputeTangentAngBinormal(
		tangents,
		binormals,
		poses,
		normals,
		uvs,
		indices,
		m_nTotalTri,
		m_nTotalVerNum);

	//���Ȱ� ����
	for (int i = 0; i < this->m_nTotalVerNum; i++)
	{
		this->m_pTerrainVertices[i].normal = normals[i];
		this->m_pTerrainVertices[i].binormal = binormals[i];
		this->m_pTerrainVertices[i].tangent = tangents[i];
	}

	//
	// ���� ����
	//
	//�������� �����.
	Device->CreateVertexBuffer(
		sizeof(TERRAINVERTEX) * m_nTotalVerNum,
		D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_MANAGED,
		&m_pTerrainVB,
		0);

	//������� ���� ���۸� Lock �Ͽ� �ͷ��� ���� ���� ����.
	void* p = NULL;
	m_pTerrainVB->Lock(0, 0, &p, 0);
	memcpy(p, m_pTerrainVertices, sizeof(TERRAINVERTEX) * m_nTotalVerNum);
	m_pTerrainVB->Unlock();

	//�ε��� ���� �����.
	Device->CreateIndexBuffer(
		sizeof(TERRAINTRI) * m_nTotalTri,
		D3DUSAGE_WRITEONLY,				//D3DUSAGE_WRITEONLY ���� �����ϰ� ���������� ���۳����� �����ɶ� �ְ����� ����, D3DUSAGE_DYNAMIC �� �� �����Ͽ� ���� ������ �ٲ�� ���
		D3DFMT_INDEX32,
		D3DPOOL_DEFAULT,				//D3DUSAGE_DYNAMIC �� ������ ������ D3DPOOL_DEFAULT
		&m_pTerrainIB,
		0);

	//�ε��� ���ۿ� ���� ����.
	m_pTerrainIB->Lock(0, 0, &p, 0);
	memcpy(p, pIndices, sizeof(TERRAINTRI) * m_nTotalTri);
	m_pTerrainIB->Unlock();


	//
	// ���� ����θ� �����.
	//

	//������ ���¸� �˷��ִ� �迭
	D3DVERTEXELEMENT9 vertElement[7];			//�迭�� �������� ���� + 1

	/*

	//Terrain ���� ����ü
	typedef struct tagTERRAINVERTEX{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 binormal;
	D3DXVECTOR3 tangent;
	D3DXVECTOR2 baseUV;			//0~1 UV
	D3DXVECTOR2 tileUV;			//Ÿ�� UV
	}TERRAINVERTEX, *LPTERRAINVERTEX;
	*/


	//Position 
	vertElement[0].Stream = 0;							//Stream �� 0
	vertElement[0].Offset = 0;							//�޸��� ���� Byte ���� 0
	vertElement[0].Type = D3DDECLTYPE_FLOAT3;			//�ڷ��� Ÿ��
	vertElement[0].Method = D3DDECLMETHOD_DEFAULT;		//�ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[0].Usage = D3DDECLUSAGE_POSITION;		//���� Ÿ��
	vertElement[0].UsageIndex = 0;						//UsageIndex �ϴ� 0


	//Normal
	vertElement[1].Stream = 0;					//Stream �� 0
	vertElement[1].Offset = 12;					//�޸��� ���� Byte ���� 0
	vertElement[1].Type = D3DDECLTYPE_FLOAT3;	//�ڷ��� Ÿ��
	vertElement[1].Method = D3DDECLMETHOD_DEFAULT;		//�ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[1].Usage = D3DDECLUSAGE_NORMAL;		//���� Ÿ��
	vertElement[1].UsageIndex = 0;						//UsageIndex �ϴ� 0

	//BiNormal
	vertElement[2].Stream = 0;					//Stream �� 0
	vertElement[2].Offset = 24;					//�޸��� ���� Byte ���� 0
	vertElement[2].Type = D3DDECLTYPE_FLOAT3;	//�ڷ��� Ÿ��
	vertElement[2].Method = D3DDECLMETHOD_DEFAULT;		//�ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[2].Usage = D3DDECLUSAGE_BINORMAL;		//���� Ÿ��
	vertElement[2].UsageIndex = 0;						//UsageIndex �ϴ� 0

	//Tangent
	vertElement[3].Stream = 0;					//Stream �� 0
	vertElement[3].Offset = 36;					//�޸��� ���� Byte ���� 0
	vertElement[3].Type = D3DDECLTYPE_FLOAT3;	//�ڷ��� Ÿ��
	vertElement[3].Method = D3DDECLMETHOD_DEFAULT;		//�ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[3].Usage = D3DDECLUSAGE_TANGENT;		//���� Ÿ��
	vertElement[3].UsageIndex = 0;						//UsageIndex �ϴ� 0


	//BaseUV
	vertElement[4].Stream = 0;						//Stream �� 0
	vertElement[4].Offset = 48;						//�޸��� ���� Byte ���� 0
	vertElement[4].Type = D3DDECLTYPE_FLOAT2;		//�ڷ��� Ÿ��
	vertElement[4].Method = D3DDECLMETHOD_DEFAULT;		//�ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[4].Usage = D3DDECLUSAGE_TEXCOORD;		//���� Ÿ��
	vertElement[4].UsageIndex = 0;						//UsageIndex �ϴ� 0


	//tileUV
	vertElement[5].Stream = 0;						//Stream �� 0
	vertElement[5].Offset = 56;						//�޸��� ���� Byte ���� 0
	vertElement[5].Type = D3DDECLTYPE_FLOAT2;		//�ڷ��� Ÿ��
	vertElement[5].Method = D3DDECLMETHOD_DEFAULT;		//�ϴ� ������  D3DDECLMETHOD_DEFAULT
	vertElement[5].Usage = D3DDECLUSAGE_TEXCOORD;		//���� Ÿ��
	vertElement[5].UsageIndex = 1;						//UsageIndex �ι�° UV 1 

	//���̻� ����
	D3DVERTEXELEMENT9 end = D3DDECL_END(); //{0xFF,0,D3DDECLTYPE_UNUSED,0,0,0}
	vertElement[6] = end;


	//
	// LPDIRECT3DVERTEXDECLARATION9 ����
	//
	Device->CreateVertexDeclaration(
		vertElement,			//�տ��� ���� D3DVERTEXELEMENT9 �迭 ������
		&m_pTerrainDecl			//���� LPDIRECT3DVERTEXDECLARATION9 ������
		);


	SAFE_DELETE_ARR(poses);
	SAFE_DELETE_ARR(normals);
	SAFE_DELETE_ARR(tangents);
	SAFE_DELETE_ARR(binormals);
	SAFE_DELETE_ARR(uvs);
	SAFE_DELETE_ARR(indices);

	return S_OK;
}

//�ͷ��� ������
void cTerrain::SmoothTerrain(int passed)
{
	if (passed <= 0) return;


	float* smooth = new float[m_nTotalVerNum];

	while (passed > 0){

		passed--;

		for (int z = 0; z < m_nVerNumZ; z++){
			for (int x = 0; x < m_nVerNumX; x++){

				int adjacentSections = 0;		//��� ������ ��հ��� ����?
				float totalSections = 0.0f;		//�ֺ��� ���� ���� ������ �󸶴�?


				//����üũ
				if ((x - 1) > 0){
					totalSections += m_pTerrainVertices[(z * m_nVerNumX) + (x - 1)].pos.y;
					adjacentSections++;

					//���� ���
					if ((z - 1) > 0){
						totalSections += m_pTerrainVertices[((z - 1) * m_nVerNumX) + (x - 1)].pos.y;
						adjacentSections++;
					}

					//���� �ϴ�
					if ((z + 1) < m_nVerNumZ){
						totalSections += m_pTerrainVertices[((z + 1) * m_nVerNumX) + (x - 1)].pos.y;
						adjacentSections++;
					}
				}

				//������ üũ
				if ((x + 1) < m_nVerNumX){
					totalSections += m_pTerrainVertices[(z * m_nVerNumX) + (x + 1)].pos.y;
					adjacentSections++;

					//������ ���
					if ((z - 1) > 0){
						totalSections += m_pTerrainVertices[((z - 1) * m_nVerNumX) + (x + 1)].pos.y;
						adjacentSections++;
					}

					//������ �ϴ� 
					if ((z + 1) < m_nVerNumZ){
						totalSections += m_pTerrainVertices[((z + 1) * m_nVerNumX) + (x + 1)].pos.y;
						adjacentSections++;
					}
				}


				//���
				if ((z - 1) > 0)
				{
					totalSections += m_pTerrainVertices[((z - 1) * m_nVerNumX) + x].pos.y;
					adjacentSections++;
				}

				//�ϴ�
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


		//������ ���� y ������ ����
		for (int i = 0; i < m_nTotalVerNum; i++){
			m_pTerrainVertices[i].pos.y = smooth[i];
		}
	}

	SAFE_DELETE_ARR(smooth);
}
