#pragma once

class cCamera : public cTransform
{
protected:
	float fov;
	float camNear;
	float camFar;

	D3DXMATRIXA16 matView;			//�����
	D3DXMATRIXA16 matProjection;	//�������
	D3DXMATRIXA16 matViewProjection;//�� * ����

public:
	cCamera();
	~cCamera();

	//����Ʈ ��Ʈ��
	void DefaultControl(float timeDelta);

	//������İ� View ����� ������Ʈ
	void UpdateMatrix();

	//View ��İ� ���� ����� ����
	void UpdateCamToDevice(LPDIRECT3DDEVICE9 pDevice);


	//ī�޶� ���� ����� ��´�
	D3DXMATRIXA16 GetViewMatrix() {
		return this->matView;
	}

	D3DXMATRIXA16 GetProjectionMatrix()	{
		return this->matProjection;
	}

	D3DXMATRIXA16 GetViewProjectionMatrix()	{
		return this->matViewProjection;
	}

	//ȭ�� ����
	void SetFov(float fov) { this->fov = fov; }
	float GetFov() { return this->fov; }

};

