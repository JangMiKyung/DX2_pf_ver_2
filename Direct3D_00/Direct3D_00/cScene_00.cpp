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
	//���� ������ ä�� �ִ´�.
	this->vertices[0].pos = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	this->vertices[0].color = 0xff00ff00;

	this->vertices[1].pos = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
	this->vertices[1].color = 0xffff0000;

	this->vertices[2].pos = D3DXVECTOR3( -1.0f, 0.0f, 0.0f );
	this->vertices[2].color = 0xff0000ff;

	//������ �÷� ������ ����ϱ� ���ؼ��� Device �� �⺻ ������ 
	//����� �����ؾ� �Ѵ�
	//��а� �������� ��������� ������ �������� ����.

	Device->SetRenderState(
		D3DRS_LIGHTING,			//�������Ҳ���?
		FALSE					//���ð��� ��� �Ǵ�?
	);


	//�÷����̵� ( ���� )
	//������̵� ( ������ ���� ) ( Direct3D Default RenderSet )
	//�����̵� ( ������ ���� )

	/*
	//���̵� ��� �÷����̵����� ���� ( ������ �ȼ������� ���̷������, ù���� ���������� ����� )
	Device->SetRenderState(
	D3DRS_SHADEMODE,
	D3DSHADE_FLAT );
	*/

	/*
	//���̵� ��� ������̵����� ����
	Device->SetRenderState(
	D3DRS_SHADEMODE,
	D3DSHADE_GOURAUD );
	*/

	/*
	//���̵� ��� �����̵����� ���� ( ��Ÿ������ �ȵȴ� )
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
	//������������ ������ �ﰢ���� �׷�����.

	//���ݱ׸��� ������ ���� ���� ���� FVF �÷��� ���� �־��ش�. 
	//Device->SetFVF( D3DFVF_XYZ | D3DFVF_DIFFUSE );
	Device->SetFVF( MYVERTEX::FVF );


	Device->DrawPrimitiveUP(
		D3DPT_TRIANGLELIST,			//�׸��� ���� �ﰢ���̴�.
		1,							//�׷����� ��ü���� ���?
		this->vertices,				//CustumVertex ����ü�� �迭�ּ�
		sizeof( MYVERTEX )			//��������ü �ϳ��� ũ��
	);


}




////D3DXVECTOR3 ���� �Լ�
/*
//// ũ�⸦ �����Ѵ�
//?FLOAT D3DXVec3Length(
//CONST D3DXVECTOR3* pv
//);
//

//ex)D3DXVECTOR3 v(1.0f, 2.0f, 3.0f);
//float magnitude = D3DXVec3Length(&v); // = sqrt(14)
//
//
////������ ����ȭ
//D3DXVECTOR3* D3DXVec3Normalize(
//	D3DXVECTOR3* pOut,          //���
//	CONST D3DXVECTOR3* pV   //����ȭ �Ϸ��� ����
//	);
//
////����
//FLOAT D3DXVec3Dot(
//	CONST D3DXVECTOR3* pV1,  //���� �ǿ�����
//	CONST D3DXVECTOR3* pV2   //������ �ǿ�����
//	);
//
////����
//D3DXVECTOR3* D3DXVec3Cross(
//	D3DXVECTOR3* pOut,           //���
//	CONST D3DXVECTOR3* pV1, //���� �ǿ�����
//	CONST D3DXVECTOR3* pV2  //������ �ǿ�����
//	);
//
////�׵� ���
//D3DXMATRIX* D3DXMatrixIdentity(
//	D3DXMATRIX* pOut   //�׵����� ������ ���
//	)
//
//	//��ġ ���
//	D3DXMATRIX* D3DXMatrixTranspose(
//	D3DXMATRIX* pOut,                //����� ����� ��ġ ���
//	CONST D3DXMATRIX* pM        //��ġ�� ���
//	);
//
////�����
//D3DXMATRIX* D3DXMatrixInverse(
//	D3DXMATRIX* pOut,               //����� ����� �����
//	FLOAT* pDeterminant,           //��Ľ�, �ʿ��� ��쿡 �̿�Ǹ� �׷��� ������ 0�� ����
//	CONST D3DXMATRIX* pM       //������ ���
//	)
//
//	//�̵� ���
//	D3DXMATRIX* D3DXMatrixTranslation(
//	D3DXMATRIX* pOut,    //���
//	FLOAT x,                      //x-axis ������ �̵��� ������ ��
//	FLOAT y,                      //y-axis ������ �̵�
//	FLOAT z
//	)

*/



//Color����
/*
//D3DXColorAdd - 2 ���� ������ ���� ��, ���ο� ������ ���� �Ѵ�.
//
//D3DXColorAdjustContrast - ���� ��Ʈ��Ʈ���� �����Ѵ�.
//
//D3DXColorAdjustSaturation - ���� ä������ �����Ѵ�.
//
//D3DXColorLerp - ���� ������ ����� ������ ���� �Ѵ�.
//
//D3DXColorModulate - 2 ���� ���� ���� �Ѵ�.
//
//D3DXColorNegative - �ִ� ������ ���� ���� ������ ���� �Ѵ�.
//
//D3DXColorScale - ������ �����ϸ� �Ѵ�.
//
//D3DXColorSubtract - 2 ���� ������ ���� ��, ���ο� ������ ���� �Ѵ�.
*/

