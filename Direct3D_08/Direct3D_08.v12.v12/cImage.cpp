#include "StdAfx.h"
#include "cImage.h"

cImage::cImage( void )
{
}

cImage::~cImage( void )
{
}

//��Ʈ�� �����Լ�
HRESULT cImage::Init( DWORD dwWidth, DWORD dwHeight )
{
	HDC hdc = GetDC( g_hWnd );

	//��Ʈ�� ���μ��� ũ�����
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;

	//BitmapDC ����
	m_BitDC = CreateCompatibleDC( hdc );
	//dwWidth, dwHeight ũ���� ���ũ���� �����.
	m_NewBit = CreateCompatibleBitmap( hdc, dwWidth, dwHeight );
	//BitmapDC �� ǥ���� �ҷ��� Bitmap ���� �ϰ� ���� ǥ�� ������ m_OldBit ��....
	m_OldBit = (HBITMAP) SelectObject( m_BitDC, m_NewBit );

	//Blend�� �� BitmapDC ����
	m_BlendDC = CreateCompatibleDC( hdc );
	m_NewBlendBit = CreateCompatibleBitmap( hdc, dwWidth, dwHeight );
	m_OldBlendBit = (HBITMAP) SelectObject( m_BlendDC, m_NewBlendBit );

	m_TransColorKey = -1;

	//���� ����ü �� ����
	m_BlendFunc.AlphaFormat = 0;
	m_BlendFunc.BlendFlags = 0;
	m_BlendFunc.BlendOp = AC_SRC_OVER;
	m_BlendFunc.SourceConstantAlpha = 0;

	//���� DC ����
	ReleaseDC( g_hWnd, hdc );

	return S_OK;
}


HRESULT cImage::Init( const char* szFileName, DWORD dwWidth, DWORD dwHeight, COLORREF transColorKey )
{
	HDC hdc = GetDC( g_hWnd );

	//��Ʈ�� ���μ��� ũ�����
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;

	//BitmapDC ����
	m_BitDC = CreateCompatibleDC( hdc );
	//Bitmap �ҷ��´�.
	m_NewBit = (HBITMAP) LoadImage( g_hInst, szFileName, IMAGE_BITMAP, dwWidth, dwHeight, LR_LOADFROMFILE );
	//BitmapDC �� ǥ���� �ҷ��� Bitmap ���� �ϰ� ���� ǥ�� ������ m_OldBit ��....
	m_OldBit = (HBITMAP) SelectObject( m_BitDC, m_NewBit );

	//Blend�� �� BitmapDC ����
	m_BlendDC = CreateCompatibleDC( hdc );
	m_NewBlendBit = CreateCompatibleBitmap( hdc, dwWidth, dwHeight );
	m_OldBlendBit = (HBITMAP) SelectObject( m_BlendDC, m_NewBlendBit );

	//���� DC ����
	ReleaseDC( g_hWnd, hdc );

	//���� ����ü �� ����
	m_BlendFunc.AlphaFormat = 0;
	m_BlendFunc.BlendFlags = 0;
	m_BlendFunc.BlendOp = AC_SRC_OVER;
	m_BlendFunc.SourceConstantAlpha = 0;

	//�÷� Ű�� ����
	m_TransColorKey = transColorKey;

	return S_OK;
}

//��Ʈ�� ���� �Լ�
void cImage::Release( void )
{
	//Bitmap ����
	DeleteObject( SelectObject( m_BitDC, m_OldBit ) );
	DeleteObject( SelectObject( m_BlendDC, m_OldBlendBit ) );

	//Bitmap DC ����
	DeleteDC( m_BitDC );
	DeleteDC( m_BlendDC );
}

