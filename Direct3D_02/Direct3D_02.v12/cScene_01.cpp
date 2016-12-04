#include "stdafx.h"
#include "cScene_01.h"


cScene_01::cScene_01()
{
}


cScene_01::~cScene_01()
{
}

void cScene_01::Init()
{
	//��Ʈ ��ü �����
	D3DXCreateFont(
		Device,				//����̽�
		30.f,				//��Ʈ�� ����
		0.f,				//��Ʈ�� ��(�� ���� 0�̸� �տ��� ������ ��Ʈ ���̿� ���)
		FW_BOLD,			//��Ʈ�� �β� ����(FW_BOLD ��Ʈ�� BOLD ��Ÿ�� ũ�⸦ ���´�)
		1,					//�Ӹ�(�׳� 1)
		FALSE,				//���ڸ� ����
		DEFAULT_CHARSET,	//ĳ���� set( �׳� DEFAULT_CHARSET )
		OUT_DEFAULT_PRECIS,	//�ܰ� ó����� ( �׳� OUT_DEFAULT_PRECIS )
		DEFAULT_QUALITY,	//����Ƽ ( �׳� DEFAULT_QUALITY )
		DEFAULT_PITCH | FF_DONTCARE,// �۾�ü �йи��� ����
		"�ü�ü",			//���� ��Ʈ �̸� 
		&pDxFont			//������ ��Ʈ��ü ���� ���� ������
		);

}

void cScene_01::Release()
{
	SAFE_RELEASE(pDxFont);
}

void cScene_01::Update(float timeDelta)
{

}

void cScene_01::Render()
{
	//��� ���� (������ ��ǥ��)
	RECT rcText2 = { 101, 101, 120, 120 };

	pDxFont->DrawText(
		NULL,				//��������Ʈ�� �׸��� ��� ��������Ʈ ��ü
		"����������",			//�� ���ڿ�
		-1,					//��� ���ڿ� ���� -1�̸� �� ����
		&rcText2,			//��� ����
		DT_LEFT | DT_NOCLIP,//��������, DT_NOCLIP�ϸ� ������ �����ϰ� �� ���
		0xff000000			//���ڿ� ����
		);

	RECT rcText1 = { WINSIZE_X / 2, 0, WINSIZE_X, 50 };

	pDxFont->DrawText(
		NULL,
		"��Ⱑ �ʹ� ����",
		-1,
		&rcText1,
		DT_RIGHT | DT_NOCLIP,
		0xffffff00
		);

	RECT rcText3 = { 50, 0, WINSIZE_X - 50, WINSIZE_Y };

	pDxFont->DrawText(
		NULL,
		"���� �ð��� ����",
		-1,
		&rcText3,
		DT_CENTER | DT_NOCLIP | DT_VCENTER,
		0xffffffff
		);


}