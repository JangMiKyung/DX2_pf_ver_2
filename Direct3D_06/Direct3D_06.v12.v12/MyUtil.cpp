#include "StdAfx.h"
#include "MyUtil.h"



namespace MyUtil{

	LPDIRECT3DDEVICE9 Device;			//���� Device

	//����̽� �����Լ�
	HRESULT CreateDevice()
	{
		//
		// Direct3D �ʱ�ȭ
		//

		//1. Device �� ������ IDirect3D9 ��ü�� ��´�.

		//IDirect3D9* d3d9;
		LPDIRECT3D9 d3d9;
		d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

		//IDirect3D9 ��ü��Ⱑ �����ϸ� NULL �� ������
		if (d3d9 == NULL)
		{
			MessageBox(g_hWnd, "Direct3DCreate9() - FAILED", "FAILED", MB_OK);
			return E_FAIL;
		}

		//2. �ϵ���� ������ ������ �ͼ� �ڽ��� ���� ���μ��� Ÿ���� ������
		D3DCAPS9	caps;
		//Direct3D9 ��ü�� ���� ���� ī���� �ϵ���� ������ ������ �´�.
		d3d9->GetDeviceCaps(
			D3DADAPTER_DEFAULT,		//�� ���÷��� �׷��� ī�� �׳� D3DADAPTER_DEFAULT
			D3DDEVTYPE_HAL,			//����̽�Ÿ�� ���� �׳� D3DDEVTYPE_HAL
			&caps					//����̽� ������ �޾ƿ� D3DCAPS9 ������
			);

		//������� ó������� ������ �÷��� ��
		DWORD vertexProcessing = 0;

		//���� ó���� ���� ���� ó���� �ϵ����� �����ϴ°�?
		if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
			vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
		else
			vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;


		//3. D3DPRESENT_PARAMETERS ����ü ������ ����
		//���� �̷��� Device �� ����ٴٶ�� ����

		D3DPRESENT_PARAMETERS d3dpp;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;				//���� ���ۿ� ���׽� ���� ũ�� ���� 24bit ���ٽ� ���� 8 ��Ʈ
		d3dpp.BackBufferCount = 1;						//����۴� �Ѱ�
		d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;			//����� ���� ���� ����8��Ʈ,����8��Ʈ,�׸�8��Ʈ,���8��Ʈ
		d3dpp.BackBufferHeight = WINSIZE_Y;				//����� �ȼ� ���� ũ��
		d3dpp.BackBufferWidth = WINSIZE_X;				//����� �ȼ� ���� ũ��
		d3dpp.EnableAutoDepthStencil = true;						//�ڵ� ���̹��� ��� ���� ( �׳� true )
		d3dpp.Flags = 0;						//�ϴ� 0
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//ȭ�� �ֻ��� ( �׳� D3DPRESENT_RATE_DEFAULT ����� �ֻ����� ���Ͻ� )
		d3dpp.hDeviceWindow = g_hWnd;					//Device �� ��µ� ������ �ڵ�
		d3dpp.MultiSampleQuality = 0;						//��Ƽ ���ø� �� 
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;		//��Ƽ ���ø� Ÿ�� ( D3DMULTISAMPLE_NONE ��Ƽ�˸��ƽ� ������ ) 
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;	//ȭ�� ���� ���� ( �׳� D3DPRESENT_INTERVAL_ONE ����� �ֻ����� ���Ͻ� )
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//ȭ�� ���� ü�� ��� ( D3DSWAPEFFECT_DISCARD )
		d3dpp.Windowed = true;


		//4. Device ����

		//Direct3D9 ��ü�� ���ؼ� Device �� �����Ѵ�.

		HRESULT result = d3d9->CreateDevice(
			D3DADAPTER_DEFAULT,			//�� ���÷��� �׷��� ī�� �׳� D3DADAPTER_DEFAULT
			D3DDEVTYPE_HAL,				//����̽�Ÿ�� ���� �׳� D3DDEVTYPE_HAL
			g_hWnd,						//����̽��� ����� ������ �ڵ�
			vertexProcessing,			//���� ó�� ��Ŀ� ���� �÷���
			&d3dpp,						//�տ��� ������ D3DPRESENT_PARAMETERS ����ü ������
			&Device);				//���� ����̽� ����������

		if (FAILED(result))
		{
			//����̽� ��������

			//d3d9 ��ü �������Ѵ�
			d3d9->Release();
			d3d9 = NULL;


			MessageBox(g_hWnd, "CreateDevice() - FAILED", "FAILED", MB_OK);
			return E_FAIL;
		}


		//����̽� �������� 

		//���̻� ������ �ʴ� d3d9 ��ü �������Ѵ�
		d3d9->Release();
		d3d9 = NULL;

		return S_OK;
	}

	//����̽� �����Լ�
	void ReleaseDevice()
	{

		//����̽� ����
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
		GetCursorPos(&mousePos); //���콺 Screen ���� ��ġ�� ����ش�.

		//Screen ���� ��ġ�� Client ������ ������ �ٲ۴�
		ScreenToClient(g_hWnd, &mousePos);

		return mousePos;
	}

	void SetMousePos(int x, int y)
	{
		POINT setMousePos;
		setMousePos.x = x;
		setMousePos.y = y;

		//Client ���� ��ġ�� Screen ������ ������ �ٲ۴�
		ClientToScreen(g_hWnd, &setMousePos);

		//SetCursorPos �� Screen ���� ��ġ�� �����ؾ� �Ѵ�.
		SetCursorPos(setMousePos.x, setMousePos.y);
	}

	//value ���� min max ���̷� ������
	float Clamp(float value, float min, float max)
	{
		if (value < min)
			return min;
		else if (value > max)
			return max;

		return value;
	}
	
	//value ���� 0~1 ���̷� ������.
	float Clamp01(float value)
	{
		if (value < 0.0f)
			return 0.0f;
		if (value > 1.0f)
			return 1.0f;

		return value;
	}

	//value ���� -1 ~ 1 ���̷� ������.
	float ClampMinusOnePlusOne(float value)
	{
		if (value < -1.0f)
			return -1.0f;
		if (value > 1.0f)
			return 1.0f;

		return value;
	}

	//float ���� ����
	float Lerp(float from, float to, float t)
	{
		t = Clamp01(t);

		float delta = to - from;

		return (delta * t) + from;
	}

	//���� ���� ����
	D3DXVECTOR3 VecLerp(const D3DXVECTOR3& from, const D3DXVECTOR3& to, float t)
	{
		D3DXVECTOR3 result(
			Lerp(from.x, to.x, t),
			Lerp(from.y, to.y, t),
			Lerp(from.z, to.z, t));

		return result;
	}

}