//��Ʈ�� �׸���
void cImage::Draw( HDC hdc, int x, int y )
{
	//���÷�Ű�� ���ٸ�
	if ( m_TransColorKey == -1 ) {
		BitBlt(
			hdc,			//Target DC
			x, y,			//Target DC ��ġ
			m_dwWidth, m_dwHeight,  //ũ���� ���μ��� ��
			m_BitDC,		//Source DC
			0, 0,			//Source ���� ��ġ
			SRCCOPY			//���� �ɼ�
		);
	}

	//���÷��� �ִٸ�..
	else {
		GdiTransparentBlt(
			hdc,			//Target DC
			x, y,			//Target DC ��ġ
			m_dwWidth, m_dwHeight,  //ũ���� ���μ��� ��
			m_BitDC,		//Source DC
			0, 0,			//Source ����
			m_dwWidth, m_dwHeight,
			m_TransColorKey );
	}




}

void cImage::Draw( HDC hdc, int x, int y, int sourX, int sourY, int sourW, int sourH )
{
	//���÷�Ű�� ���ٸ�
	if ( m_TransColorKey == -1 ) {

		BitBlt(
			hdc,			//Target DC
			x, y,			//Target DC ��ġ
			sourW, sourH,   //�׸��� ���μ��� ��
			m_BitDC,		//Source DC
			sourX, sourY,	//Source ���� ��ġ
			SRCCOPY			//���� �ɼ�
		);
	}

	//���÷��� �ִٸ�..
	else {

		GdiTransparentBlt(
			hdc,			//Target DC
			x, y,			//Target DC ��ġ
			sourW, sourH,   //ũ���� ���μ��� ��
			m_BitDC,		//Source DC
			sourX, sourY,	//Source ����
			sourW, sourH,
			m_TransColorKey );
	}

}

void cImage::Draw( HDC hdc, int x, int y, RECT& rcSour )
{
	DWORD sourW = rcSour.right - rcSour.left;
	DWORD sourH = rcSour.bottom - rcSour.top;
	DWORD sourX = rcSour.left;
	DWORD sourY = rcSour.top;

	//���÷�Ű�� ���ٸ�
	if ( m_TransColorKey == -1 ) {

		BitBlt(
			hdc,			//Target DC
			x, y,			//Target DC ��ġ
			sourW, sourH,   //�׸��� ���μ��� ��
			m_BitDC,		//Source DC
			sourX, sourY,	//Source ���� ��ġ
			SRCCOPY			//���� �ɼ�
		);
	}

	//���÷��� �ִٸ�..
	else {

		GdiTransparentBlt(
			hdc,			//Target DC
			x, y,			//Target DC ��ġ
			sourW, sourH,   //ũ���� ���μ��� ��
			m_BitDC,		//Source DC
			sourX, sourY,	//Source ����
			sourW, sourH,
			m_TransColorKey );
	}
}


void cImage::DrawAlpha( HDC hdc, int x, int y, BYTE alpha )
{
	m_BlendFunc.SourceConstantAlpha = alpha;

	//�÷�Ű���� ������....
	if ( m_TransColorKey != -1 ) {

		//1. hdc TargetDC ������ BlendDC �� �׸���.
		BitBlt(
			m_BlendDC,
			0, 0, m_dwWidth, m_dwHeight,
			hdc, x, y, SRCCOPY );

		//2. Sour�� ������ BlendDC �� �׸���.
		GdiTransparentBlt(
			m_BlendDC,				//Target DC
			0, 0,					//Target DC ��ġ
			m_dwWidth, m_dwHeight,  //ũ���� ���μ��� ��
			m_BitDC,				//Source DC
			0, 0,					//Source ����
			m_dwWidth, m_dwHeight,
			m_TransColorKey );

		//3. BlendDC �� ������ hdc �� Alpha ����� �׸���.
		GdiAlphaBlend(
			hdc,
			x, y,
			m_dwWidth, m_dwHeight,
			m_BlendDC,
			0, 0,
			m_dwWidth, m_dwHeight,
			m_BlendFunc );
	}

	//�÷� Ű ���� ������..
	else {
		GdiAlphaBlend(
			hdc,
			x, y,
			m_dwWidth, m_dwHeight,
			m_BitDC,
			0, 0,
			m_dwWidth, m_dwHeight,
			m_BlendFunc );

	}
}

