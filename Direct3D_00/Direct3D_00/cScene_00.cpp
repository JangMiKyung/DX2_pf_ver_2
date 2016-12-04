#include "StdAfx.h"
#include "cScene_00.h"


cScene_00::cScene_00( void )
{
}


cScene_00::~cScene_00( void )
{
}

void cScene_00::Init()
{
	//정점 정보를 채워 넣는다.
	this->vertices[0].pos = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	this->vertices[0].color = 0xff00ff00;

	this->vertices[1].pos = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
	this->vertices[1].color = 0xffff0000;

	this->vertices[2].pos = D3DXVECTOR3( -1.0f, 0.0f, 0.0f );
	this->vertices[2].color = 0xff0000ff;

	//정점의 컬러 정보를 출력하기 위해서는 Device 의 기본 라이팅 
	//기능을 해재해야 한다
	//당분간 라이팅을 사용할일이 없으니 라이팅을 끄자.

	Device->SetRenderState(
		D3DRS_LIGHTING,			//뭘셋팅할꺼니?
		FALSE					//셋팅값은 어떻게 되니?
	);


	//플랫셰이딩 ( 병신 )
	//고러드셰이딩 ( 좋지만 병신 ) ( Direct3D Default RenderSet )
	//퐁셰이딩 ( 간지의 시작 )

	/*
	//셰이드 모드 플렛셰이딩으로 셋팅 ( 정점간 픽셀보간이 안이루어진다, 첫빳다 정점정보로 도배됨 )
	Device->SetRenderState(
	D3DRS_SHADEMODE,
	D3DSHADE_FLAT );
	*/

	/*
	//셰이드 모드 고러드셰이딩으로 셋팅
	Device->SetRenderState(
	D3DRS_SHADEMODE,
	D3DSHADE_GOURAUD );
	*/

	/*
	//셰이드 모드 퐁셰이딩으로 셋팅 ( 안타깞지만 안된다 )
	Device->SetRenderState(
	D3DRS_SHADEMODE,
	D3DSHADE_PHONG );
	*/

}

void cScene_00::Release()
{

}

void cScene_00::Update( float timeDelta )
{

}

void cScene_00::Render()
{
	//정점데이터의 정보로 삼각형을 그려본다.

	//지금그리는 정점에 대한 정점 정보 FVF 플레그 값을 넣어준다. 
	//Device->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE );
	Device->SetFVF( MYVERTEX::FVF );


	Device->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,			//그리는 것은 삼각형이다.
		1,							//그려지는 개체수가 몇개니?
		this->vertices,				//CustumVertex 구조체의 배열주소
		sizeof( MYVERTEX )			//정점구조체 하나의 크기
	);


}




////D3DXVECTOR3 연산 함수
/*
//// 크기를 리턴한다
//?FLOAT D3DXVec3Length(
//CONST D3DXVECTOR3* pv
//);
//

//ex)D3DXVECTOR3 v(1.0f, 2.0f, 3.0f);
//float magnitude = D3DXVec3Length(&v); // = sqrt(14)
//
//
////벡터의 정규화
//D3DXVECTOR3* D3DXVec3Normalize(
//	D3DXVECTOR3* pOut,          //결과
//	CONST D3DXVECTOR3* pV   //정규화 하려는 벡터
//	);
//
////내적
//FLOAT D3DXVec3Dot(
//	CONST D3DXVECTOR3* pV1,  //왼쪽 피연산자
//	CONST D3DXVECTOR3* pV2   //오른쪽 피연산자
//	);
//
////외적
//D3DXVECTOR3* D3DXVec3Cross(
//	D3DXVECTOR3* pOut,           //결과
//	CONST D3DXVECTOR3* pV1, //왼쪽 피연산자
//	CONST D3DXVECTOR3* pV2  //오른쪽 피연산자
//	);
//
////항등 행렬
//D3DXMATRIX* D3DXMatrixIdentity(
//	D3DXMATRIX* pOut   //항등으로 지정될 행렬
//	)
//
//	//전치 행렬
//	D3DXMATRIX* D3DXMatrixTranspose(
//	D3DXMATRIX* pOut,                //결과로 얻어진 전치 행렬
//	CONST D3DXMATRIX* pM        //전치할 행렬
//	);
//
////역행렬
//D3DXMATRIX* D3DXMatrixInverse(
//	D3DXMATRIX* pOut,               //결과로 얻어진 역행렬
//	FLOAT* pDeterminant,           //행렬식, 필요한 경우에 이용되며 그렇지 않으면 0을 전달
//	CONST D3DXMATRIX* pM       //뒤집을 행렬
//	)
//
//	//이동 행렬
//	D3DXMATRIX* D3DXMatrixTranslation(
//	D3DXMATRIX* pOut,    //결과
//	FLOAT x,                      //x-axis 축으로 이동할 단위의 수
//	FLOAT y,                      //y-axis 축으로 이동
//	FLOAT z
//	)

*/



