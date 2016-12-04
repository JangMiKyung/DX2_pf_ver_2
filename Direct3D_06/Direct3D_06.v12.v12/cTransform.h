#pragma once

#define RESET_POSITION	1
#define RESET_ROTATION	2
#define RESET_SCALE		4

class cTransform
{
protected:
	//�������� Union ���� ���� axis �迭�ε� ���� �����ϰ�, right, up, forward �ε� ���� �����ϴ�.
	union
	{
		struct {
			D3DXVECTOR3 axis[3];
		};

		struct{
			D3DXVECTOR3 right;
			D3DXVECTOR3 up;
			D3DXVECTOR3 forward;
		};
	};

	//��ġ��
	D3DXVECTOR3 position;

	//�����ϰ�
	D3DXVECTOR3 scale;

	//�������
	D3DXMATRIXA16 matFinal;

	//transform ��ȯ�� �ڵ����� ������Ʈ�� �ǳ�?
	bool bAutoUpdate;

public:
	cTransform();
	~cTransform();

	//���� ������Ʈ ���� ����
	void SetAutoUpdate(bool bAuto)
	{
		this->bAutoUpdate = bAuto;
	}

	//transform ������ �ʱ�ȭ
	void Reset(int resetFlag = -1);

	//��ġ�� ���� ��ǥ��� ����
	void SetWorldPosition(float x, float y, float z);
	void SetWorldPosition(D3DXVECTOR3 pos);

	//�ڽ��� �� �������� �̵� ��Ŵ
	void MovePositionSelf(float dx, float dy, float dz);
	void MovePositionSelf(D3DXVECTOR3 delta);

	//���� �������� �̵���Ų��.
	void MovePositionWorld(float dx, float dy, float dz);
	void MovePositionWorld(D3DXVECTOR3 delta);

	//������ ����
	void SetScale(float x, float y, float z);
	void SetScale(D3DXVECTOR3 deltaScale);

	//�����ϸ�
	void Scaling(float dx, float dy, float dz);
	void Scaling(D3DXVECTOR3 deltaScale);

	//���� �������� ȸ��
	void RotateWorld(float angleX, float angleY, float angleZ);
	void RotateWorld(D3DXVECTOR3 angle);

	//�ڽ��� ��������� ȸ��
	void RotateSelf(float angleX, float angleY, float angleZ);
	void RotateSelf(D3DXVECTOR3 angle);

	//������� �̿��� Ư�� ȸ�������� ȸ������ ������
	void SetRotate(float eAngleX, float eAngleY, float eAngleZ);

	//Ư�� �������� �ٶ󺸰� ȸ���ض�
	void LookDirection(D3DXVECTOR3 dir, D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0));

	//Ư�� ������ �ٶ󺸴µ� angle ����ŭ ȸ�� �ض�
	void LookDirection(D3DXVECTOR3 dir, float angle);

	//Ư�� ��ġ�� �ٶ󺸰� ȸ���ض�
	void LookPosition(D3DXVECTOR3 pos, D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0));

	//Ư�� ��ġ�� �ٶ󺸴µ� angle ����ŭ ȸ�� �ض�
	void LookPosition(D3DXVECTOR3 dir, float angle);

	//
	//update ��
	//

	//Transform �� ������Ʈ �Ѵ�
	void UpdateTransform();

	//Device �� �ڽ��� Transform �� ����� ����
	void SetDeviceWorld(LPDIRECT3DDEVICE9 pDevice);

	//Device �� �ڽ��� Transform �� ��� ����
	void SetDeviceView(LPDIRECT3DDEVICE9 pDevice);

	//
	//Get ��
	//

	D3DXMATRIXA16 GetFinalMatrix();

	//Transform �� ���� ����� �׸���
	void RenderGizmo(bool applyScale = false);
};

