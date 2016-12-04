#include "StdAfx.h"
#include "cImage.h"

cImage::cImage( void )
{
}

cImage::~cImage( void )
{
}

//비트맵 셋팅함수
HRESULT cImage::Init( DWORD dwWidth, DWORD dwHeight )
{
	HDC hdc = GetDC( g_hWnd );

	//비트맵 가로세로 크기대입
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;

	//BitmapDC 생성
	m_BitDC = CreateCompatibleDC( hdc );
	//dwWidth, dwHeight 크기의 빈비크맵을 만든다.
	m_NewBit = CreateCompatibleBitmap( hdc, dwWidth, dwHeight );
	//BitmapDC 의 표면을 불러온 Bitmap 으로 하고 이전 표면 정보를 m_OldBit 가....
	m_OldBit = (HBITMAP) SelectObject( m_BitDC, m_NewBit );

	//Blend용 빈 BitmapDC 생성
	m_BlendDC = CreateCompatibleDC( hdc );
	m_NewBlendBit = CreateCompatibleBitmap( hdc, dwWidth, dwHeight );
	m_OldBlendBit = (HBITMAP) SelectObject( m_BlendDC, m_NewBlendBit );

	m_TransColorKey = -1;

	//블렌드 구조체 값 설정
	m_BlendFunc.AlphaFormat = 0;
	m_BlendFunc.BlendFlags = 0;
	m_BlendFunc.BlendOp = AC_SRC_OVER;
	m_BlendFunc.SourceConstantAlpha = 0;

	//얻어온 DC 해제
	ReleaseDC( g_hWnd, hdc );

	return S_OK;
}


HRESULT cImage::Init( const char* szFileName, DWORD dwWidth, DWORD dwHeight, COLORREF transColorKey )
{
	HDC hdc = GetDC( g_hWnd );

	//비트맵 가로세로 크기대입
	m_dwWidth = dwWidth;
	m_dwHeight = dwHeight;

	//BitmapDC 생성
	m_BitDC = CreateCompatibleDC( hdc );
	//Bitmap 불러온다.
	m_NewBit = (HBITMAP) LoadImage( g_hInst, szFileName, IMAGE_BITMAP, dwWidth, dwHeight, LR_LOADFROMFILE );
	//BitmapDC 의 표면을 불러온 Bitmap 으로 하고 이전 표면 정보를 m_OldBit 가....
	m_OldBit = (HBITMAP) SelectObject( m_BitDC, m_NewBit );

	//Blend용 빈 BitmapDC 생성
	m_BlendDC = CreateCompatibleDC( hdc );
	m_NewBlendBit = CreateCompatibleBitmap( hdc, dwWidth, dwHeight );
	m_OldBlendBit = (HBITMAP) SelectObject( m_BlendDC, m_NewBlendBit );

	//얻어온 DC 해제
	ReleaseDC( g_hWnd, hdc );

	//블렌드 구조체 값 설정
	m_BlendFunc.AlphaFormat = 0;
	m_BlendFunc.BlendFlags = 0;
	m_BlendFunc.BlendOp = AC_SRC_OVER;
	m_BlendFunc.SourceConstantAlpha = 0;

	//컬러 키값 대입
	m_TransColorKey = transColorKey;

	return S_OK;
}

//비트맵 해제 함수
void cImage::Release( void )
{
	//Bitmap 해제
	DeleteObject( SelectObject( m_BitDC, m_OldBit ) );
	DeleteObject( SelectObject( m_BlendDC, m_OldBlendBit ) );

	//Bitmap DC 해제
	DeleteDC( m_BitDC );
	DeleteDC( m_BlendDC );
}

//비트맵 그리기
void cImage::Draw( HDC hdc, int x, int y )
{
	//뺄컬러키가 없다면
	if ( m_TransColorKey == -1 ) {
		BitBlt(
			hdc,			//Target DC
			x, y,			//Target DC 위치
			m_dwWidth, m_dwHeight,  //크리기 가로세로 량
			m_BitDC,		//Source DC
			0, 0,			//Source 시잣 위치
			SRCCOPY			//복사 옵션
		);
	}

	//뺄컬러가 있다면..
	else {
		GdiTransparentBlt(
			hdc,			//Target DC
			x, y,			//Target DC 위치
			m_dwWidth, m_dwHeight,  //크리기 가로세로 량
			m_BitDC,		//Source DC
			0, 0,			//Source 시작
			m_dwWidth, m_dwHeight,
			m_TransColorKey );
	}




}

