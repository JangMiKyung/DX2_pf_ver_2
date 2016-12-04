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
	DXFONT_MGR->PrintText("우리의 목적은 적화통일", 100, 100, 0xffff0000);

	DXFONT_MGR->PrintStyleTextShadow("차차차", "우리민족끼리", 150, 130, 0xff0000ff, 0xffff0000);

	DXFONT_MGR->PrintStyleTextOutline("차차차", "위대하신 령도자 김정은동지", 200, 160, 0xff000000, 0xffff0000);
}