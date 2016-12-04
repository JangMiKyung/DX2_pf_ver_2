#pragma once

#include "cSingletonBase.h"

class cSpriteManager : public cSingletonBase<cSpriteManager>
{
private:
	LPD3DXSPRITE m_pSprite;

public:
	cSpriteManager();
	~cSpriteManager();

	HRESULT Init(LPDIRECT3DDEVICE9	pDevice);
	void Release();

	void BeginSpriteRender();		//���ö���Ʈ ���� ���۽� ȣ��
	void EndSpriteRender();			//���ö���Ʈ ���� ����� ȣ��

	void DrawTexture(
		LPDIRECT3DTEXTURE9 pTex,			//�׸� Texture
		LPRECT pSour,						//�׸� �ҽ� ����
		float posX, float posY,				//�׸� ȭ�� ��ġ
		DWORD color = 0xffffffff,			//���̴� ����
		D3DXVECTOR3* pCenter = NULL			//�̹����� �߽� ( NULL �̸� �»�� )
		);


	void DrawTexture(
		LPDIRECT3DTEXTURE9 pTex,			//�׸� Texture
		LPRECT pSour,						//�׸� �ҽ� ����
		float posX, float posY,				//�׸� ȭ�� ��ġ
		float scaleX, float scaleY,			//�׸� ������ ũ��
		float rotate,						//�׸� �����̼� ��
		DWORD color = 0xffffffff,			//���̴� ����
		D3DXVECTOR3* pCenter = NULL			//�̹����� �߽� ( NULL �̸� �»�� )
		);


	void DrawArea(LPDIRECT3DTEXTURE9 pTex,
		LPRECT pSour,						//�׸��ҽ��� ����
		LPRECT pDest,						//�׸�ȭ���� ����
		DWORD color = 0xffffffff			//���̴� ���� 
		);


	void DrawNinePatch(
		LPDIRECT3DTEXTURE9 pTex,
		LPRECT pSour,						//�׸� �ҽ� ����
		LPRECT pDest,						//�׸� ȭ�� ����
		LPRECT pCenter,						//������ġ ���� ����
		DWORD color = 0xffffffff,			//���̴� ���� 
		bool bDrawCenter = true				//�߾� �׸����� �ȱ׸����� ����
		);


	void DrawLoop(
		LPDIRECT3DTEXTURE9 pTex,
		float offsetX, float offsetY,		//SourOffset 
		LPRECT pSour,						//�׸� �ҽ� ����
		LPRECT pDest,						//�׸� ȭ�� ����
		DWORD color = 0xffffffff			//���̴� ���� 
		);
};

#define SPRITE_MGR cSpriteManager::GetInstance()