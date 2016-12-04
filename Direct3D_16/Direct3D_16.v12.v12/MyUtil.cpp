#include "StdAfx.h"
#include "MyUtil.h"



namespace MyUtil{

	DWORD MaxAnisotropic;				//이방성 필터링 최대 단계

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

		//그래픽 카드에서 지원되는 이방성 필터링 최대 단계
		MaxAnisotropic = caps.MaxAnisotropy;

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

	float InverseLerp(float head, float rear, float value)
	{
		float delta = rear - head;
		float inValue = value - head;

		if (inValue < 0)
			inValue = 0;

		return inValue / delta;
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


	void SetRandomSeed()
	{
		srand(GetTickCount());
	}

	float RandomFloatRange(float min, float max)
	{
		// 0 ~ 99999
		// 0 ~ 1 까지의 실수

		float factor = (rand() % 1001) * 0.001f;

		float delta = (max - min);

		float result = (delta * factor) + min;

		return result;

	}

	int RandomIntRange(int min, int max)
	{
		int delta = max - min;

		return (rand() % delta) + min;
	}





	//정점의 노말을 계산해주는 함수
	void ComputeNormal(
		D3DXVECTOR3* pNorams,			//결과로 받을 노말백터 배열 포인터 ( 노말벡터의 배열의 수는 정점 배열의 수와 같다 )
		const D3DXVECTOR3* pVertices,	//계산에 사용된 정점위치 배열 포인터
		int verticesNum,				//정점 갯수
		const DWORD* pIndices,			//인덱스 배열포인터 ( 일단 인덱스는 DWORD 로 대동단결 )
		int indicesNum					//인덱스 배열포인터 갯수
		)
	{
		//노말 결과 털고 시작
		ZeroMemory(pNorams, sizeof(D3DXVECTOR3) * verticesNum);

		//삼각형 갯수
		DWORD triNum = indicesNum / 3;

		for (DWORD i = 0; i < triNum; i++)
		{
			//해당 삼각형의 정점 인덱스
			DWORD i0 = pIndices[(i * 3) + 0];
			DWORD i1 = pIndices[(i * 3) + 1];
			DWORD i2 = pIndices[(i * 3) + 2];

			//정점 3개
			D3DXVECTOR3 v0 = pVertices[i0];
			D3DXVECTOR3 v1 = pVertices[i1];
			D3DXVECTOR3 v2 = pVertices[i2];

			//Edge
			D3DXVECTOR3 edge1 = v1 - v0;
			D3DXVECTOR3 edge2 = v2 - v0;

			//Cross
			D3DXVECTOR3 cross;
			D3DXVec3Cross(&cross, &edge1, &edge2);

			//Normal
			D3DXVECTOR3 normal;
			D3DXVec3Normalize(&normal, &cross);

			//증가 시킨다.
			pNorams[i0] += normal;
			pNorams[i1] += normal;
			pNorams[i2] += normal;
		}

		//최종 적으로 Normalvector 정규화한다.
		for (DWORD i = 0; i < verticesNum; i++)
		{
			D3DXVec3Normalize(&pNorams[i], &pNorams[i]);
		}
	}




	//탄젠트와 바이노말 계산
	void ComputeTangentAngBinormal(
		D3DXVECTOR3* outTangets,			//결과 Tangent 배열
		D3DXVECTOR3* outBinormals,			//결과 Binormal 배열
		const D3DXVECTOR3* pPositions,		//정점 위치 배열
		const D3DXVECTOR3* pNormals,		//정점 노말 배열
		const D3DXVECTOR2* pUVs,			//정점의 UV 배열
		const DWORD* pIndices,				//인덱스 배열
		DWORD NumTris,						//삼각형 갯수		
		DWORD NumVertices 					//정점 갯수
		)
	{
		//탄젠트 바이노말 턴다
		ZeroMemory(outTangets, sizeof(D3DXVECTOR3) * NumVertices);
		ZeroMemory(outBinormals, sizeof(D3DXVECTOR3) * NumVertices);

		//임시 Tangent Binormal 배열
		D3DXVECTOR3* pTangents = new D3DXVECTOR3[NumVertices];
		D3DXVECTOR3* pBinormals = new D3DXVECTOR3[NumVertices];

		//임시 Tangent Binormal 배열 턴다
		ZeroMemory(pTangents, sizeof(D3DXVECTOR3) * NumVertices);
		ZeroMemory(pBinormals, sizeof(D3DXVECTOR3) * NumVertices);

		//일단 삼각형 수대로....
		for (DWORD a = 0; a < NumTris; a++)
		{
			//해당 삼각형의 인덱스
			DWORD i0 = pIndices[a * 3 + 0];
			DWORD i1 = pIndices[a * 3 + 1];
			DWORD i2 = pIndices[a * 3 + 2];

			//해당 삼각형의 정점위치
			D3DXVECTOR3 p0 = pPositions[i0];
			D3DXVECTOR3 p1 = pPositions[i1];
			D3DXVECTOR3 p2 = pPositions[i2];

			//해당 삼각형의 UV
			D3DXVECTOR2 uv0 = pUVs[i0];
			D3DXVECTOR2 uv1 = pUVs[i1];
			D3DXVECTOR2 uv2 = pUVs[i2];

			//각변의 Edge
			D3DXVECTOR3 edge1 = p1 - p0;
			D3DXVECTOR3 edge2 = p2 - p0;

			//UV Edge
			D3DXVECTOR2 uvEdge1 = uv1 - uv0;
			D3DXVECTOR2 uvEdge2 = uv2 - uv0;

			// 위의 정보로 다음과 같은 공식이 성립
			// edge1 = ( uvEdge1.x ) * Tangent + ( uvEdge1.y ) * Binormal;
			// edge2 = ( uvEdge2.x ) * Tangent + ( uvEdge2.y ) * Binormal;

			// 다음과 같이 치환
			// E1 = edge1;
			// E2 = edge2;
			// U1 = uvEdge1.x;
			// V1 = uvEdge1.y;
			// U2 = uvEdge2.x;
			// V2 = uvEdge2.y;
			// T = Tangent;
			// B = Binormal;

			// E1 = U1 * T + V1 * B;
			// E2 = U2 * T + V2 * B;

			// | E1 |   | U1 , V1 | | T |
			// |    | = |         | |   |
			// | E2 |   | U2 , V2 | | B |

			// | T |          1        |  V2 , -V1 | | E1 |
			// |   | = --------------- |           | |    |
			// | B |    U1*V2 - V1*U2  | -U2 ,  U1 | | E2 |

			// R = 1 / U1*V2 - V1*U2;

			// T = ( ( E1 * V2 ) + ( E2 * -V1 ) ) * R
			// B = ( ( E1 * -U2 ) + ( E2 * U1 ) ) * R

			float r =
				1.0f / ((uvEdge1.x * uvEdge2.y) - (uvEdge1.y * uvEdge2.x));

			//Tangent
			D3DXVECTOR3 t = ((edge1 * uvEdge2.y) + (edge2 * -uvEdge1.y)) * r;
			D3DXVec3Normalize(&t, &t);

			//Binormal 
			D3DXVECTOR3 b = ((edge1 * -uvEdge2.x) + (edge2 * uvEdge1.x)) * r;
			D3DXVec3Normalize(&b, &b);

			//탄젠트 바이노말 임시 배열에 추가
			pTangents[i0] += t;
			pTangents[i1] += t;
			pTangents[i2] += t;
			pBinormals[i0] += b;
			pBinormals[i1] += b;
			pBinormals[i2] += b;
		}



		//Binormal Tangent 노말에 직교화
		for (int i = 0; i < NumVertices; i++){

			D3DXVECTOR3 n = pNormals[i];

			//Tangent 그람슈미트 직교
			D3DXVECTOR3 t = pTangents[i] -
				(D3DXVec3Dot(&pTangents[i], &n) * n);
			D3DXVec3Normalize(&t, &t);

			//노말과 직교화된 T 와 외적하여 B
			D3DXVECTOR3 b;
			D3DXVec3Cross(&b, &n, &t);
			D3DXVec3Normalize(&b, &b);

			outTangets[i] = t;
			outBinormals[i] = b;
		}

		//임시 배열 삭제
		SAFE_DELETE_ARR(pTangents);
		SAFE_DELETE_ARR(pBinormals);



	}


	//플룻의 비트값을 손실하지 않은체 DWORD 형으로 변환
	DWORD FloatToDWORD(float f)
	{
		float* pFloat = &f;

		DWORD* pDword = (DWORD*)pFloat;

		return *pDword;
	}

}