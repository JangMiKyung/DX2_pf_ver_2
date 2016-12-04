#pragma once

class cCamera : public cTransform
{
protected:
	float fov;
	float camNear;
	float camFar;

	D3DXMATRIXA16 matView;			//뷰행렬
	D3DXMATRIXA16 matProjection;	//투영행렬
	D3DXMATRIXA16 matViewProjection;//뷰 * 투영

public:
	cCamera();
	~cCamera();

	//디폴트 컨트롤
	void DefaultControl(float timeDelta);

	//투영행렬과 View 행렬을 업데이트
	void UpdateMatrix();

	//View 행렬과 투영 행렬을 셋팅
	void UpdateCamToDevice(LPDIRECT3DDEVICE9 pDevice);


	//카메라 관련 행렬을 얻는다
	D3DXMATRIXA16 GetViewMatrix() {
		return this->matView;
	}

	D3DXMATRIXA16 GetProjectionMatrix()	{
		return this->matProjection;
	}

	D3DXMATRIXA16 GetViewProjectionMatrix()	{
		return this->matViewProjection;
	}

	//화각 셋팅
	void SetFov(float fov) { this->fov = fov; }
	float GetFov() { return this->fov; }

};

