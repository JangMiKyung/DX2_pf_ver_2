// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.


//필요한 헤더 (DirectSDK 설치폴더/Include)
//d3dx9.h
//d3d9.h
//#include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/d3d9.h"
//#include "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/d3dx9.h"

//(DirectSDK 설치폴더/Include) 경로가 프로젝트의 System 폴더로 등록이 되어있다면 < > 로도 포함가능
#include <d3d9.h>
#include <d3dx9.h>

//System 폴더 (DirectSDK 설치폴더/Include) 등록 방법
//솔루션탐색기에서 프로젝트 우클릭
//속성
//구성속성의 VC++ 디렉터리 항목선택
//창의 우측을봐라.
//포함디렉토리칸의 오른속문자열라인을 선택하면 나오는 아래화살표 클릭
//<편집..> 문자를클릭
//포함디렉토리 창이 열리면서 우측 상단의 폴더추가 아이콘클릭
//클릭해서 나오는 빈간 오른쪽 ... 버튼 눌러
//탐색기 나오면서 (DirectSDK 설치폴더/Include) 선택


//필요라이브러리 (DirectSDK 설치폴더/Lib/x86)
//d3dx9.lib
//d3d9.lib
//#pragma comment( lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/d3d9.lib" )
//#pragma comment( lib, "C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Lib/x86/d3dx9.lib" )
#pragma comment( lib, "d3d9.lib" )
#pragma comment( lib, "d3dx9.lib" )


//System 폴더 (DirectSDK 설치폴더/Lib/x86) 등록 방법
//솔루션탐색기에서 프로젝트 우클릭
//속성
//구성속성의 VC++ 디렉터리 항목선택
//창의 우측을봐라.
//라이브러리 디렉토리칸의 오른속문자열라인을 선택하면 나오는 아래화살표 클릭
//<편집..> 문자를클릭
//포함디렉토리 창이 열리면서 우측 상단의 폴더추가 아이콘클릭
//클릭해서 나오는 빈간 오른쪽 ... 버튼 눌러
//탐색기 나오면서 (DirectSDK 설치폴더/Lib/x86) 선택





//
// 매크로 상수
//
#define WINSIZE_X	1280		//초기 윈도우 가로 크기
#define WINSIZE_Y	800			//초기 윈도우 세로 크기
#define WINPOS_X	0			//초기 윈도우 시작 위치 X
#define WINPOS_Y	0			//초기 윈도우 시작 위치 Y
#define WIN_CLASS	"Windows Exam"		//윈도우 클레스 이름
#define WIN_TITLE	"Windows Exam"		//윈도우 타이틀 이름
#define WIN_STYLE	WS_OVERLAPPEDWINDOW	//윈도우 형태


#define ONE_RAD 0.017453f				//라디안1 동


//
// 매크로 함수
//
#define SAFE_DELETE(p)		{ if(p){ delete (p); (p) = NULL; } }
#define SAFE_DELETE_ARR(p)	{ if(p){ delete[] (p); (p) = NULL; } }
#define SAFE_RELEASE(p)		{ if(p){ (p)->Release(); (p) = NULL; } }

//
// 전역 변수
//
extern HWND			g_hWnd;		//윈도우 핸들 ( 자신의 프로그램에서 돌고있는 윈도우 번호 )
extern HINSTANCE	g_hInst;	//프로그램 인스턴스 핸들 ( OS 가 부여한 프로그램 번호 )


//
// 매니져
//
#include "MyUtil.h"

using namespace MyUtil;


#include "cTimeMgr.h"
#include "cKeyMgr.h"
#include "cLogMgr.h"


//
// 씬 
//
#include "cScene.h"