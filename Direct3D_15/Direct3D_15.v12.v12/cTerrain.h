#pragma once

class cTerrain
{
public:
	//�ͷ��� ���� ����ü
	typedef struct tagTERRAINVERTEX{
		D3DXVECTOR3 pos;
	}TERRAINVERTEX, *LPTERRAINVERTTEX;

	//�ͷ��� �ε��� ����ü ( ������ �ϳ��� �ε��� )
	typedef struct tagTERRAININDEX{
		DWORD		_0;
		DWORD		_1;
		DWORD		_2;
	}TERRAININDEX, *LPTERRAININDEX;



private:
	LPDIRECT3DVERTEXBUFFER9				terrainVb;
	LPDIRECT3DINDEXBUFFER9				terrainIb;
	LPDIRECT3DVERTEXDECLARATION9		terrainDecl;			//���� ����ü ����

	float								fCellScale;				//���� ����

	int									nVerNumX;				//���� ������
	int									nVerNumZ;				//���� ������
	int									nTotalVerNum;			//�� ������

	int									nCellNumX;				//���� ����
	int									nCellNumZ;				//���� ����
	int									nTotalCellNum;			//�� ����
	int									nTotalTri;				//�ﰢ�� ����


	float								fTerrainSizeZ;			//�ͷ��� ���� ������
	float								fTerrainSizeX;			//�ͷ��� ���� ������

	D3DXVECTOR3*						vertexPoses;			//���� ��ġ �迭
	LPTERRAININDEX						pIndices;				//�ε��� ����




public:
	cTerrain(void);
	~cTerrain(void);

	//�ʱ�ȭ 
	HRESULT Init(int vertexNum, float cellSize);

	//����
	void Release();

	//����
	void Render();

};