//��� ����
/*
//D3DXCreateMatrixStack - ID3DXMatrixStack �������̽��� �ν��Ͻ��� ���� �Ѵ�.
//
//D3DXMatrixAffineTransformation - ���� ��ȯ ����� ���� �Ѵ�.
//
//D3DXMatrixDeterminant - ����� ��Ľ��� �����ش�.
//
//D3DXMatrixIdentity - ��������� ���� �Ѵ�.
//
//D3DXMatrixInverse - ����� ���࿭�� ����Ѵ�.
//
//D3DXMatrixIsIdentity - ����� ����������� �ƴ����� �����Ѵ�.
//
//D3DXMatrixLookAtLH - �޼� ��ǥ�� �� ����� ���� �Ѵ�.
//
//D3DXMatrixLookAtRH - ������ ��ǥ�� �� ����� ���� �Ѵ�.
//
//D3DXMatrixMultiply - 2 ���� ����� ���� ����Ѵ�.
//
//D3DXMatrixMultiplyTranspose - 2 ���� ����� ��ġ���� ����Ѵ�.
//
//D3DXMatrixOrthoLH - �޼� ��ǥ�� ���翵 ����� ���� �Ѵ�.
//
//D3DXMatrixOrthoOffCenterLH - Ŀ���͸����� �� �޼� ��ǥ�� ���翵 ����� ���� �Ѵ�.
//
//D3DXMatrixOrthoOffCenterRH - Ŀ���͸����� �� ������ ��ǥ�� ���翵 ����� ���� �Ѵ�.
//
//D3DXMatrixOrthoRH - ������ ��ǥ�� ���翵 ����� ���� �Ѵ�.
//
//D3DXMatrixPerspectiveFovLH - �þ߿� �ٰ���, �޼� ��ǥ�� �۽���Ƽ�� ���� ����� ���� �Ѵ�.
//
//D3DXMatrixPerspectiveFovRH - �þ߿� �ٰ���, ������ ��ǥ�� �۽���Ƽ�� ���� ����� ���� �Ѵ�.
//
//D3DXMatrixPerspectiveLH - �޼� ��ǥ�� �۽���Ƽ�� ���� ����� ���� �Ѵ�.
//
//D3DXMatrixPerspectiveOffCenterLH - Ŀ���͸����� �� �޼� ��ǥ�� �۽���Ƽ�� ���� ����� ���� �Ѵ�.
//
//D3DXMatrixPerspectiveOffCenterRH - Ŀ���͸����� �� ������ ��ǥ�� �۽���Ƽ�� ���� ����� ���� �Ѵ�.
//
//D3DXMatrixPerspectiveRH - ������ ��ǥ�� �۽���Ƽ�� ���� ����� ���� �Ѵ�.
//
//D3DXMatrixReflect - ��鿡 ���ؼ� ��ǥ�踦 ������ ����� ���� �Ѵ�.
//
//D3DXMatrixRotationAxis - ������ ���� ȸ�������� �� ȸ���ϴ� ����� ���� �Ѵ�.
//
//D3DXMatrixRotationQuaternion - ���ʹϿ����κ��� ȸ�� ����� ���� �Ѵ�.
//
//D3DXMatrixRotationX - x ���� ȸ�������� �� ȸ���ϴ� ����� ���� �Ѵ�.
//
//D3DXMatrixRotationY - y ���� ȸ�������� �� ȸ���ϴ� ����� ���� �Ѵ�.
//
//D3DXMatrixRotationYawPitchRoll - ��, ��ġ, �� ���� ������ ����� ���� �Ѵ�.
//
//D3DXMatrixRotationZ - z ���� ȸ�������� �� ȸ���ϴ� ����� ���� �Ѵ�.
//
//D3DXMatrixScaling - x ��, y ��, z ������ ���� �����ϸ� �ϴ� ����� ���� �Ѵ�.
//
//D3DXMatrixShadow - ������Ʈ���� ��鿡 ���� �ϴ� ����� ���� �Ѵ�.
//
//D3DXMatrixTransformation - ��ȯ ����� ���� �Ѵ�.
//
//D3DXMatrixTranslation - ������(offset)�� ������ ����� ���� �Ѵ�.
//
//D3DXMatrixTranspose - ����� ��ġ����� �����ش�.
//
//
*/


