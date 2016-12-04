// APIExam_005.cpp : ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"
#include "cMainGame.h"

//
// ���� ����
//
HWND		g_hWnd;		//������ �ڵ� ( �ڽ��� ���α׷����� �����ִ� ������ ��ȣ )
HINSTANCE	g_hInst;	//���α׷� �ν��Ͻ� �ڵ� ( OS �� �ο��� ���α׷� ��ȣ )

						//���� ���� ��ü
cMainGame	g_MainGame;


// �ݹ� ���ν��� �Լ� ������ Ÿ�� 
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam );


//
// ������ ���� �Լ�
//
int APIENTRY WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdParam, int nCmdShow )
{
	//OS ���˷��� ���α׷� �ν��Ͻ� ��ȣ�� ���������� ����Ѵ�.
	g_hInst = hInstance;

	//������ Ŭ���� ���� ����
	//���� �̷��� ������ ����ڴ� ��� ����
	WNDCLASS WndClass;
	WndClass.cbClsExtra = 0;			//�����쿡�� ����ϴ� ������ �޸𸮼���( �׳� 0 �̴�  �Ű澲������ )
	WndClass.cbWndExtra = 0;			//�����쿡�� ����ϴ� ������ �޸𸮼���( �׳� 0 �̴�  �Ű澲������ )
	WndClass.hbrBackground = (HBRUSH) GetStockObject( WHITE_BRUSH );		//������ ������
	WndClass.hCursor = LoadCursor( NULL, IDC_ARROW );			//�������� Ŀ����� ����
	WndClass.hIcon = LoadIcon( NULL, IDI_APPLICATION );		//����������ܸ�� ����
	WndClass.hInstance = hInstance;				//���α׷��ν��Ͻ��ڵ� 
	WndClass.lpfnWndProc = (WNDPROC) WndProc;			//������ ���ν��� �Լ� ������
	WndClass.lpszMenuName = NULL;						//�޴��̸� ������ NULL
	WndClass.lpszClassName = WIN_CLASS;				//���� �ۼ��ϰ� �ִ� ������ Ŭ������ �̸�
	WndClass.style = CS_HREDRAW | CS_VREDRAW;	//������ �׸��� ��� ���� ( ����� ����ɶ� ȭ�鰻�� CS_HREDRAW | CS_VREDRAW )

												//������ �ۼ��� ������ Ŭ�������� ���
	RegisterClass( &WndClass );

	//������ ����
	//������ ������ �ڵ��� �������� g_hWnd �� �޴´�.
	g_hWnd = CreateWindow(
		WIN_CLASS,				//�����Ǵ� �������� Ŭ�����̸�
		WIN_TITLE,				//������ Ÿ��Ʋ�ٿ� ��µǴ� �̸�
		WIN_STYLE,				//������ ��Ÿ��
		WINPOS_X,				//������ ���� ��ġ X 
		WINPOS_Y,				//������ ���� ��ġ Y
		WINSIZE_X,				//������ ���� ũ�� ( �۾������� ũ�Ⱑ �ƴ� )
		WINSIZE_Y,				//������ ���� ũ�� ( �۾������� ũ�Ⱑ �ƴ� )
		GetDesktopWindow(),		//�θ� ������ �ڵ� ( ���α׷����� �ֻ��� ������� NULL �Ǵ� GetDesktopWindow() )
		NULL,					//�޴� ID ( �ڽ��� ��Ʈ�� ��ü�� �������ΰ�� ��Ʈ�� ID �� ��	
		hInstance,				//�� �����찡 ���� ���α׷� �ν��Ͻ� �ڵ�
		NULL					//�߰� ���� NULL ( �Ű���� )
	);

	//WINSIZE_X �� WINSIZE_Y �� �۾������� ũ�⸦ 
	//������ �����츦 �����.

	//������� �۾������� ũ��
	RECT rcClient = {0, 0, WINSIZE_X, WINSIZE_Y};

	//�� : rcClient ũ��� �۾������� ����� �ʹ�.
	//WINAPI : �� �׷��ٸ� ������ ���¸� �˿����, ���� �޴��� �ִ��� �˿���� �׷��ٸ� ���� 
	// ��Ȯ�� ��������� ũ�⸦ rcClient �˷��ָ�. AdjustWindowRect �Լ��� ���.
	AdjustWindowRect( &rcClient, WIN_STYLE, FALSE );

	//�� : ��Ȯ�� Client �� ������ �˾����� WindowSize�� �����ؾ���....
	//WINAPI : �׷��ٸ�... SetWindowPos �Լ��� ����Ϸ�....
	SetWindowPos( g_hWnd,
		NULL,
		0, 0,		//��ġ�� 0, 0, ����...
		rcClient.right - rcClient.left,	//���� ũ��
		rcClient.bottom - rcClient.top,	//���� ũ��
		SWP_NOZORDER | SWP_NOMOVE		//�� : API �� ũ�⸸ ���Ҳ��ϱ� ���� ������,  ��ġ �� �����ض�...
	);


	//�޽��� ����ü
	MSG Message;
	ZeroMemory( &Message, sizeof( MSG ) );

	//���� �ʱ�ȭ �����ߴٸ�.....
	if ( SUCCEEDED( g_MainGame.Init() ) )
	{

		//������ ���� �����츦 ȭ�鿡 ����.
		ShowWindow( g_hWnd, nCmdShow );

		//
		// �޽��� ����
		//
		/*
		while( GetMessage( &Message, NULL, NULL, NULL ) )
		{
		TranslateMessage( &Message ); //���� Ű���� �� ���ڸ� �����Ͽ� WM_CHAR �޽����� �߻���Ų��.
		DispatchMessage( &Message );  //�޾ƿ� �޽��� ������ ������ ���ν��� �Լ��� �����Ų��.
		}
		*/


		//�޽�ť���� ���� �޽����� ����޽����� �ƴϸ� ��� ����
		while ( Message.message != WM_QUIT ) {

			//�޽���ť�� �޽��������� 
			//������ �������� PM_REMOVE �ɼ��������� ���޽�����
			//�����.
			//���� Message �� ���ٸ� false �� �����Ѵ�.
			if ( PeekMessage( &Message, NULL, NULL, NULL, PM_REMOVE ) ) {
				TranslateMessage( &Message ); //���� Ű���� �� ���ڸ� �����Ͽ� WM_CHAR �޽����� �߻���Ų��.
				DispatchMessage( &Message );  //�޾ƿ� �޽��� ������ ������ ���ν��� �Լ��� �����Ų��.
			}

			//������Ʈ 
			g_MainGame.Update();

			//��ο�
			g_MainGame.Draw();

		}

	}

	//����
	g_MainGame.Release();

	//��ϵ� ������ Ŭ���� ���� ( ���൵ �׸� �����൵ �׸�.. )
	UnregisterClass( WIN_CLASS, hInstance );

	//������ ���α׷� ����	
	return (int) Message.wParam;
}

//
// ������ ���ν��� �Լ� ( �޽��� ť���� �޾ƿ� �޽����� ó���Ѵ� )
//
LRESULT CALLBACK WndProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	return g_MainGame.MainProc( hWnd, iMessage, wParam, lParam );
}