#include "stdafx.h"
#include "cSpriteManager.h"


cSpriteManager::cSpriteManager()
{
}


cSpriteManager::~cSpriteManager()
{
}

HRESULT cSpriteManager::Init(LPDIRECT3DDEVICE9	pDevice)
{
	//���ö���Ʈ ��ü ����
	if (FAILED(D3DXCreateSprite(pDevice, &m_pSprite)))
		return E_FAIL;

	return S_OK;
}

void cSpriteManager::Release()
{
	//���ö���Ʈ ��ü ����
	SAFE_RELEASE(m_pSprite);
}

void cSpriteManager::BeginSpriteRender()		//���ö���Ʈ ���� ���۽� ȣ��
{
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void cSpriteManager::EndSpriteRender()			//���ö���Ʈ ���� ����� ȣ��
{
	m_pSprite->End();
}

void cSpriteManager::DrawTexture(
	LPDIRECT3DTEXTURE9 pTex,			//�׸� Texture
	LPRECT pSour,						//�׸� �ҽ� ����
	float posX, float posY,				//�׸� ȭ�� ��ġ
	DWORD color /*= 0xffffffff*/,		//���̴� ����
	D3DXVECTOR3* pCenter /*= NULL*/		//�̹����� �߽� ( NULL �̸� �»�� )
	)
{
	//ȭ�� ���� ��ǥ�� �׸��� SpriteCanvas �� Transform �� �ʱ�ȭ
	D3DXMATRIXA16 matFinal;
	D3DXMatrixIdentity(&matFinal);
	m_pSprite->SetTransform(&matFinal);

	//�׸�ȭ����ġ ( ���ǻ��� Vector2 ���ƴ϶� Vector3 �̴�, Z �� �Ϲ������� ���õǴµ� ���̸� �ְ������ ����ض�... )
	D3DXVECTOR3 pos(posX, posY, 0.0f);

	//���ö���Ʈ ���並 ���� Texture �� ȭ�鿡 �׸���...
	m_pSprite->Draw(
		pTex,		//�׸� Texture
		pSour,		//�׸� �̹����� �ҽ� ����
		(pCenter == NULL) ? &D3DXVECTOR3(0, 0, 0) : pCenter,		//�׸��� �̹��� �߽���ġ
		&pos,		//�׸�ȭ�� ��ġ 
		color		//Hit �÷�
		);
}


void cSpriteManager::DrawTexture(
	LPDIRECT3DTEXTURE9 pTex,			//�׸� Texture
	LPRECT pSour,						//�׸� �ҽ� ����
	float posX, float posY,				//�׸� ȭ�� ��ġ
	float scaleX, float scaleY,			//�׸� ������ ũ��
	float rotate,						//�׸� �����̼� ��
	DWORD color /*= 0xffffffff*/,		//���̴� ����
	D3DXVECTOR3* pCenter /*= NULL*/		//�̹����� �߽� ( NULL �̸� �»�� )
	)
{
	//�̹����� ������
	D3DXVECTOR3 pivot =
		(pCenter == NULL) ? D3DXVECTOR3(0, 0, 0) : *pCenter;

	//������ ���
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale,
		scaleX, scaleY, 1.0f);

	//�̵� ���
	D3DXMATRIXA16 matTrans;
	D3DXMatrixTranslation(&matTrans,
		posX, posY, 0.0f);


	//ȸ�� ���
	D3DXMATRIXA16 matRot;
	D3DXMatrixRotationZ(&matRot,
		rotate);

	//���� ���
	D3DXMATRIXA16 matFinal;
	matFinal = matScale * matRot * matTrans;

	//������ ����
	m_pSprite->SetTransform(&matFinal);


	//�̹��� �׷���
	m_pSprite->Draw(
		pTex,
		pSour,
		&pivot,
		&D3DXVECTOR3(0, 0, 0),	//�׸� ��ġ�� 0 �̳�....
		color);
}