//Color관련
/*
//D3DXColorAdd - 2 개의 색값을 덧셈 해, 새로운 색값을 생성 한다.
//
//D3DXColorAdjustContrast - 색의 콘트라스트값을 조정한다.
//
//D3DXColorAdjustSaturation - 색의 채도값을 조정한다.
//
//D3DXColorLerp - 선형 보간을 사용해 색값을 생성 한다.
//
//D3DXColorModulate - 2 개의 색을 블렌드 한다.
//
//D3DXColorNegative - 있는 색값에 대한 부의 색값을 생성 한다.
//
//D3DXColorScale - 색값을 스케일링 한다.
//
//D3DXColorSubtract - 2 개의 색값을 뺄셈 해, 새로운 색값을 생성 한다.
*/

//행렬 관련
/*
//D3DXCreateMatrixStack - ID3DXMatrixStack 인터페이스의 인스턴스를 생성 한다.
//
//D3DXMatrixAffineTransformation - 아핀 변환 행렬을 생성 한다.
//
//D3DXMatrixDeterminant - 행렬의 행렬식을 돌려준다.
//
//D3DXMatrixIdentity - 단위행렬을 생성 한다.
//
//D3DXMatrixInverse - 행렬의 역행열을 계산한다.
//
//D3DXMatrixIsIdentity - 행렬이 단위행렬인지 아닌지를 판정한다.
//
//D3DXMatrixLookAtLH - 왼손 좌표계 뷰 행렬을 생성 한다.
//
//D3DXMatrixLookAtRH - 오른손 좌표계 뷰 행렬을 생성 한다.
//
//D3DXMatrixMultiply - 2 개의 행렬의 적을 계산한다.
//
//D3DXMatrixMultiplyTranspose - 2 개의 행렬의 전치적을 계산한다.
//
//D3DXMatrixOrthoLH - 왼손 좌표계 정사영 행렬을 생성 한다.
//
//D3DXMatrixOrthoOffCenterLH - 커스터마이즈 한 왼손 좌표계 정사영 행렬을 생성 한다.
//
//D3DXMatrixOrthoOffCenterRH - 커스터마이즈 한 오른손 좌표계 정사영 행렬을 생성 한다.
//
//D3DXMatrixOrthoRH - 오른손 좌표계 정사영 행렬을 생성 한다.
//
//D3DXMatrixPerspectiveFovLH - 시야에 근거해, 왼손 좌표계 퍼스펙티브 투영 행렬을 생성 한다.
//
//D3DXMatrixPerspectiveFovRH - 시야에 근거해, 오른손 좌표계 퍼스펙티브 투영 행렬을 생성 한다.
//
//D3DXMatrixPerspectiveLH - 왼손 좌표계 퍼스펙티브 투영 행렬을 생성 한다.
//
//D3DXMatrixPerspectiveOffCenterLH - 커스터마이즈 한 왼손 좌표계 퍼스펙티브 투영 행렬을 생성 한다.
//
//D3DXMatrixPerspectiveOffCenterRH - 커스터마이즈 한 오른손 좌표계 퍼스펙티브 투영 행렬을 생성 한다.
//
//D3DXMatrixPerspectiveRH - 오른손 좌표계 퍼스펙티브 투영 행렬을 생성 한다.
//
//D3DXMatrixReflect - 평면에 대해서 좌표계를 반전한 행렬을 생성 한다.
//
//D3DXMatrixRotationAxis - 임의의 축을 회전축으로 해 회전하는 행렬을 생성 한다.
//
//D3DXMatrixRotationQuaternion - 쿼터니온으로부터 회전 행렬을 생성 한다.
//
//D3DXMatrixRotationX - x 축을 회전축으로 해 회전하는 행렬을 생성 한다.
//
//D3DXMatrixRotationY - y 축을 회전축으로 해 회전하는 행렬을 생성 한다.
//
//D3DXMatrixRotationYawPitchRoll - 요, 피치, 및 롤을 지정해 행렬을 생성 한다.
//
//D3DXMatrixRotationZ - z 축을 회전축으로 해 회전하는 행렬을 생성 한다.
//
//D3DXMatrixScaling - x 축, y 축, z 축으로 따라 스케일링 하는 행렬을 생성 한다.
//
//D3DXMatrixShadow - 지오메트리를 평면에 투영 하는 행렬을 생성 한다.
//
//D3DXMatrixTransformation - 변환 행렬을 생성 한다.
//
//D3DXMatrixTranslation - 오프셋(offset)를 지정해 행렬을 생성 한다.
//
//D3DXMatrixTranspose - 행렬의 전치행렬을 돌려준다.
//
//
*/


