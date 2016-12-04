#pragma once

class cCamera : public cTransform
{
protected:
	float fov;			//ī�޶� ȭ��
	float camNear;		//ī�޶� Near
	float camFar;		//ī�޶� Far


	D3DXMATRIXA16		matView;		//�����
	D3DXMATRIXA16		matProjection;	//�������
	D3DXMATRIXA16		matViewProjection;	//�� * �������




public:
	cCamera(void);
	~cCamera(void);




	//������İ� View ����� ������Ʈ
	void UpdateMatrix();

	//View ��İ� ���� ����� ����
	void UpdateCamToDevice(LPDIRECT3DDEVICE9 pDevice);



	//ī�޶� ���� ����� ��´�.
	D3DXMATRIXA16 GetViewMatrix()const {
		return this->matView;
	}

	D3DXMATRIXA16 GetProjectionMatrix()const {
		return this->matProjection;
	}

	D3DXMATRIXA16 GetViewProjectionMatrix()const {
		return this->matViewProjection;
	}

	//ȭ�� ����
	void SetFov(float fov){
		this->fov = fov;
	}

	float GetFov(){
		return this->fov;
	}
};