void cSpriteManager::DrawArea(LPDIRECT3DTEXTURE9 pTex,
	LPRECT pSour,						//�׸��ҽ��� ����
	LPRECT pDest,						//�׸�ȭ���� ����
	DWORD color /*= 0xffffffff*/		//���̴� ���� 
	)
{
	float destWidth = pDest->right - pDest->left;
	float destHeight = pDest->bottom - pDest->top;

	float sourWidth = pSour->right - pSour->left;
	float sourHeight = pSour->bottom - pSour->top;

	//���� ������ ���� ������ ��
	float scaleX = destWidth / sourWidth;
	float scaleY = destHeight / sourHeight;


	//������ ���
	D3DXMATRIXA16 matScale;
	D3DXMatrixScaling(&matScale,
		scaleX, scaleY, 1.0f);


	//�̵����
	D3DXMATRIXA16 matTrans;
	D3DXMatrixTranslation(
		&matTrans,
		pDest->left, pDest->top, 0.0f);

	D3DXMATRIXA16 matFinal = matScale * matTrans;
	m_pSprite->SetTransform(&matFinal);


	//�̹��� �׷���
	m_pSprite->Draw(
		pTex,
		pSour,
		&D3DXVECTOR3(0, 0, 0),
		&D3DXVECTOR3(0, 0, 0),	//�׸� ��ġ�� 0 �̳�....
		color);

}


void cSpriteManager::DrawNinePatch(
	LPDIRECT3DTEXTURE9 pTex,
	LPRECT pSour,						//�׸� �ҽ� ����
	LPRECT pDest,						//�׸� ȭ�� ����
	LPRECT pCenter,						//������ġ ���� ����
	DWORD color /*= 0xffffffff*/,		//���̴� ���� 
	bool bDrawCenter /*= true*/			//�߾� �׸����� �ȱ׸����� ����
	)
{
	float fCenterW = pCenter->right - pCenter->left;
	float fCenterH = pCenter->bottom - pCenter->top;

	//�ҽ�����
	RECT rcSrcLeftTop		= { 0,				0,				pCenter->left,	pCenter->top };
	RECT rcSrcTop			= { pCenter->left,	0,				pCenter->right, pCenter->top };
	RECT rcSrcRightTop		= { pCenter->right, 0,				pSour->right,	pCenter->top };
	RECT rcSrcLeftCenter	= { 0,				pCenter->top,	pCenter->left,	pCenter->bottom };
	RECT rcSrcCenter		= { pCenter->left,	pCenter->top,	pCenter->right, pCenter->bottom };
	RECT rcSrcRightCenter	= { pCenter->right, pCenter->top,	pSour->right,	pCenter->bottom };
	RECT rcSrcLeftBottom	= { 0,				pCenter->bottom,pCenter->left,	pSour->bottom };
	RECT rcSrcBottom		= { pCenter->left,	pCenter->bottom,pCenter->right, pSour->bottom };
	RECT rcSrcRightBottom	= { pCenter->right, pCenter->bottom,pSour->right,	pSour->bottom };

	//�׸��� ����
	//��ü
	float left = pDest->left;
	float top = pDest->top;
	float right = pDest->right;
	float bottom = pDest->bottom;

	float centerLeft	= pDest->left + (pCenter->left - pSour->left);
	float centerRight	= pDest->right - (pSour->right - pCenter->right);
	float centerTop		= pDest->top + (pCenter->top - pSour->top);
	float centerBottom	= pDest->bottom - (pSour->bottom - pCenter->bottom);


	RECT rcDstLeftTop		= { left,			top,			centerLeft,		centerTop };
	RECT rcDstTop			= { centerLeft,		top,			centerRight,	centerTop };
	RECT rcDstRightTop		= { centerRight,	top,			right,			centerTop };
	RECT rcDstLeftCenter	= { left,			centerTop,		centerLeft,		centerBottom };
	RECT rcDstCenter		= { centerLeft,		centerTop,		centerRight,	centerBottom };
	RECT rcDstRightCenter	= { centerRight,	centerTop,		right,			centerBottom };
	RECT rcDstLeftBottom	= { left,			centerBottom,	centerLeft,		bottom };
	RECT rcDstBottom		= { centerLeft,		centerBottom,	centerRight,	bottom };
	RECT rcDstRightBottom	= { centerRight,	centerBottom,	right,			bottom };

	DrawArea(pTex, &rcSrcLeftTop, &rcDstLeftTop, color);
	DrawArea(pTex, &rcSrcTop, &rcDstTop, color);
	DrawArea(pTex, &rcSrcRightTop, &rcDstRightTop, color);
	DrawArea(pTex, &rcSrcLeftCenter, &rcDstLeftCenter, color);

	//����� true �϶��� �׸���.
	if (bDrawCenter)
		DrawArea(pTex, &rcSrcCenter, &rcDstCenter, color);

	DrawArea(pTex, &rcSrcRightCenter, &rcDstRightCenter, color);
	DrawArea(pTex, &rcSrcLeftBottom, &rcDstLeftBottom, color);
	DrawArea(pTex, &rcSrcBottom, &rcDstBottom, color);
	DrawArea(pTex, &rcSrcRightBottom, &rcDstRightBottom, color);
}