void cImage::DrawAlpha( HDC hdc, int x, int y, int sourX, int sourY, int sourW, int sourH, BYTE alpha )
{
	m_BlendFunc.SourceConstantAlpha = alpha;

	//�÷�Ű���� ������....
	if ( m_TransColorKey != -1 ) {

		//1. hdc TargetDC ������ BlendDC �� �׸���.
		BitBlt(
			m_BlendDC,
			0, 0, sourW, sourH,
			hdc, x, y, SRCCOPY );

		//2. Sour�� ������ BlendDC �� �׸���.
		GdiTransparentBlt(
			m_BlendDC,				//Target DC
			0, 0,					//Target DC ��ġ
			sourW, sourH,  //ũ���� ���μ��� ��
			m_BitDC,				//Source DC
			sourX, sourY,					//Source ����
			sourW, sourH,
			m_TransColorKey );

		//3. BlendDC �� ������ hdc �� Alpha ����� �׸���.
		GdiAlphaBlend(
			hdc,
			x, y,
			sourW, sourH,
			m_BlendDC,
			0, 0,
			sourW, sourH,
			m_BlendFunc );

	}

	//�÷� Ű ���� ������..
	else {
		GdiAlphaBlend(
			hdc,
			x, y,
			sourW, sourH,
			m_BitDC,
			sourX, sourY,
			sourW, sourH,
			m_BlendFunc );

	}
}

void cImage::DrawAlpha( HDC hdc, int x, int y, RECT& rcSour, BYTE alpha )
{

	DWORD sourW = rcSour.right - rcSour.left;
	DWORD sourH = rcSour.bottom - rcSour.top;
	DWORD sourX = rcSour.left;
	DWORD sourY = rcSour.top;


	m_BlendFunc.SourceConstantAlpha = alpha;

	//�÷�Ű���� ������....
	if ( m_TransColorKey != -1 ) {

		//1. hdc TargetDC ������ BlendDC �� �׸���.
		BitBlt(
			m_BlendDC,
			0, 0, sourW, sourH,
			hdc, x, y, SRCCOPY );

		//2. Sour�� ������ BlendDC �� �׸���.
		GdiTransparentBlt(
			m_BlendDC,				//Target DC
			0, 0,					//Target DC ��ġ
			sourW, sourH,  //ũ���� ���μ��� ��
			m_BitDC,				//Source DC
			sourX, sourY,					//Source ����
			sourW, sourH,
			m_TransColorKey );

		//3. BlendDC �� ������ hdc �� Alpha ����� �׸���.
		GdiAlphaBlend(
			hdc,
			x, y,
			sourW, sourH,
			m_BlendDC,
			0, 0,
			sourW, sourH,
			m_BlendFunc );

	}

	//�÷� Ű ���� ������..
	else {
		GdiAlphaBlend(
			hdc,
			x, y,
			sourW, sourH,
			m_BitDC,
			sourX, sourY,
			sourW, sourH,
			m_BlendFunc );

	}
}