//���ʹϿ� ����
/*
//D3DXQuaternionBaryCentric - �߽� ��ǥ�� ���ʹϿ��� �����ش�.
//
//D3DXQuaternionConjugate - ���ʹϿ��� ������ �����ش�.
//
//D3DXQuaternionDot - 2 ���� ���ʹϿ��� ������ �����ش�.
//
//D3DXQuaternionExp - �����Լ��� ����Ѵ�.
//
//D3DXQuaternionIdentity - �׵� ���ʹϿ��� �����ش�.
//
//D3DXQuaternionInverse - ���ʹϿ��� ��� �η�, ������ȭ�Ѵ�.
//
//D3DXQuaternionIsIdentity - ���ʹϿ��� �׵� ���ʹϿ����� ����� �����Ѵ�.
//
//D3DXQuaternionLength - ���ʹϿ��� ���̸� �����ش�.
//
//D3DXQuaternionLengthSq - ���ʹϿ��� ������ 2 ���� �����ش�.
//
//D3DXQuaternionLn - �ڿ������ ����Ѵ�.
//
//D3DXQuaternionMultiply - 2 ���� ���ʹϿ��� ���� ����Ѵ�.
//
//D3DXQuaternionNormalize - ����ȭ�� ������ ���ʹϿ��� ����Ѵ�.
//
//D3DXQuaternionRotationAxis - ������ ���� ȸ�������μ� ���ʹϿ��� ȸ����Ų��.
//
//D3DXQuaternionRotationMatrix - ȸ�� ��ķκ��� ���ʹϿ��� ���� �Ѵ�.
//
//D3DXQuaternionRotationYawPitchRoll - �䡤��ġ������ ������ ���ʹϿ��� ���� �Ѵ�.
//
//D3DXQuaternionSlerp - ���鼱�� ������ �����, 2 ���� ���ʹϿ°��� ���� �Ѵ�.
//
//D3DXQuaternionSquad - ����2�� ������ �����, ���ʹϿ°��� ���� �Ѵ�.
//
//D3DXQuaternionSquadSetup - ����2�� ������ ���� ����Ʈ�� �����Ѵ�.
//
//D3DXQuaternionToAxisAngle - ���ʹϿ��� ȸ����� ȸ�� ������ ����Ѵ�.
//
//
*/



//<3D ����>
/*
//D3DXVec3Add - 2 ���� 3D ���͸� �߰��Ѵ�.
//
//D3DXVec3BaryCentric - ������ 3D ���͸� �����, �߽� ��ǥ�� ���� �����ش�.
//
//D3DXVec3CatmullRom - ������ 3D ���͸� �����, Catmull - Rom ������ �����Ѵ�.
//
//D3DXVec3Cross - 2 ���� 3D ������ ������ ����Ѵ�.
//
//D3DXVec3Dot - 2 ���� 3D ������ ������ ����Ѵ�.
//
//D3DXVec3Hermite - ������ 3D ���͸� �����, ������Ʈ�� ���ö��� ������ �����Ѵ�.
//
//D3DXVec3Length - 3D ������ ���̸� �����ش�.
//
//D3DXVec3LengthSq - 3D ������ ������ 2 ���� �����ش�.
//
//D3DXVec3Lerp - 2 ���� 3D ���Ͱ��� ���� ������ �����Ѵ�.
//
//D3DXVec3Maximize - 2 ���� 3D ������ �ִ�ġ�� �����Ǵ� 3D ���͸� �����ش�.
//
//D3DXVec3Minimize - 2 ���� 3D ������ �ּ� ������ �����Ǵ� 3D ���͸� �����ش�.
//
//D3DXVec3Normalize - 3D ������ ����ȭ�� ���͸� �����ش�.
//
//D3DXVec3Project - ��ü �������κ��� ��ũ�� ������ ���͸� ���� �Ѵ�.
//
//D3DXVec3ProjectArray - ��ü �������κ��� ��ũ�� ������ ���͸� ���� �Ѵ�.
//
//D3DXVec3Scale - 3D ���͸� �����ϸ� �Ѵ�.
//
//D3DXVec3Subtract - 2 ���� 3D ���͸� ���� �Ѵ�.
//
//D3DXVec3Transform - ������ ��Ŀ� ���� ����(x, y, z, 1)�� ��ȯ �Ѵ�.
//
//D3DXVec3TransformArray - ������ ��Ŀ� ���� �迭(x, y, z, 1)�� ��ȯ �Ѵ�.
//
//D3DXVec3TransformCoord - ������ ��Ŀ� ���� 3D ���͸� ��ȯ ��, �� ����� w = 1 �� ���� �Ѵ�.
//
//D3DXVec3TransformCoordArray - ������ ��Ŀ� ���� �迭(x, y, z, 1)�� ��ȯ ��, �� ����� w = 1 �� ���� �Ѵ�.
//
//D3DXVec3TransformNormal - ������ ��Ŀ� ���� 3D ���� ������ ��ȯ �Ѵ�.
//
//D3DXVec3TransformNormalArray - ������ ��Ŀ� ���� �迭(x, y, z, 0)�� ��ȯ �Ѵ�.
//
//D3DXVec3Unproject - ��ũ�� �������κ��� ��ü ������ ���͸� ���� �Ѵ�.
//
//D3DXVec3UnprojectArray - ��ũ�� �������κ��� ��ü ������ ���͸� ���� �Ѵ�.
//
//
*/