void cSpriteManager::DrawLoop(
	LPDIRECT3DTEXTURE9 pTex,
	float fOffsetX, float fOffsetY,		//SourOffset 
	LPRECT pSour,						//�׸� �ҽ� ����
	LPRECT pDest,						//�׸� ȭ�� ����
	DWORD color /*= 0xffffffff*/		//���̴� ���� 
	)
{
	//�ҽ��� ����ũ��
	int sourW = pSour->right - pSour->left;
	int sourH = pSour->bottom - pSour->top;

	//�ҽ� Offset ������
	int offSetX = static_cast<int>(fOffsetX) % sourW;
	int offSetY = static_cast<int>(fOffsetY) % sourH;

	//���� ������ ��ġ
	if (offSetX < 0)
		offSetX = sourW + offSetX;
	if (offSetY < 0)
		offSetY = sourH + offSetY;

	//����Ʈ�� ����ũ�� ( �ѱ׸� ���� �ȴ� )
	int destW = pDest->right - pDest->left;
	int destH = pDest->bottom - pDest->top;

	//���� �ѹ��׷����� ( ���� �ѹ� �������� �׷��� �� )
	int drawX = 0;
	int drawY = 0;

	//���� �ҽ� ����
	RECT rcDrawSour = { 0, 0, 0, 0 };
	RECT rcDrawDest = { 0, 0, 0, 0 };


	//���� ����
	for (int x = 0; x < destW; x += drawX)
	{
		//���� �ҽ� ������ ���Ѵ�.
		rcDrawSour.left = (offSetX + x) % sourW + pSour->left;
		rcDrawSour.right = pSour->right;

		//��ο� �Ǵ� ���� ���� 
		drawX = rcDrawSour.right - rcDrawSour.left;

		//���η� �� �׷����� ���� �Ѿ�ٸ� 
		if (x + drawX > destW){
			//�Ѿ ����ŭ �������� ���� �����.
			int overW = (x + drawX) - destW;
			rcDrawSour.right -= overW;

			//��ο� �Ǵ� ���η� ����
			drawX = rcDrawSour.right - rcDrawSour.left;
		}



		//���� ����Ʈ ������ ���Ѵ�.
		rcDrawDest.left = pDest->left + x;
		rcDrawDest.right = rcDrawDest.left + drawX;




		for (int y = 0; y < destH; y += drawY)
		{
			//���� �ҽ� ������ ���Ѵ�.
			rcDrawSour.top = (offSetY + y) % sourH + pSour->top;
			rcDrawSour.bottom = pSour->bottom;

			//��ο� �Ǵ� ���� ���� 
			drawY = rcDrawSour.bottom - rcDrawSour.top;

			//���η� �� �׷����� ���� �Ѿ�ٸ� 
			if (y + drawY > destH){

				//�Ѿ ����ŭ �Ʒ��� �������� �����.
				int overH = (y + drawY) - destH;
				rcDrawSour.bottom -= overH;

				//��ο� �Ǵ� ���� �� ����
				drawY = rcDrawSour.bottom - rcDrawSour.top;
			}


			//���� ����Ʈ ������ ���Ѵ�.
			rcDrawDest.top = pDest->top + y;
			rcDrawDest.bottom = rcDrawDest.top + drawY;


			//�ҽ������� ����Ʈ������ �̿��Ͽ� �׸���.
			DrawArea(pTex, &rcDrawSour, &rcDrawDest, color);
		}
	}
}