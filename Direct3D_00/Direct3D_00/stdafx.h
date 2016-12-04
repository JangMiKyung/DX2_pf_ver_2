// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.


//�ʿ��� ��� (DirectSDK ��ġ����/Include)
//d3dx9.h
//d3d9.h
//#include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/d3d9.h"
//#include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/d3dx9.h"

//(DirectSDK ��ġ����/Include) ��ΰ� ������Ʈ�� System ������ ����� �Ǿ��ִٸ� < > �ε� ���԰���
#include <d3d9.h>
#include <d3dx9.h>

//System ���� (DirectSDK ��ġ����/Include) ��� ���
//�ַ��Ž���⿡�� ������Ʈ ��Ŭ��
//�Ӽ�
//�����Ӽ��� VC++ ���͸� �׸���
//â�� ����������.
//���Ե��丮ĭ�� �����ӹ��ڿ������� �����ϸ� ������ �Ʒ�ȭ��ǥ Ŭ��
//<����..> ���ڸ�Ŭ��
//���Ե��丮 â�� �����鼭 ���� ����� �����߰� ������Ŭ��
//Ŭ���ؼ� ������ �� ������ ... ��ư ����
//Ž���� �����鼭 (DirectSDK ��ġ����/Include) ����


//�ʿ���̺귯�� (DirectSDK ��ġ����/Lib/x86)
//d3dx9.lib
//d3d9.lib
//#pragma comment( lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/d3d9.lib" )
//#pragma comment( lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/d3dx9.lib" )
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )


//System ���� (DirectSDK ��ġ����/Lib/x86) ��� ���
//�ַ��Ž���⿡�� ������Ʈ ��Ŭ��
//�Ӽ�
//�����Ӽ��� VC++ ���͸� �׸���
//â�� ����������.
//���̺귯�� ���丮ĭ�� �����ӹ��ڿ������� �����ϸ� ������ �Ʒ�ȭ��ǥ Ŭ��
//<����..> ���ڸ�Ŭ��
//���Ե��丮 â�� �����鼭 ���� ����� �����߰� ������Ŭ��
//Ŭ���ؼ� ������ �� ������ ... ��ư ����
//Ž���� �����鼭 (DirectSDK ��ġ����/Lib/x86) ����





//
// ��ũ�� ���
//
#define WINSIZE_X	1280		//�ʱ� ������ ���� ũ��
#define WINSIZE_Y	800			//�ʱ� ������ ���� ũ��
#define WINPOS_X	0			//�ʱ� ������ ���� ��ġ X
#define WINPOS_Y	0			//�ʱ� ������ ���� ��ġ Y
#define WIN_CLASS	"Windows Exam"		//������ Ŭ���� �̸�
#define WIN_TITLE	"Windows Exam"		//������ Ÿ��Ʋ �̸�
#define WIN_STYLE	WS_OVERLAPPEDWINDOW	//������ ����


#define ONE_RAD 0.017453f				//����1 ��


//
// ��ũ�� �Լ�
//
#define SAFE_DELETE(p)		{ if(p){ delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARR(p)	{ if(p){ delete[] (p); (p) = NULL; } }
#define SAFE_RELEASE(p)		{ if(p){ (p)->Release(); (p) = NULL; } }

//
// ���� ����
//
extern HWND			g_hWnd;		//������ �ڵ� ( �ڽ��� ���α׷����� �����ִ� ������ ��ȣ )
extern HINSTANCE	g_hInst;	//���α׷� �ν��Ͻ� �ڵ� ( OS �� �ο��� ���α׷� ��ȣ )


//
// �Ŵ���
//
#include "MyUtil.h"

using namespace MyUtil;


#include "cTimeMgr.h"
#include "cKeyMgr.h"
#include "cLogMgr.h"


//
// �� 
//
#include "cScene.h"