//쿼터니온 관련
/*
//D3DXQuaternionBaryCentric - 중심 좌표의 쿼터니온을 돌려준다.
//
//D3DXQuaternionConjugate - 쿼터니온의 공역을 돌려준다.
//
//D3DXQuaternionDot - 2 개의 쿼터니온의 내적을 돌려준다.
//
//D3DXQuaternionExp - 지수함수를 계산한다.
//
//D3DXQuaternionIdentity - 항등 쿼터니온을 돌려준다.
//
//D3DXQuaternionInverse - 쿼터니온을 모두 부려, 재정규화한다.
//
//D3DXQuaternionIsIdentity - 쿼터니온이 항등 쿼터니온일지 어떨지를 판정한다.
//
//D3DXQuaternionLength - 쿼터니온의 길이를 돌려준다.
//
//D3DXQuaternionLengthSq - 쿼터니온의 길이의 2 승을 돌려준다.
//
//D3DXQuaternionLn - 자연대수를 계산한다.
//
//D3DXQuaternionMultiply - 2 개의 쿼터니온의 적을 계산한다.
//
//D3DXQuaternionNormalize - 정규화한 길이의 쿼터니온을 계산한다.
//
//D3DXQuaternionRotationAxis - 임의의 축을 회전축으로서 쿼터니온을 회전시킨다.
//
//D3DXQuaternionRotationMatrix - 회전 행렬로부터 쿼터니온을 생성 한다.
//
//D3DXQuaternionRotationYawPitchRoll - 요·피치·롤을 지정해 쿼터니온을 생성 한다.
//
//D3DXQuaternionSlerp - 구면선형 보간을 사용해, 2 개의 쿼터니온간을 보간 한다.
//
//D3DXQuaternionSquad - 구면2차 보간을 사용해, 쿼터니온간을 보간 한다.
//
//D3DXQuaternionSquadSetup - 구면2차 보간의 제어 포인트를 설정한다.
//
//D3DXQuaternionToAxisAngle - 쿼터니온의 회전축과 회전 각도를 계산한다.
//
//
*/



//<3D 벡터>
/*
//D3DXVec3Add - 2 개의 3D 벡터를 추가한다.
//
//D3DXVec3BaryCentric - 지정한 3D 벡터를 사용해, 중심 좌표의 점을 돌려준다.
//
//D3DXVec3CatmullRom - 지정된 3D 벡터를 사용해, Catmull - Rom 보간을 실행한다.
//
//D3DXVec3Cross - 2 개의 3D 벡터의 외적을 계산한다.
//
//D3DXVec3Dot - 2 개의 3D 벡터의 내적을 계산한다.
//
//D3DXVec3Hermite - 지정된 3D 벡터를 사용해, 에르미트의 스플라인 보간을 실행한다.
//
//D3DXVec3Length - 3D 벡터의 길이를 돌려준다.
//
//D3DXVec3LengthSq - 3D 벡터의 길이의 2 승을 돌려준다.
//
//D3DXVec3Lerp - 2 개의 3D 벡터간의 선형 보간을 실행한다.
//
//D3DXVec3Maximize - 2 개의 3D 벡터의 최대치로 구성되는 3D 벡터를 돌려준다.
//
//D3DXVec3Minimize - 2 개의 3D 벡터의 최소 값으로 구성되는 3D 벡터를 돌려준다.
//
//D3DXVec3Normalize - 3D 벡터의 정규화한 벡터를 돌려준다.
//
//D3DXVec3Project - 개체 공간으로부터 스크린 공간에 벡터를 투영 한다.
//
//D3DXVec3ProjectArray - 개체 공간으로부터 스크린 공간에 벡터를 투영 한다.
//
//D3DXVec3Scale - 3D 벡터를 스케일링 한다.
//
//D3DXVec3Subtract - 2 개의 3D 벡터를 뺄셈 한다.
//
//D3DXVec3Transform - 지정된 행렬에 의해 벡터(x, y, z, 1)를 변환 한다.
//
//D3DXVec3TransformArray - 지정된 행렬에 의해 배열(x, y, z, 1)을 변환 한다.
//
//D3DXVec3TransformCoord - 지정된 행렬에 의해 3D 벡터를 변환 해, 그 결과를 w = 1 에 투영 한다.
//
//D3DXVec3TransformCoordArray - 지정된 행렬에 의해 배열(x, y, z, 1)을 변환 해, 그 결과를 w = 1 에 투영 한다.
//
//D3DXVec3TransformNormal - 지정된 행렬에 의해 3D 벡터 법선을 변환 한다.
//
//D3DXVec3TransformNormalArray - 지정된 행렬에 의해 배열(x, y, z, 0)을 변환 한다.
//
//D3DXVec3Unproject - 스크린 공간으로부터 개체 공간에 벡터를 투영 한다.
//
//D3DXVec3UnprojectArray - 스크린 공간으로부터 개체 공간에 벡터를 투영 한다.
//
//
*/