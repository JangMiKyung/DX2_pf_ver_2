#pragma once

class cCamera : public cTransform
{
protected:
	float fov;			//카메라 화각
	float camNear;		//카메라 Near
	float camFar;		//카메라 Far


	D3DXMATRIXA16		matView;		//뷰행렬
	D3DXMATRIXA16		matProjection;	//투영행렬
	D3DXMATRIXA16		matViewProjection;	//뷰 * 투영행렬




public:
	cCamera(void);
	~cCamera(void);




	//투영행렬과 View 행렬을 업데이트
	void UpdateMatrix();

	//View 행렬과 투영 행렬을 셋팅
	void UpdateCamToDevice(LPDIRECT3DDEVICE9 pDevice);



	//카메라 관련 행렬을 얻는다.
	D3DXMATRIXA16 GetViewMatrix()const {
		return this->matView;
	}

	D3DXMATRIXA16 GetProjectionMatrix()const {
		return this->matProjection;
	}

	D3DXMATRIXA16 GetViewProjectionMatrix()const {
		return this->matViewProjection;
	}

	//화각 셋팅
	void SetFov(float fov){
		this->fov = fov;
	}

	float GetFov(){
		return this->fov;
	}
};

