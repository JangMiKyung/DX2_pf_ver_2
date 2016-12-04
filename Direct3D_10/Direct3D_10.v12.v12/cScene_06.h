#pragma once
#include "cScene.h"
class cScene_06 : public cScene
{
private:
	//�����ϳ��� ���� ������ �����ϴ� ����ü
	typedef struct tagMYVERTEX{
		D3DXVECTOR3 pos;			//������ ��ġ
		D3DXVECTOR2 uv;				//���� UV ��ǥ	

		//����Space 3�ѻ�
		D3DXVECTOR3 normal;
		D3DXVECTOR3 binormal;
		D3DXVECTOR3 tangent;

	}MYVERTEX, *LPMYVERTEX;

	LPDIRECT3DVERTEXBUFFER9			m_vb;
	LPDIRECT3DINDEXBUFFER9			m_ib;
	LPDIRECT3DTEXTURE9				m_pDiffuseTex;
	LPDIRECT3DTEXTURE9				m_pNormalTex;
	LPDIRECT3DVERTEXDECLARATION9	m_pVDecl;			//���� ���� ����

	cCamera							m_mainCamera;
	cTransform						m_quadTrans;
	cTransform						m_lightTrans;		//���� Transform
	LPD3DXEFFECT					pEffect;			//�ε��� ���̴�

public:
	cScene_06();
	~cScene_06();

	virtual HRESULT Init();

	virtual void Release();

	virtual void Update(float timeDelta);

	virtual void Render();

private:

	void CreateQuad();
};

