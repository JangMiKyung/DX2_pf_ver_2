#pragma once

class cTerrain
{
public:
	//터레인 정점 구조체
	typedef struct tagTERRAINVERTEX{
		D3DXVECTOR3 pos;
	}TERRAINVERTEX, *LPTERRAINVERTTEX;

	//터레인 인덱스 구조체 ( 폴리곤 하나의 인덱싱 )
	typedef struct tagTERRAININDEX{
		DWORD		_0;
		DWORD		_1;
		DWORD		_2;
	}TERRAININDEX, *LPTERRAININDEX;



private:
	LPDIRECT3DVERTEXBUFFER9				terrainVb;
	LPDIRECT3DINDEXBUFFER9				terrainIb;
	LPDIRECT3DVERTEXDECLARATION9		terrainDecl;			//정점 구조체 선언

	float								fCellScale;				//셀의 간격

	int									nVerNumX;				//가로 정점수
	int									nVerNumZ;				//깊이 정점수
	int									nTotalVerNum;			//총 정점수

	int									nCellNumX;				//가로 셀수
	int									nCellNumZ;				//깊이 셀수
	int									nTotalCellNum;			//총 셀수
	int									nTotalTri;				//삼각형 갯수


	float								fTerrainSizeZ;			//터레인 가로 사이즈
	float								fTerrainSizeX;			//터레인 세로 사이즈

	D3DXVECTOR3*						vertexPoses;			//정점 위치 배열
	LPTERRAININDEX						pIndices;				//인덱스 정보




public:
	cTerrain(void);
	~cTerrain(void);

	//초기화 
	HRESULT Init(int vertexNum, float cellSize);

	//해제
	void Release();

	//랜더
	void Render();

};

