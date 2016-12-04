#pragma once

#define EPSILON 0.00001f

//a �� b�� ����
#define FLOATEQUAL(a, b) ( abs((a) - (b)) < EPSILON )

//���� 0 �̴�?
#define FLOATZERO(a) ( abs((a)) < EPSILON )

//���� ���ʹ�?
#define VECTORZERO(v)  (FLOATZERO((v).x) && FLOATZERO((v).y) && FLOATZERO((v).z))

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
	float Lerp(float from, float to, float t);

	//���� ���� ����
	D3DXVECTOR3 VecLerp(const D3DXVECTOR3& from, const D3DXVECTOR3& to, float t);
}