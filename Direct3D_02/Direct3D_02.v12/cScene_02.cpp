#include "stdafx.h"
#include "cScene_02.h"


cScene_02::cScene_02()
{
}


cScene_02::~cScene_02()
{
}

void cScene_02::Init()
{

}

void cScene_02::Release()
{

}

void cScene_02::Update(float timeDelta)
{

}

void cScene_02::Render()
{
	DXFONT_MGR->PrintText("�츮�� ������ ��ȭ����", 100, 100, 0xffff0000);

	DXFONT_MGR->PrintStyleTextShadow("������", "�츮��������", 150, 130, 0xff0000ff, 0xffff0000);

	DXFONT_MGR->PrintStyleTextOutline("������", "�����Ͻ� �ɵ��� ����������", 200, 160, 0xff000000, 0xffff0000);
}