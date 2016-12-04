#include "StdAfx.h"
#include "cMainGame.h"
#include "cImage.h"

#include "cScene_00.h"
#include "cScene_01_Correct.h"
#include "cScene_02_UseXMesh.h"

cMainGame::cMainGame( void )
{
}

cMainGame::~cMainGame( void )
{
}

//����
HRESULT cMainGame::Init( void )
{
	//����̽� ����
	CreateDevice();

	//�Ŵ��� ����
	TIME_MGR->Init();
	LOG_MGR->Init( LOG_WINDOW | LOG_FILE, g_hWnd, "Dump" );
	DXFONT_MGR->Init(Device);
	GIZMO_MGR->Init(Device);

	DXFONT_MGR->addStyle(Device, "������", "���긲", 30.f);


	//������
	m_pNowScene = new cScene_02_UseXMesh();

	//���ʱ�ȭ
	if (FAILED(m_pNowScene->Init()))		//���ϰ��� E_FAIL;
		return E_FAIL;

	//return E_FAIL;
	return S_OK;
}

//����
void cMainGame::Release()
{

	//������
	m_pNowScene->Release();
	SAFE_DELETE( m_pNowScene );

	//�Ŵ��� ����
	TIME_MGR->Release();
	cTimeMgr::ReleaseInstance();
	KEY_MGR->Release();
	cKeyMgr::ReleaseInstance();
	LOG_MGR->Release();
	cLogMgr::ReleaseInstance();
	GIZMO_MGR->Release();
	cGizmoManager::ReleaseInstance();

	RESOURCE_TEXTURE->ClearResource();
	cResourceMgr_Texture::ReleaseInstance();
	RESOURCE_FX->ClearResource();
	cResourceMgr_ShaderFX::ReleaseInstance();
	RESOURCE_STATICXMESH->ClearResource();
	cResourceMgr_XStaticMesh::ReleaseInstance();


	//����̽� ����
	ReleaseDevice();
}

//������Ʈ
void cMainGame::Update()
{
	//Ÿ�ӸŴ��� ������Ʈ
	TIME_MGR->UpdateTime( 60.0f );

	//�������� ���� �ð�
	double timeDelta = TIME_MGR->GetFrameDeltaSec();

	//��������Ʈ
	m_pNowScene->Update( static_cast<float>( timeDelta ) );
}

//��ο�
void cMainGame::Draw()
{
	//���۸� û���Ѵ�
	HRESULT result = Device->Clear(
		0,							//û���� ������ D3DRECT �迭 ����		( ��ü Ŭ���� 0 )
		NULL,						//û���� ������ D3DRECT �迭 ������		( ��ü Ŭ���� NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,		//û�ҵ� ���� �÷��� ( D3DCLEAR_TARGET �÷�����, D3DCLEAR_ZBUFFER ���̹���, D3DCLEAR_STENCIL ���ٽǹ���
		0xff123434,					//�÷����۸� û���ϰ� ä���� ����( 0xAARRGGBB )
		1.0f,						//���̹��۸� û���Ұ� ( 0 ~ 1, 0 �� ī�޶󿡼� ���ϰ���� 1 �� ī�޶󿡼� ���� �� )
		0							//���ٽ� ���۸� ä�ﰪ
	);

	//���� û�Ұ� �����ߴٸ�.......
	if ( SUCCEEDED( result ) )
	{
		//����̽� ������ ���� ���
		Device->BeginScene();

		//������ ���

		//������
		m_pNowScene->Render();

#ifdef _DEBUG	//����� ��忡���� ����
		//����̽� ������ ���� ���
		//���� �׸���
		GIZMO_MGR->WorldGrid(1, 10);

		//Ÿ������ ���
		TIME_MGR->DrawTimeInfo();
#endif

		//����̽� ������ ���� ���
		Device->EndScene();

		//�������� ���۳����� ȭ�鿡 ����
		Device->Present( 0, 0, 0, 0 );
	}

}



//////////////////////////////////////////////////////////////////////////

//���� ���ν��� �Լ�
LRESULT cMainGame::MainProc( HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam )
{
	switch ( iMessage )
	{
	case WM_KEYDOWN:
		switch ( wParam )
		{
		case VK_ESCAPE:
			DestroyWindow( hWnd );
			break;
		}
		break;

	case WM_DESTROY:			//�����찡 �ı��ȴٸ�..
		PostQuitMessage( 0 );	//���α׷� ���� ��û ( �޽��� ������ ���������� �ȴ� )
		break;
	}
	return DefWindowProc( hWnd, iMessage, wParam, lParam );
}