//�ݺ���ο�
void cImage::LoopDraw( HDC hdc, int offsetX, int offsetY, RECT& rcDest )
{
	//�׸� �̹��� ���� ��ġ
	if ( offsetX < 0 ) offsetX = m_dwWidth + ( offsetX % m_dwWidth );
	if ( offsetY < 0 ) offsetY = m_dwHeight + ( offsetY % m_dwHeight );


	//�׸��� �ѷ�
	int nDrawTotalWidth = rcDest.right - rcDest.left;
	int nDrawTotalHeight = rcDest.bottom - rcDest.top;

	//�׷��� �����簢��
	RECT	rcHdc;

	//�׸� �ҽ��� �����簢��
	RECT	rcSour;

	//�׷��� ���� ������ ��
	int		nDrawW;
	int		nDrawH;

	//���� 
	for ( int y = 0; y < nDrawTotalHeight; y += nDrawH ) {

		//�ҽ��� �������� ������.
		rcSour.top = ( y + offsetY ) % m_dwHeight;
		rcSour.bottom = m_dwHeight;

		//�׸��� ���� ��
		nDrawH = rcSour.bottom - rcSour.top;

		//�Ѿ�ٸ�...
		if ( y + nDrawH > nDrawTotalHeight ) {

			//�Ѿ ����ŭ ������ ���� �����.
			rcSour.bottom -= ( y + nDrawH ) - nDrawTotalHeight;

			//�׸��� ���� ��
			nDrawH = rcSour.bottom - rcSour.top;
		}

		//�׸��� ������ ���� ���� ���Ѵ�.
		rcHdc.top = rcDest.top + y;
		rcHdc.bottom = rcHdc.top + nDrawH;

		//���� 
		for ( int x = 0; x < nDrawTotalWidth; x += nDrawW ) {

			//�ҽ��� �������� ������.
			rcSour.left = ( x + offsetX ) % m_dwWidth;
			rcSour.right = m_dwWidth;

			//�׸��� ���� ��
			nDrawW = rcSour.right - rcSour.left;

			//�Ѿ�ٸ�...
			if ( x + nDrawW > nDrawTotalWidth ) {

				//�Ѿ ����ŭ �������� �������� �����.
				rcSour.right -= ( x + nDrawW ) - nDrawTotalWidth;

				//�׸��� ���� ��
				nDrawW = rcSour.right - rcSour.left;
			}

			//�׸��� ������ ���� ���� ���Ѵ�.
			rcHdc.left = rcDest.left + x;
			rcHdc.right = rcHdc.left + nDrawW;


			//�׸���
			this->Draw( hdc, rcHdc.left, rcHdc.top, rcSour );
		}
	}
}


void cImage::LoopDrawAlpha( HDC hdc, int offsetX, int offsetY, RECT& rcDest, BYTE alpha )
{
	//�׸� �̹��� ���� ��ġ
	if ( offsetX < 0 ) offsetX = m_dwWidth + ( offsetX % m_dwWidth );
	if ( offsetY < 0 ) offsetY = m_dwHeight + ( offsetY % m_dwHeight );




	//�׸��� �ѷ�
	int nDrawTotalWidth = rcDest.right - rcDest.left;
	int nDrawTotalHeight = rcDest.bottom - rcDest.top;

	//�׷��� �����簢��
	RECT	rcHdc;

	//�׸� �ҽ��� �����簢��
	RECT	rcSour;

	//�׷��� ���� ������ ��
	int		nDrawW;
	int		nDrawH;

	//���� 
	for ( int y = 0; y < nDrawTotalHeight; y += nDrawH ) {

		//�ҽ��� �������� ������.
		rcSour.top = ( y + offsetY ) % m_dwHeight;
		rcSour.bottom = m_dwHeight;

		//�׸��� ���� ��
		nDrawH = rcSour.bottom - rcSour.top;

		//�Ѿ�ٸ�...
		if ( y + nDrawH > nDrawTotalHeight ) {

			//�Ѿ ����ŭ ������ ���� �����.
			rcSour.bottom -= ( y + nDrawH ) - nDrawTotalHeight;

			//�׸��� ���� ��
			nDrawH = rcSour.bottom - rcSour.top;
		}

		//�׸��� ������ ���� ���� ���Ѵ�.
		rcHdc.top = rcDest.top + y;
		rcHdc.bottom = rcHdc.top + nDrawH;

		//���� 
		for ( int x = 0; x < nDrawTotalWidth; x += nDrawW ) {

			//�ҽ��� �������� ������.
			rcSour.left = ( x + offsetX ) % m_dwWidth;
			rcSour.right = m_dwWidth;

			//�׸��� ���� ��
			nDrawW = rcSour.right - rcSour.left;

			//�Ѿ�ٸ�...
			if ( x + nDrawW > nDrawTotalWidth ) {

				//�Ѿ ����ŭ �������� �������� �����.
				rcSour.right -= ( x + nDrawW ) - nDrawTotalWidth;

				//�׸��� ���� ��
				nDrawW = rcSour.right - rcSour.left;
			}

			//�׸��� ������ ���� ���� ���Ѵ�.
			rcHdc.left = rcDest.left + x;
			rcHdc.right = rcHdc.left + nDrawW;


			//�׸���
			this->DrawAlpha( hdc, rcHdc.left, rcHdc.top, rcSour, alpha );
		}
	}
}




///////////////////////////////////////////////////////////////////////////
