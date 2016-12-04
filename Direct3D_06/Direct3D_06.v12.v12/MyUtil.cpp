#include "StdAfx.h"
#include "MyUtil.h"



namespace MyUtil{

	LPDIRECT3DDEVICE9 Device;			//전역 Device

	//디바이스 생성함수
	HRESULT CreateDevice()
	{
		//
		// Direct3D 초기화
		//

		//1. Device 를 생성할 IDirect3D9 객체를 얻는다.

		//IDirect3D9* d3d9;
		LPDIRECT3D9 d3d9;
		d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

		//IDirect3D9 객체얻기가 실패하면 NULL 이 참조됨
		if (d3d9 == NULL)
		{
			MessageBox(g_hWnd, "Direct3DCreate9() - FAILED", "FAILED", MB_OK);
			return E_FAIL;
		}

		//2. 하드웨어 정보를 가지고 와서 자신의 정점 프로세스 타입을 정하자
		D3DCAPS9	caps;
		//Direct3D9 객체를 통해 비디오 카드의 하드웨어 정보를 가지고 온다.
		d3d9->GetDeviceCaps(
			D3DADAPTER_DEFAULT,		//주 디스플레이 그래픽 카드 그냥 D3DADAPTER_DEFAULT
			D3DDEVTYPE_HAL,			//디바이스타입 설정 그냥 D3DDEVTYPE_HAL
			&caps					//디바이스 정보를 받아올 D3DCAPS9 포인터
			);

		//정점계산 처리방식을 지정할 플레그 값
		DWORD vertexProcessing = 0;

		//정점 처리와 정점 광원 처리를 하드웨어에서 지원하는가?
		if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
			vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		else
			vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;


		//3. D3DPRESENT_PARAMETERS 구조체 정보를 생성
		//내가 이러한 Device 를 만들겟다라는 정보

		D3DPRESENT_PARAMETERS d3dpp;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				//뎁스 버퍼와 스테실 버퍼 크기 뎁스 24bit 스텐실 버퍼 8 비트
		d3dpp.BackBufferCount = 1;						//백버퍼는 한개
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;			//백버퍼 색상 포맷 알파8비트,레드8비트,그린8비트,블루8비트
		d3dpp.BackBufferHeight = WINSIZE_Y;				//백버퍼 픽셀 높이 크기
		d3dpp.BackBufferWidth = WINSIZE_X;				//백버퍼 픽셀 가로 크기
		d3dpp.EnableAutoDepthStencil = true;						//자동 깊이버퍼 사용 여부 ( 그냥 true )
		d3dpp.Flags = 0;						//일단 0
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//화면 주사율 ( 그냥 D3DPRESENT_RATE_DEFAULT 모니터 주사율과 동일시 )
		d3dpp.hDeviceWindow = g_hWnd;					//Device 가 출력될 윈도우 핸들
		d3dpp.MultiSampleQuality = 0;						//멀티 샘플링 질 
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;		//멀티 샘플링 타입 ( D3DMULTISAMPLE_NONE 안티알리아싱 사용안함 ) 
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;	//화면 전송 간격 ( 그냥 D3DPRESENT_INTERVAL_ONE 모니터 주사율과 동일시 )
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//화면 스왑 체인 방식 ( D3DSWAPEFFECT_DISCARD )
		d3dpp.Windowed = true;


		//4. Device 생성

		//Direct3D9 객체를 통해서 Device 를 생성한다.

		HRESULT result = d3d9->CreateDevice(
			D3DADAPTER_DEFAULT,			//주 디스플레이 그래픽 카드 그냥 D3DADAPTER_DEFAULT
			D3DDEVTYPE_HAL,				//디바이스타입 설정 그냥 D3DDEVTYPE_HAL
			g_hWnd,						//디바이스를 사용할 윈도우 핸들
			vertexProcessing,			//정점 처리 방식에 대한 플레그
			&d3dpp,						//앞에서 정의한 D3DPRESENT_PARAMETERS 구조체 포인터
			&Device);				//얻어올 디바이스 더블포인터

		if (FAILED(result))
		{
			//디바이스 생성실패

			//d3d9 객체 날려야한다
			d3d9->Release();
			d3d9 = NULL;


			MessageBox(g_hWnd, "CreateDevice() - FAILED", "FAILED", MB_OK);
			return E_FAIL;
		}


		//디바이스 생성성공 

		//더이상 사용되지 않는 d3d9 객체 날려야한다
		d3d9->Release();
		d3d9 = NULL;

		return S_OK;
	}

	//디바이스 해제함수
	void ReleaseDevice()
	{

		//디바이스 해제
		/*
		if( this->m_pDevice != NULL )
		{
		this->m_pDevice->Release();
		this->m_pDevice = NULL;
		}*/
		SAFE_RELEASE(Device);



	}

	POINT GetMousePos()
	{
		POINT mousePos;
		GetCursorPos(&mousePos); //마우스 Screen 상의 위치를 얻어준다.

		//Screen 상의 위치를 Client 영역의 범위로 바꾼다
		ScreenToClient(g_hWnd, &mousePos);

		return mousePos;
	}

	void SetMousePos(int x, int y)
	{
		POINT setMousePos;
		setMousePos.x = x;
		setMousePos.y = y;

		//Client 상의 위치를 Screen 영역의 범위로 바꾼다
		ClientToScreen(g_hWnd, &setMousePos);

		//SetCursorPos 는 Screen 상의 위치로 셋팅해야 한다.
		SetCursorPos(setMousePos.x, setMousePos.y);
	}

	//value 값을 min max 사이로 맞춰줌
	float Clamp(float value, float min, float max)
	{
		if (value < min)
			return min;
		else if (value > max)
			return max;

		return value;
	}
	
	//value 값을 0~1 사이로 맞춰줌.
	float Clamp01(float value)
	{
		if (value < 0.0f)
			return 0.0f;
		if (value > 1.0f)
			return 1.0f;

		return value;
	}

	//value 값을 -1 ~ 1 사이로 맞춰줌.
	float ClampMinusOnePlusOne(float value)
	{
		if (value < -1.0f)
			return -1.0f;
		if (value > 1.0f)
			return 1.0f;

		return value;
	}

	//float 선형 보간
	float Lerp(float from, float to, float t)
	{
		t = Clamp01(t);

		float delta = to - from;

		return (delta * t) + from;
	}

	//벡터 선형 보간
	D3DXVECTOR3 VecLerp(const D3DXVECTOR3& from, const D3DXVECTOR3& to, float t)
	{
		D3DXVECTOR3 result(
			Lerp(from.x, to.x, t),
			Lerp(from.y, to.y, t),
			Lerp(from.z, to.z, t));

		return result;
	}

}