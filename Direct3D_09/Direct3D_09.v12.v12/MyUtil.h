#pragma once

#define EPSILON 0.00001f

//a �� b�� ����
#define FLOATEQUAL(a, b) ( abs((a) - (b)) < EPSILON )

//���� 0 �̴�?
#define FLOATZERO(a) ( abs((a)) < EPSILON )

//���� ���ʹ�?
#define VECTORZERO(v)  (FLOATZERO((v).x) && FLOATZERO((v).y) && FLOATZERO((v).z))

#define RS(a, b)		Device->SetRenderState((a), (b))

namespace MyUtil 
{
	extern LPDIRECT3DDEVICE9 Device;			//���� Device ������ �ι� ( ��¥�߿��� ���� ���� ������ �ȵ�, ���� �Ŀ��� HDC �ʹ� �񱳵� �ȵ� )

	HRESULT CreateDevice();						//����̽� �����Լ�
	void ReleaseDevice();						//����̽� �����Լ�

	POINT GetMousePos();
	void SetMousePos(int x, int y);

	float Clamp(float value, float min, float max); //value ���� min max ���̷� ������
	float Clamp01(float value);				//value ���� 0~1 ���̷� ������.
	float ClampMinusOnePlusOne(float value);//value ���� -1 ~ 1 ���̷� ������.

	//float ���� ���� 
	//( t �� 0 ~ 1 ������ ���� �޴µ�...)
	//( t �� 0 �ϼ��� from ����� ���� ���ϵǰ� )
	//( t �� 1 �ϼ��� to ����� ���� ���ϵȴ� )
	float Lerp(float from, float to, float t);


	//float ���� ���� �ݴ�
	//value ���� head �� rear ���̿� ������� ��ġ�� �ִ���
	//0~1 ������ ������ �����Ѵ�.
	float InverseLerp(float head, float rear, float value);

	//���� ���� ����
	D3DXVECTOR3 VecLerp(const D3DXVECTOR3& from, const D3DXVECTOR3& to, float t);


	//Random
	void SetRandomSeed();
	float RandomFloatRange(float min, float max);
	int RandomIntRange(int min, int max);



	//������ �븻�� ������ִ� �Լ�
	void ComputeNormal(
		D3DXVECTOR3* pNorams,			//����� ���� �븻���� �迭 ������ ( �븻������ �迭�� ���� ���� �迭�� ���� ���� )
		const D3DXVECTOR3* pVertices,	//��꿡 ���� ������ġ �迭 ������
		int verticesNum,				//���� ����
		const DWORD* pIndices,			//�ε��� �迭������ ( �ϴ� �ε����� DWORD �� �뵿�ܰ� )
		int indicesNum					//�ε��� �迭������ ����
		);
}