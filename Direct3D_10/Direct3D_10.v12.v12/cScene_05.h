#pragma once
#include "cScene.h"
class cScene_05 : public cScene
{
private:
	//�����ϳ��� ���� ������ �����ϴ� ����ü
	typedef struct tagMYVERTEX{
		D3DXVECTOR3 pos;			//������ ��ġ
		D3DXVECTOR2 uv;				//���� UV ��ǥ	
		D3DXVECTOR3 normal;

	}MYVERTEX, *LPMYVERTEX;

	LPDIRECT3DVERTEXBUFFER9			m_vb;
	LPDIRECT3DINDEXBUFFER9			m_ib;
	LPDIRECT3DTEXTURE9				m_pDiffuseTex;
	LPDIRECT3DVERTEXDECLARATION9	m_pVDecl;			//���� ���� ����

	cCamera							m_mainCamera;
	cTransform						m_quadTrans;
	cTransform						m_lightTrans;		//���� Transform
	LPD3DXEFFECT					pEffect;			//�ε��� ���̴�

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

