#pragma once

#define RESET_POSITION	1
#define RESET_ROTATION	2
#define RESET_SCALE		4

class cTransform
{
protected:
	//축정보는 Union 으로 묵여 axis 배열로도 접근 가능하고, right, up, forward 로도 접근 가능하다.
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

	//위치값
	D3DXVECTOR3 position;

	//스케일값
	D3DXVECTOR3 scale;

	//최종행렬
	D3DXMATRIXA16 matFinal;

	//transform 변환시 자동으로 업데이트가 되냐?
	bool bAutoUpdate;

public:
	cTransform();
	~cTransform();

	//오토 업데이트 여부 셋팅
	void SetAutoUpdate(bool bAuto)
	{
		this->bAutoUpdate = bAuto;
	}

	//transform 정보를 초기화
	void Reset(int resetFlag = -1);

	//위치를 월드 좌표계로 셋팅
	void SetWorldPosition(float x, float y, float z);
	void SetWorldPosition(D3DXVECTOR3 pos);

	//자신의 축 기준으로 이동 시킴
	void MovePositionSelf(float dx, float dy, float dz);
	void MovePositionSelf(D3DXVECTOR3 delta);

	//월드 기준으로 이동시킨다.
	void MovePositionWorld(float dx, float dy, float dz);
	void MovePositionWorld(D3DXVECTOR3 delta);

	//스케일 셋팅
	void SetScale(float x, float y, float z);
	void SetScale(D3DXVECTOR3 deltaScale);

	//스케일링
	void Scaling(float dx, float dy, float dz);
	void Scaling(D3DXVECTOR3 deltaScale);

	//월드 기준으로 회전
	void RotateWorld(float angleX, float angleY, float angleZ);
	void RotateWorld(D3DXVECTOR3 angle);

	//자신의 축기준으로 회전
	void RotateSelf(float angleX, float angleY, float angleZ);
	void RotateSelf(D3DXVECTOR3 angle);

	//사원수를 이용한 특정 회전값으로 회전량을 가져라
	void SetRotate(float eAngleX, float eAngleY, float eAngleZ);

	//특정 방향으로 바라보게 회전해라
	void LookDirection(D3DXVECTOR3 dir, D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0));

	//특정 방향을 바라보는데 angle 각만큼 회전 해라
	void LookDirection(D3DXVECTOR3 dir, float angle);

	//특정 위치를 바라보게 회전해라
	void LookPosition(D3DXVECTOR3 pos, D3DXVECTOR3 up = D3DXVECTOR3(0, 1, 0));

	//특정 위치를 바라보는데 angle 값만큼 회전 해라
	void LookPosition(D3DXVECTOR3 dir, float angle);

	//
	//update 들
	//

	//Transform 을 업데이트 한다
	void UpdateTransform();

	//Device 에 자신의 Transform 을 월드로 셋팅
	void SetDeviceWorld(LPDIRECT3DDEVICE9 pDevice);

	//Device 에 자신의 Transform 을 뷰로 셋팅
	void SetDeviceView(LPDIRECT3DDEVICE9 pDevice);

	//
	//Get 들
	//

	D3DXMATRIXA16 GetFinalMatrix();

	//Transform 에 대한 기즈모를 그린다
	void RenderGizmo(bool applyScale = false);
};

