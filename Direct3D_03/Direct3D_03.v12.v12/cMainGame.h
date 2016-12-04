#pragma once

class cImage;			//전방선언, 이런 클래스가 있다.


class cMainGame
{
private:
	cScene*					m_pNowScene;


public:
	cMainGame( void );
	~cMainGame( void );

	//셋팅
	HRESULT Init( void );

	//해제
	void Release();

	//업데이트
	void Update();

	//드로우
	void Draw();


public:

	//프로시져
	LRESULT MainProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );

};