void cImage::Draw( HDC hdc, int x, int y, int sourX, int sourY, int sourW, int sourH )
{
	//뺄컬러키가 없다면
	if ( m_TransColorKey == -1 ) {

		BitBlt(
			hdc,			//Target DC
			x, y,			//Target DC 위치
			sourW, sourH,   //그리기 가로세로 량
			m_BitDC,		//Source DC
			sourX, sourY,	//Source 시잣 위치
			SRCCOPY			//복사 옵션
		);
	}

	//뺄컬러가 있다면..
	else {

		GdiTransparentBlt(
			hdc,			//Target DC
			x, y,			//Target DC 위치
			sourW, sourH,   //크리기 가로세로 량
			m_BitDC,		//Source DC
			sourX, sourY,	//Source 시작
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

	//뺄컬러키가 없다면
	if ( m_TransColorKey == -1 ) {

		BitBlt(
			hdc,			//Target DC
			x, y,			//Target DC 위치
			sourW, sourH,   //그리기 가로세로 량
			m_BitDC,		//Source DC
			sourX, sourY,	//Source 시잣 위치
			SRCCOPY			//복사 옵션
		);
	}

	//뺄컬러가 있다면..
	else {

		GdiTransparentBlt(
			hdc,			//Target DC
			x, y,			//Target DC 위치
			sourW, sourH,   //크리기 가로세로 량
			m_BitDC,		//Source DC
			sourX, sourY,	//Source 시작
			sourW, sourH,
			m_TransColorKey );
	}
}


void cImage::DrawAlpha( HDC hdc, int x, int y, BYTE alpha )
{
	m_BlendFunc.SourceConstantAlpha = alpha;

	//컬러키값이 있을때....
	if ( m_TransColorKey != -1 ) {

		//1. hdc TargetDC 내용을 BlendDC 에 그린다.
		BitBlt(
			m_BlendDC,
			0, 0, m_dwWidth, m_dwHeight,
			hdc, x, y, SRCCOPY );

		//2. Sour의 내용을 BlendDC 에 그린다.
		GdiTransparentBlt(
			m_BlendDC,				//Target DC
			0, 0,					//Target DC 위치
			m_dwWidth, m_dwHeight,  //크리기 가로세로 량
			m_BitDC,				//Source DC
			0, 0,					//Source 시작
			m_dwWidth, m_dwHeight,
			m_TransColorKey );

		//3. BlendDC 의 내용을 hdc 에 Alpha 블렌드로 그린다.
		GdiAlphaBlend(
			hdc,
			x, y,
			m_dwWidth, m_dwHeight,
			m_BlendDC,
			0, 0,
			m_dwWidth, m_dwHeight,
			m_BlendFunc );
	}

	//컬러 키 값이 없을때..
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

	//컬러키값이 있을때....
	if ( m_TransColorKey != -1 ) {

		//1. hdc TargetDC 내용을 BlendDC 에 그린다.
		BitBlt(
			m_BlendDC,
			0, 0, sourW, sourH,
			hdc, x, y, SRCCOPY );

		//2. Sour의 내용을 BlendDC 에 그린다.
		GdiTransparentBlt(
			m_BlendDC,				//Target DC
			0, 0,					//Target DC 위치
			sourW, sourH,  //크리기 가로세로 량
			m_BitDC,				//Source DC
			sourX, sourY,					//Source 시작
			sourW, sourH,
			m_TransColorKey );

		//3. BlendDC 의 내용을 hdc 에 Alpha 블렌드로 그린다.
		GdiAlphaBlend(
			hdc,
			x, y,
			sourW, sourH,
			m_BlendDC,
			0, 0,
			sourW, sourH,
			m_BlendFunc );

	}

	//컬러 키 값이 없을때..
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

	//컬러키값이 있을때....
	if ( m_TransColorKey != -1 ) {

		//1. hdc TargetDC 내용을 BlendDC 에 그린다.
		BitBlt(
			m_BlendDC,
			0, 0, sourW, sourH,
			hdc, x, y, SRCCOPY );

		//2. Sour의 내용을 BlendDC 에 그린다.
		GdiTransparentBlt(
			m_BlendDC,				//Target DC
			0, 0,					//Target DC 위치
			sourW, sourH,  //크리기 가로세로 량
			m_BitDC,				//Source DC
			sourX, sourY,					//Source 시작
			sourW, sourH,
			m_TransColorKey );

		//3. BlendDC 의 내용을 hdc 에 Alpha 블렌드로 그린다.
		GdiAlphaBlend(
			hdc,
			x, y,
			sourW, sourH,
			m_BlendDC,
			0, 0,
			sourW, sourH,
			m_BlendFunc );

	}

	//컬러 키 값이 없을때..
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



//반복드로우
void cImage::LoopDraw( HDC hdc, int offsetX, int offsetY, RECT& rcDest )
{
	//그릴 이미지 시작 위치
	if ( offsetX < 0 ) offsetX = m_dwWidth + ( offsetX % m_dwWidth );
	if ( offsetY < 0 ) offsetY = m_dwHeight + ( offsetY % m_dwHeight );


	//그리기 총량
	int nDrawTotalWidth = rcDest.right - rcDest.left;
	int nDrawTotalHeight = rcDest.bottom - rcDest.top;

	//그려질 영역사각형
	RECT	rcHdc;

	//그릴 소스의 영역사각형
	RECT	rcSour;

	//그려진 가로 세로의 량
	int		nDrawW;
	int		nDrawH;

	//세로 
	for ( int y = 0; y < nDrawTotalHeight; y += nDrawH ) {

		//소스의 세로폭을 구하자.
		rcSour.top = ( y + offsetY ) % m_dwHeight;
		rcSour.bottom = m_dwHeight;

		//그리기 세로 량
		nDrawH = rcSour.bottom - rcSour.top;

		//넘어갔다면...
		if ( y + nDrawH > nDrawTotalHeight ) {

			//넘어간 량만큼 바텀을 위로 땡긴다.
			rcSour.bottom -= ( y + nDrawH ) - nDrawTotalHeight;

			//그리기 세로 량
			nDrawH = rcSour.bottom - rcSour.top;
		}

		//그리기 영역의 세로 폭을 구한다.
		rcHdc.top = rcDest.top + y;
		rcHdc.bottom = rcHdc.top + nDrawH;

		//가로 
		for ( int x = 0; x < nDrawTotalWidth; x += nDrawW ) {

			//소스의 가로폭을 구하자.
			rcSour.left = ( x + offsetX ) % m_dwWidth;
			rcSour.right = m_dwWidth;

			//그리기 가로 량
			nDrawW = rcSour.right - rcSour.left;

			//넘어갔다면...
			if ( x + nDrawW > nDrawTotalWidth ) {

				//넘어간 량만큼 오른쪽을 왼쪽으로 땡긴다.
				rcSour.right -= ( x + nDrawW ) - nDrawTotalWidth;

				//그리기 가로 량
				nDrawW = rcSour.right - rcSour.left;
			}

			//그리기 영역의 세로 폭을 구한다.
			rcHdc.left = rcDest.left + x;
			rcHdc.right = rcHdc.left + nDrawW;


			//그린다
			this->Draw( hdc, rcHdc.left, rcHdc.top, rcSour );
		}
	}
}


void cImage::LoopDrawAlpha( HDC hdc, int offsetX, int offsetY, RECT& rcDest, BYTE alpha )
{
	//그릴 이미지 시작 위치
	if ( offsetX < 0 ) offsetX = m_dwWidth + ( offsetX % m_dwWidth );
	if ( offsetY < 0 ) offsetY = m_dwHeight + ( offsetY % m_dwHeight );




	//그리기 총량
	int nDrawTotalWidth = rcDest.right - rcDest.left;
	int nDrawTotalHeight = rcDest.bottom - rcDest.top;

	//그려질 영역사각형
	RECT	rcHdc;

	//그릴 소스의 영역사각형
	RECT	rcSour;

	//그려진 가로 세로의 량
	int		nDrawW;
	int		nDrawH;

	//세로 
	for ( int y = 0; y < nDrawTotalHeight; y += nDrawH ) {

		//소스의 세로폭을 구하자.
		rcSour.top = ( y + offsetY ) % m_dwHeight;
		rcSour.bottom = m_dwHeight;

		//그리기 세로 량
		nDrawH = rcSour.bottom - rcSour.top;

		//넘어갔다면...
		if ( y + nDrawH > nDrawTotalHeight ) {

			//넘어간 량만큼 바텀을 위로 땡긴다.
			rcSour.bottom -= ( y + nDrawH ) - nDrawTotalHeight;

			//그리기 세로 량
			nDrawH = rcSour.bottom - rcSour.top;
		}

		//그리기 영역의 세로 폭을 구한다.
		rcHdc.top = rcDest.top + y;
		rcHdc.bottom = rcHdc.top + nDrawH;

		//가로 
		for ( int x = 0; x < nDrawTotalWidth; x += nDrawW ) {

			//소스의 가로폭을 구하자.
			rcSour.left = ( x + offsetX ) % m_dwWidth;
			rcSour.right = m_dwWidth;

			//그리기 가로 량
			nDrawW = rcSour.right - rcSour.left;

			//넘어갔다면...
			if ( x + nDrawW > nDrawTotalWidth ) {

				//넘어간 량만큼 오른쪽을 왼쪽으로 땡긴다.
				rcSour.right -= ( x + nDrawW ) - nDrawTotalWidth;

				//그리기 가로 량
				nDrawW = rcSour.right - rcSour.left;
			}

			//그리기 영역의 세로 폭을 구한다.
			rcHdc.left = rcDest.left + x;
			rcHdc.right = rcHdc.left + nDrawW;


			//그린다
			this->DrawAlpha( hdc, rcHdc.left, rcHdc.top, rcSour, alpha );
		}
	}
}




///////////////////////////////////////////////////////////////////////////
