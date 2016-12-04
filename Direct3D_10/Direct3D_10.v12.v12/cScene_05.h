#pragma once
#include "cScene.h"
class cScene_05 : public cScene
{
private:
	//정점하나에 대한 정보를 정의하는 구조체
	typedef struct tagMYVERTEX{
		D3DXVECTOR3 pos;			//정점의 위치
		D3DXVECTOR2 uv;				//정점 UV 좌표	
		D3DXVECTOR3 normal;

	}MYVERTEX, *LPMYVERTEX;

	LPDIRECT3DVERTEXBUFFER9			m_vb;
	LPDIRECT3DINDEXBUFFER9			m_ib;
	LPDIRECT3DTEXTURE9				m_pDiffuseTex;
	LPDIRECT3DVERTEXDECLARATION9	m_pVDecl;			//정점 정보 선언

	cCamera							m_mainCamera;
	cTransform						m_quadTrans;
	cTransform						m_lightTrans;		//광원 Transform
	LPD3DXEFFECT					pEffect;			//로딩된 셰이더

public:
	cScene_05();
	~cScene_05();

	virtual HRESULT Init();

	virtual void Release();

	virtual void Update(float timeDelta);

	virtual void Render();

private:

	void CreateQuad();
};

