#pragma once

class cImage;			//���漱��, �̷� Ŭ������ �ִ�.
class cScene;

class cMainGame
{
private:
	cScene*					m_pNowScene;


public:
	cMainGame( void );
	~cMainGame( void );

	//����
	HRESULT Init( void );

	//����
	void Release();

	//������Ʈ
	void Update();

	//��ο�
	void Draw();


public:

	//���ν���
	LRESULT MainProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );

};
