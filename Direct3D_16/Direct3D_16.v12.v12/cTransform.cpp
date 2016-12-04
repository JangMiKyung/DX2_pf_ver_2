#include "StdAfx.h"
#include "cTransform.h"


cTransform::cTransform(void)
{
	this->pParent = NULL;
	this->pFirstChild = NULL;
	this->pNextSibling = NULL;

	this->bAutoUpdate = true;

	//정보 리셋
	this->Reset();

	if (this->bAutoUpdate)
		this->UpdateTransform();



	ShakePosPower = 0.0f;
	ShakeRotPower = 0.0f;

	ShakePosRelease = 0.0f;
	ShakeRotRelease = 0.0f;
	ShakePosFlag = SHAKE_X | SHAKE_Y | SHAKE_Z;
	ShakeRotFlag = SHAKE_X | SHAKE_Y | SHAKE_Z;
}


cTransform::~cTransform(void)
{
}


//Transform 정보를 초기화
void cTransform::Reset(int resetFlag)
{

	if (resetFlag & RESET_POSITION)
	{
		//위치는 0, 0, 0
		this->position.x = 0;
		this->position.y = 0;
		this->position.z = 0;
	}

	if (resetFlag & RESET_ROTATION)
	{
		//회전방향은 
		//this->axis[0] = D3DXVECTOR3( 1, 0, 0 );
		//this->axis[1] = D3DXVECTOR3( 0, 1, 0 );
		//this->axis[2] = D3DXVECTOR3( 0, 0, 1 );
		this->right = D3DXVECTOR3(1, 0, 0);
		this->up = D3DXVECTOR3(0, 1, 0);
		this->forward = D3DXVECTOR3(0, 0, 1);
	}

	if (resetFlag & RESET_SCALE)
	{
		//스케일의 초기화 값
		this->scale = D3DXVECTOR3(1, 1, 1);
	}

	this->UpdateTransform();
}


//특정 Child 를 내자식으로 붙인다.
void cTransform::AddChild(cTransform* pNewChild)
{
	//이미 내새끼면 할필요 없다
	if (pNewChild->pParent == this)
		return;

	//너이새끼 내밑으로 들어올려면 부모랑 연을 끊어라...
	pNewChild->ReleaseParent();

	//부모의 상대적인 좌표값으로 갱신하기위해 
	//부모의 final 역행렬을 구한다.
	D3DXMATRIXA16 matInvFinal;
	D3DXMatrixInverse(&matInvFinal, NULL, &this->matFinal);

	//자식의 Position 과 Axis 및 Scale 갱신
	D3DXVec3TransformCoord(&pNewChild->position, &pNewChild->position, &matInvFinal);

	//축3개 변환하고 
	for (int i = 0; i < 3; i++){
		D3DXVec3TransformNormal(pNewChild->axis + i, pNewChild->axis + i, &matInvFinal);
	}

	//3축에 대한 길이값을 얻는다.
	pNewChild->scale.x = D3DXVec3Length(&pNewChild->right);
	pNewChild->scale.y = D3DXVec3Length(&pNewChild->up);
	pNewChild->scale.z = D3DXVec3Length(&pNewChild->forward);

	//정규화
	D3DXVec3Normalize(&pNewChild->right, &pNewChild->right);
	D3DXVec3Normalize(&pNewChild->up, &pNewChild->up);
	D3DXVec3Normalize(&pNewChild->forward, &pNewChild->forward);


	//새로운 놈의 부모는 내가 된다.
	pNewChild->pParent = this;

	//나의 자식놈 포인터
	cTransform* pChild = this->pFirstChild;

	//자식이 없는 쓸쓸한 독거노인이라면...
	if (pChild == NULL){
		//안심하고 추가
		this->pFirstChild = pNewChild;
		pNewChild->pParent = this;
	}

	//대가족에 들어간다.
	else{

		while (pChild != NULL){

			//내가 들어갈 자리를 찾았다면...
			if (pChild->pNextSibling == NULL)
			{
				pChild->pNextSibling = pNewChild;
				pNewChild->pParent = this;
				break;
			}

			pChild = pChild->pNextSibling;
		}
	}





	//업데이트
	this->UpdateTransform();
}

//특정 Transform 에 붙는다.
void cTransform::AttachTo(cTransform* pNewParent)
{
	pNewParent->AddChild(this);
}

//부모와 안녕
void cTransform::ReleaseParent()
{
	//부모가 없니?
	if (this->pParent == NULL)
		return;

	//부모랑 연을 끊기 전에 부모부터 자식연을 끊어라...
	cTransform* pChild = this->pParent->pFirstChild;

	//내가 부모의 첫째자식이니?
	if (pChild == this){

		//내다음 자식이 첫번째 자식이 된다.
		this->pParent->pFirstChild = this->pNextSibling;

		//형재들과의 연도 끊는다.
		this->pNextSibling = NULL;
	}

	else
	{
		while (pChild != NULL){

			//현재 자식의 다음이 나니?
			if (pChild->pNextSibling == this){

				pChild->pNextSibling = this->pNextSibling;

				//형재들과의 연도 끊는다.
				this->pNextSibling = NULL;

				break;
			}

			//다음 자식본다.
			pChild = pChild->pNextSibling;
		}
	}

	//부모랑 연을 끊어라...
	this->pParent = NULL;

	//자신의 현재 월드 위치에 대한 갱신이 필요하다.
	//진짜월드 위치는 matFinal 이 다 가지고 있다.

	//월드 위치 갱신
	this->position.x = this->matFinal._41;
	this->position.y = this->matFinal._42;
	this->position.z = this->matFinal._43;

	//3축 얻어온다.
	D3DXVECTOR3 forwardScaled(this->matFinal._31, this->matFinal._32, this->matFinal._33);
	D3DXVECTOR3 upScaled(this->matFinal._21, this->matFinal._22, this->matFinal._23);
	D3DXVECTOR3 rightScaled(this->matFinal._11, this->matFinal._12, this->matFinal._13);

	//3축에서 스케일 뺀다
	float scaleX = D3DXVec3Length(&rightScaled);
	float scaleY = D3DXVec3Length(&upScaled);
	float scaleZ = D3DXVec3Length(&forwardScaled);

	//정규화
	D3DXVECTOR3 forwardUnit;
	D3DXVECTOR3 upUnit;
	D3DXVECTOR3 rightUnit;
	D3DXVec3Normalize(&rightUnit, &rightScaled);
	D3DXVec3Normalize(&upUnit, &upScaled);
	D3DXVec3Normalize(&forwardUnit, &forwardScaled);

	//정규화된 3축 대입
	this->forward = forwardUnit;
	this->right = rightUnit;
	this->up = upUnit;


	//스케일 대입
	this->scale.x = scaleX;
	this->scale.y = scaleY;
	this->scale.z = scaleZ;

	//나만의 새상이 되었다...

	//부모랑 사라진 기념으로 Update 한번 콜
	this->UpdateTransform();
}


//위치를 월드 좌표계로 셋팅한다. 
void cTransform::SetWorldPosition(float x, float y, float z)
{
	D3DXVECTOR3 pos(x, y, z);

	//부모가 있다면 부모의 상태적인 위치로 바꿔라...
	if (this->pParent != NULL)
	{
		//부모의 최종 행렬의 역행렬
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &this->pParent->matFinal);

		//그 역행에 Pos 적용
		D3DXVec3TransformCoord(&pos, &pos, &matInvParentFinal);
	}


	this->position.x = pos.x;
	this->position.y = pos.y;
	this->position.z = pos.z;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}
void cTransform::SetWorldPosition(D3DXVECTOR3 pos)
{
	//부모가 있다면 부모의 상태적인 위치로 바꿔라...
	if (this->pParent != NULL)
	{
		//부모의 최종 행렬의 역행렬
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &this->pParent->matFinal);

		//그 역행에 Pos 적용
		D3DXVec3TransformCoord(&pos, &pos, &matInvParentFinal);
	}


	this->position.x = pos.x;
	this->position.y = pos.y;
	this->position.z = pos.z;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

//위치를 로컬 좌표계로 셋팅한다.  ( 부모가 있는 경우 무모의 상태적인 위치 )
void cTransform::SetLocalPosition(float x, float y, float z)
{
	this->position.x = x;
	this->position.y = y;
	this->position.z = z;

	if (this->bAutoUpdate)
		this->UpdateTransform();

}
void cTransform::SetLocalPosition(D3DXVECTOR3 pos)
{
	this->position.x = pos.x;
	this->position.y = pos.y;
	this->position.z = pos.z;

	if (this->bAutoUpdate)
		this->UpdateTransform();

}


//자신의 축기준으로 이동 시킨다.
void cTransform::MovePositionSelf(float dx, float dy, float dz)
{
	//이동 벡터
	D3DXVECTOR3 move(0, 0, 0);

	//자신의 이동 축 얻는다.
	D3DXVECTOR3 moveAxis[3];
	this->GetUnitAxies(moveAxis);
	move += moveAxis[0] * dx;
	move += moveAxis[1] * dy;
	move += moveAxis[2] * dz;

	//월드 이동
	D3DXVECTOR3 nowWorldPos = this->GetWorldPosition();

	//아래의 함수에서 TransformUpdate 가 일어남
	this->SetWorldPosition(nowWorldPos + move);
}
void cTransform::MovePositionSelf(D3DXVECTOR3 delta)
{
	//이동 벡터
	D3DXVECTOR3 move(0, 0, 0);

	//자신의 이동 축 얻는다.
	D3DXVECTOR3 moveAxis[3];
	this->GetUnitAxies(moveAxis);
	move += moveAxis[0] * delta.x;
	move += moveAxis[1] * delta.y;
	move += moveAxis[2] * delta.z;

	//월드 이동
	D3DXVECTOR3 nowWorldPos = this->GetWorldPosition();

	//아래의 함수에서 TransformUpdate 가 일어남
	this->SetWorldPosition(nowWorldPos + move);
}

//월드 기준으로 이동 시킨다.
void cTransform::MovePositionWorld(float dx, float dy, float dz)
{
	//이동 벡터
	D3DXVECTOR3 move(dx, dy, dz);

	//월드 이동
	D3DXVECTOR3 nowWorldPos = this->GetWorldPosition();

	//아래의 함수에서 TransformUpdate 가 일어남
	this->SetWorldPosition(nowWorldPos + move);

}
void cTransform::MovePositionWorld(D3DXVECTOR3 delta)
{
	//월드 이동
	D3DXVECTOR3 nowWorldPos = this->GetWorldPosition();

	//아래의 함수에서 TransformUpdate 가 일어남
	this->SetWorldPosition(nowWorldPos + delta);
}


//부모가 있는 경우 로컬 기준으로 이동 시킨다.
void cTransform::MovePositionLocal(float dx, float dy, float dz)
{
	this->position.x += dx;
	this->position.y += dy;
	this->position.z += dz;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

void cTransform::MovePositionLocal(D3DXVECTOR3 delta)
{
	this->position += delta;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

//스케일 셋팅 
void cTransform::SetScale(float x, float y, float z)
{
	this->scale.x = x;
	this->scale.y = y;
	this->scale.z = z;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}
void cTransform::SetScale(D3DXVECTOR3 scale)
{
	this->scale = scale;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

//스케일링
void cTransform::Scaling(float dx, float dy, float dz)
{
	this->scale.x += dx;
	this->scale.y += dy;
	this->scale.z += dz;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}
void cTransform::Scaling(D3DXVECTOR3 deltaScale)
{
	this->scale += deltaScale;

	if (this->bAutoUpdate)
		this->UpdateTransform();
}



//월드 기준으로 회전 
void cTransform::RotateWorld(float angleX, float angleY, float angleZ)
{
	//부모가 있는 경우
	if (this->pParent)
	{
		//진짜로월드 축
		D3DXVECTOR3 worldAxis[3];
		this->GetUnitAxies(worldAxis);

		//각 축에 대한 회전 행렬
		D3DXMATRIXA16 matRotateX;
		D3DXMatrixRotationX(&matRotateX, angleX);

		D3DXMATRIXA16 matRotateY;
		D3DXMatrixRotationY(&matRotateY, angleY);

		D3DXMATRIXA16 matRotateZ;
		D3DXMatrixRotationZ(&matRotateZ, angleZ);


		/*
		//다음과 같이 하면 회전 행렬을 2번 돌리니깐.....

		//최종 회전 행렬
		D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;

		//최종 회전 행렬 대로 회전 시킨다.
		for( int i = 0 ; i < 3 ; i++ )
		D3DXVec3TransformNormal( worldAxis + i, worldAxis + i, &matRotate );

		//부모의 역행렬로 다시 회전
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse( &matInvParentFinal, NULL, &this->pParent->matFinal );


		//최종 회전 행렬 대로 회전 시킨다.
		for( int i = 0 ; i < 3 ; i++ )
		D3DXVec3TransformNormal( worldAxis + i, worldAxis + i, &matInvParentFinal );
		*/

		//최종 회전 행렬만들때 미리 역행렬성분을 곱해놓으면 1 번만 회전시키면 된다.

		//최종 회전 행렬
		D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;

		//부모의 역행렬로 다시 회전
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &this->pParent->matFinal);

		matRotate = matRotate * matInvParentFinal;

		//최종 회전 행렬 대로 회전 시킨다.
		for (int i = 0; i < 3; i++)
			D3DXVec3TransformNormal(this->axis + i, worldAxis + i, &matRotate);


		if (this->bAutoUpdate)
			this->UpdateTransform();
	}

	//부모가 없는 경우
	else
	{
		RotateLocal(angleX, angleY, angleZ);
	}


}

void cTransform::RotateWorld(D3DXVECTOR3 angle)
{
	//부모가 있는 경우
	if (this->pParent)
	{
		//진짜로월드 축
		D3DXVECTOR3 worldAxis[3];
		this->GetUnitAxies(worldAxis);

		//각 축에 대한 회전 행렬
		D3DXMATRIXA16 matRotateX;
		D3DXMatrixRotationX(&matRotateX, angle.x);

		D3DXMATRIXA16 matRotateY;
		D3DXMatrixRotationY(&matRotateY, angle.y);

		D3DXMATRIXA16 matRotateZ;
		D3DXMatrixRotationZ(&matRotateZ, angle.z);


		//최종 회전 행렬만들때 미리 역행렬성분을 곱해놓으면 1 번만 회전시키면 된다.

		//최종 회전 행렬
		D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;

		//부모의 역행렬로 다시 회전
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &this->pParent->matFinal);

		matRotate = matRotate * matInvParentFinal;

		//최종 회전 행렬 대로 회전 시킨다.
		for (int i = 0; i < 3; i++)
			D3DXVec3TransformNormal(this->axis + i, worldAxis + i, &matRotate);


		if (this->bAutoUpdate)
			this->UpdateTransform();
	}

	//부모가 없는 경우
	else
	{
		RotateLocal(angle.x, angle.y, angle.z);
	}
}


//자신의 축기준으로 회전
void cTransform::RotateSelf(float angleX, float angleY, float angleZ)
{
	//각 축에 대한 회전 행렬
	D3DXMATRIXA16 matRotateX;
	D3DXMatrixRotationAxis(&matRotateX, &this->GetRight(), angleX);

	D3DXMATRIXA16 matRotateY;
	D3DXMatrixRotationAxis(&matRotateY, &this->GetUp(), angleY);

	D3DXMATRIXA16 matRotateZ;
	D3DXMatrixRotationAxis(&matRotateZ, &this->GetForward(), angleZ);


	//최종 회전 행렬
	D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;


	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);


	if (this->bAutoUpdate)
		this->UpdateTransform();
}

void cTransform::RotateSelf(D3DXVECTOR3 angle)
{

	//각 축에 대한 회전 행렬
	D3DXMATRIXA16 matRotateX;
	D3DXMatrixRotationAxis(&matRotateX, &this->GetRight(), angle.x);

	D3DXMATRIXA16 matRotateY;
	D3DXMatrixRotationAxis(&matRotateY, &this->GetUp(), angle.y);

	D3DXMATRIXA16 matRotateZ;
	D3DXMatrixRotationAxis(&matRotateZ, &this->GetForward(), angle.z);


	//최종 회전 행렬
	D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;


	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);


	if (this->bAutoUpdate)
		this->UpdateTransform();
}

//부모가 있는 경우 부모 로컬의 축기준으로 회전
void cTransform::RotateLocal(float angleX, float angleY, float angleZ)
{

	//각 축에 대한 회전 행렬
	D3DXMATRIXA16 matRotateX;
	D3DXMatrixRotationX(&matRotateX, angleX);

	D3DXMATRIXA16 matRotateY;
	D3DXMatrixRotationY(&matRotateY, angleY);

	D3DXMATRIXA16 matRotateZ;
	D3DXMatrixRotationZ(&matRotateZ, angleZ);

	//최종 회전 행렬
	D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;


	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);


	if (this->bAutoUpdate)
		this->UpdateTransform();
}

void cTransform::RotateLocal(D3DXVECTOR3 angle)
{
	//각 축에 대한 회전 행렬
	D3DXMATRIXA16 matRotateX;
	D3DXMatrixRotationX(&matRotateX, angle.x);

	D3DXMATRIXA16 matRotateY;
	D3DXMatrixRotationY(&matRotateY, angle.y);

	D3DXMATRIXA16 matRotateZ;
	D3DXMatrixRotationZ(&matRotateZ, angle.z);

	//최종 회전 행렬
	D3DXMATRIXA16 matRotate = matRotateY * matRotateX * matRotateZ;

	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);

	if (this->bAutoUpdate)
		this->UpdateTransform();
}


//특정 방향을 바라보게 회전해라.
void cTransform::LookDirection(const D3DXVECTOR3& dir, const D3DXVECTOR3& up)
{


	//정면 벡터
	D3DXVECTOR3 newForward = dir;

	//오른쪽벡터 ( 매개변수로 들어온 Up 을 가지고 외적 )
	D3DXVECTOR3 newRight;
	D3DXVec3Cross(&newRight, &up, &newForward);
	D3DXVec3Normalize(&newRight, &newRight);

	//업 
	D3DXVECTOR3 newUp;
	D3DXVec3Cross(&newUp, &newForward, &newRight);
	D3DXVec3Normalize(&newUp, &newUp);

	//만약 부모가 있다면...
	if (this->pParent)
	{
		//새로운 축 성분에 부모 역행렬 곱해....
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &this->pParent->matFinal);


		D3DXVec3TransformNormal(&this->forward, &newForward, &matInvParentFinal);
		D3DXVec3TransformNormal(&this->right, &newRight, &matInvParentFinal);
		D3DXVec3TransformNormal(&this->up, &newUp, &matInvParentFinal);

	}

	else
	{
		this->forward = newForward;
		this->right = newRight;
		this->up = newUp;
	}

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

//특정 방향을 바라보는데 angle 각만큼만 회전 해라
void cTransform::LookDirection(const D3DXVECTOR3& dir, float angle)
{
	//진짜로월드 축
	D3DXVECTOR3 worldAxis[3];
	this->GetUnitAxies(worldAxis);

	//정면 벡터와 바라볼 방향의 각도차를 얻자...
	float distRadian = acos(
		ClampMinusOnePlusOne(D3DXVec3Dot(worldAxis + AXIS_Z, &dir)));

	//각도차가 거의 없다면 하지마...
	if (FLOATZERO(distRadian)) return;

	//외적 ( 내정면과 타겟까지의 방향을 외적 )
	D3DXVECTOR3 cross;
	D3DXVec3Cross(&cross, worldAxis + AXIS_Z, &dir);
	D3DXVec3Normalize(&cross, &cross);

	//외적축으로 각차만큼 회전 시키는 행렬
	D3DXMATRIXA16 matRotate;
	D3DXMatrixRotationAxis(&matRotate, &cross, min(angle, distRadian));

	//만약 부모가 있다면...
	if (this->pParent)
	{
		//회전 성분에 부모 역행렬 곱해....
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &this->pParent->matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	//적용
	for (int i = 0; i < 3; i++)
		D3DXVec3TransformNormal(this->axis + i, worldAxis + i, &matRotate);

	if (this->bAutoUpdate)
		this->UpdateTransform();

}

//특정위치를 바라보게 회전해라.
void cTransform::LookPosition(const D3DXVECTOR3& pos, const D3DXVECTOR3& up)
{
	//위치에 대한 방향벡터를 얻는다.
	D3DXVECTOR3 worldPos = this->GetWorldPosition();
	D3DXVECTOR3 dir = pos - worldPos;

	D3DXVec3Normalize(&dir, &dir);
	this->LookDirection(dir, up);



}

//특정위치를  바라보는데 angle 각만큼만 회전 해라
void cTransform::LookPosition(const D3DXVECTOR3& pos, float angle)
{
	//위치에 대한 방향벡터를 얻는다.
	D3DXVECTOR3 worldPos = this->GetWorldPosition();
	D3DXVECTOR3 dir = pos - worldPos;

	D3DXVec3Normalize(&dir, &dir);
	this->LookDirection(dir, angle);

}


//사원수를 이용한 특정 회전값으로 회전량을 가져라....
void cTransform::SetRotateWorld(float eAngleX, float eAngleY, float aAngleZ)
{
	//사원수 준비
	D3DXQUATERNION quatRot;
	D3DXQuaternionRotationYawPitchRoll(&quatRot, eAngleY, eAngleX, aAngleZ);

	//사원수에 의한 행렬준비
	D3DXMATRIXA16 matRotate;
	D3DXMatrixRotationQuaternion(&matRotate, &quatRot);		//사원수에 의한 회전값으로 회전행렬이 만들어진다.

	//만약 부모가 있다면...
	if (this->pParent)
	{
		//회전 성분에 부모 역행렬 곱해....
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &this->pParent->matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	//축리셋
	this->right = D3DXVECTOR3(1, 0, 0);
	this->up = D3DXVECTOR3(0, 1, 0);
	this->forward = D3DXVECTOR3(0, 0, 1);

	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

void cTransform::SetRotateLocal(float eAngleX, float eAngleY, float aAngleZ)
{
	//사원수 준비
	D3DXQUATERNION quatRot;
	D3DXQuaternionRotationYawPitchRoll(&quatRot, eAngleY, eAngleX, aAngleZ);

	//사원수에 의한 행렬준비
	D3DXMATRIXA16 matRotate;
	D3DXMatrixRotationQuaternion(&matRotate, &quatRot);		//사원수에 의한 회전값으로 회전행렬이 만들어진다.

	//축리셋
	this->right = D3DXVECTOR3(1, 0, 0);
	this->up = D3DXVECTOR3(0, 1, 0);
	this->forward = D3DXVECTOR3(0, 0, 1);

	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

// 회전 행렬을 넣어주면 그 회전 행렬대로 회전한다.
void cTransform::SetRotateWorld(const D3DXMATRIXA16& matWorldRotate)
{
	D3DXMATRIXA16 matRotate = matWorldRotate;

	//만약 부모가 있다면...
	if (this->pParent)
	{
		//회전 성분에 부모 역행렬 곱해....
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &this->pParent->matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	//축리셋
	this->right = D3DXVECTOR3(1, 0, 0);
	this->up = D3DXVECTOR3(0, 1, 0);
	this->forward = D3DXVECTOR3(0, 0, 1);

	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

void cTransform::SetRotateLocal(const D3DXMATRIXA16& matWorldRotate)
{
	D3DXMATRIXA16 matRotate = matWorldRotate;


	//축리셋
	this->right = D3DXVECTOR3(1, 0, 0);
	this->up = D3DXVECTOR3(0, 1, 0);
	this->forward = D3DXVECTOR3(0, 0, 1);

	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

// 회전 사원수를 넣어주면 그 회전값 대로 회전한다.
void cTransform::SetRotateWorld(const D3DXQUATERNION& matWorldRotate)
{
	//사원수 준비
	D3DXQUATERNION quatRot = matWorldRotate;

	//사원수에 의한 행렬준비
	D3DXMATRIXA16 matRotate;
	D3DXMatrixRotationQuaternion(&matRotate, &quatRot);		//사원수에 의한 회전값으로 회전행렬이 만들어진다.

	//만약 부모가 있다면...
	if (this->pParent)
	{
		//회전 성분에 부모 역행렬 곱해....
		D3DXMATRIXA16 matInvParentFinal;
		D3DXMatrixInverse(&matInvParentFinal, NULL, &this->pParent->matFinal);
		matRotate = matRotate * matInvParentFinal;
	}

	//축리셋
	this->right = D3DXVECTOR3(1, 0, 0);
	this->up = D3DXVECTOR3(0, 1, 0);
	this->forward = D3DXVECTOR3(0, 0, 1);

	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);

	if (this->bAutoUpdate)
		this->UpdateTransform();
}

void cTransform::SetRotateLocal(const D3DXQUATERNION& matWorldRotate)
{
	//사원수 준비
	D3DXQUATERNION quatRot = matWorldRotate;

	//사원수에 의한 행렬준비
	D3DXMATRIXA16 matRotate;
	D3DXMatrixRotationQuaternion(&matRotate, &quatRot);		//사원수에 의한 회전값으로 회전행렬이 만들어진다.

	//축리셋
	this->right = D3DXVECTOR3(1, 0, 0);
	this->up = D3DXVECTOR3(0, 1, 0);
	this->forward = D3DXVECTOR3(0, 0, 1);

	//최종 회전 행렬 대로 회전 시킨다.
	for (int i = 0; i < 3; i++)
		D3DXVec3TransformNormal(&this->axis[i], &this->axis[i], &matRotate);

	if (this->bAutoUpdate)
		this->UpdateTransform();
}



// 자신의 회전 값을 from 과 to 사이의 회전량만큼 회전보간(구면보간) 하여 적용
void cTransform::RotateSlerp(const cTransform& from, const cTransform& to, float t)
{
	t = Clamp01(t);


	D3DXQUATERNION fromQuat = from.GetWorldRotateQuaternion();
	D3DXQUATERNION toQuat = to.GetWorldRotateQuaternion();

	//t 가 0 과 같다면...
	if (FLOATZERO(t))
	{
		this->SetRotateWorld(fromQuat);
	}

	//t 가 1 과 같다면...
	else if (FLOATEQUAL(t, 1.0f))
	{
		this->SetRotateWorld(toQuat);
	}


	//0 과 1 사이의 값일때만 보간
	else
	{
		D3DXQUATERNION result;

		//from 과 to 사원수간의 t 만큼의 회전보간을 하여
		//result 사원수에 대입
		D3DXQuaternionSlerp(&result, &fromQuat, &toQuat, t);

		//보간된 사원수 정보로 회전 셋팅
		this->SetRotateWorld(result);
	}
}

// 자신의 위치 값을 from 과 to 사이의 위치만큼 선형보간 하여 적용
void cTransform::PositionLerp(const cTransform& from, const cTransform& to, float t)
{
	t = Clamp01(t);

	D3DXVECTOR3 fromWorldPos = from.GetWorldPosition();
	D3DXVECTOR3 toWorldPos = to.GetWorldPosition();

	//t 가 0 과 같다면...
	if (FLOATZERO(t))
	{
		this->SetWorldPosition(fromWorldPos);
	}

	//t 가 1 과 같다면...
	else if (FLOATEQUAL(t, 1.0f))
	{
		this->SetWorldPosition(toWorldPos);
	}


	//0 과 1 사이의 값일때만 보간
	else
	{
		D3DXVECTOR3 result;
		D3DXVec3Lerp(&result, &fromWorldPos, &toWorldPos, t);
		this->SetWorldPosition(result);
	}

}

// 자신의 모든 정보를 from 과 to 사이의 정보만큼 보간 하여 적용
void cTransform::Interpolate(const cTransform& from, const cTransform& to, float t)
{
	t = Clamp01(t);

	D3DXVECTOR3 resultScale;
	D3DXVECTOR3 resultPosition;
	D3DXQUATERNION resultRotate;

	//t 가 0 과 같다면...
	if (FLOATZERO(t))
	{
		resultScale = from.scale;
		resultPosition = from.GetWorldPosition();
		resultRotate = from.GetWorldRotateQuaternion();
	}

	//t 가 1 과 같다면...
	else if (FLOATEQUAL(t, 1.0f))
	{
		resultScale = to.scale;
		resultPosition = to.GetWorldPosition();
		resultRotate = to.GetWorldRotateQuaternion();
	}


	//0 과 1 사이의 값일때만 보간
	else
	{
		D3DXVECTOR3 fromScale = from.scale;
		D3DXVECTOR3 toScale = to.scale;

		D3DXVECTOR3 fromPosition = from.GetWorldPosition();
		D3DXVECTOR3 toPosition = to.GetWorldPosition();

		D3DXQUATERNION fromQuat = from.GetWorldRotateQuaternion();
		D3DXQUATERNION toQuat = to.GetWorldRotateQuaternion();


		//스케일 보간
		D3DXVec3Lerp(&resultScale, &fromScale, &toScale, t);

		//위치 보간
		D3DXVec3Lerp(&resultPosition, &fromPosition, &toPosition, t);

		//회전 보간
		D3DXQuaternionSlerp(&resultRotate, &fromQuat, &toQuat, t);


	}

	//너가 기존에 어떤 값을 지니고 있었니?
	bool bPrevAutoUpdate = this->bAutoUpdate;

	//일단 오토업데이트 막는다
	this->bAutoUpdate = false;

	this->SetScale(resultScale);
	this->SetWorldPosition(resultPosition);
	this->SetRotateWorld(resultRotate);

	//돌려
	this->bAutoUpdate = bPrevAutoUpdate;

	if (this->bAutoUpdate)
		this->UpdateTransform();

}


//
// Get 들..................
//


//최종 행렬을 얻는다.
D3DXMATRIXA16  cTransform::GetFinalMatrix() const
{
	return this->matFinal;
}

D3DXMATRIXA16 cTransform::GetWorldRotateMatrix() const
{
	//자신의 축으로 회전 행렬을 만들어 재낀다
	D3DXMATRIXA16 matRotate;
	D3DXMatrixIdentity(&matRotate);

	//3축을 얻는다.
	D3DXVECTOR3 axis[3];
	this->GetUnitAxies(axis);

	//행렬에 적용
	memcpy(&matRotate._11, axis + 0, sizeof(D3DXVECTOR3));
	memcpy(&matRotate._21, axis + 1, sizeof(D3DXVECTOR3));
	memcpy(&matRotate._31, axis + 2, sizeof(D3DXVECTOR3));

	return matRotate;
}
D3DXQUATERNION cTransform::GetWorldRotateQuaternion() const
{
	D3DXQUATERNION quat;

	D3DXMATRIXA16 matRotate = this->GetWorldRotateMatrix();

	//회전 행렬로 사원수를 만든다.
	D3DXQuaternionRotationMatrix(&quat, &matRotate);

	return quat;
}

//위치 값을 얻는다.
D3DXVECTOR3 cTransform::GetWorldPosition() const
{
	D3DXVECTOR3 pos = this->position;

	if (this->pParent){
		D3DXVec3TransformCoord(&pos, &pos, &this->pParent->matFinal);
	}

	return pos;
}
D3DXVECTOR3 cTransform::GetLocalPosition() const
{
	return this->position;
}

//축을 얻는다. ( 월드 기준 )
void cTransform::GetScaledAxies(D3DXVECTOR3* pVecArr) const
{
	for (int i = 0; i < 3; i++){
		pVecArr[i] = this->axis[i];
	}

	//부모가 있다면..
	if (this->pParent){
		D3DXMATRIXA16 matParentFinal = this->pParent->matFinal;
		for (int i = 0; i < 3; i++){
			D3DXVec3TransformNormal(&pVecArr[i], &pVecArr[i], &matParentFinal);
		}
	}

}
void cTransform::GetUnitAxies(D3DXVECTOR3* pVecArr) const
{
	for (int i = 0; i < 3; i++){
		D3DXVec3Normalize(pVecArr + i, this->axis + i);
	}

	//부모가 있다면..
	if (this->pParent){
		D3DXMATRIXA16 matParentFinal = this->pParent->matFinal;
		for (int i = 0; i < 3; i++){
			D3DXVec3TransformNormal(&pVecArr[i], &pVecArr[i], &matParentFinal);
		}
	}
}
D3DXVECTOR3 cTransform::GetScaledAxis(int axisNum) const
{
	D3DXVECTOR3 result = this->axis[axisNum];

	//부모가 있다면..
	if (this->pParent){
		D3DXMATRIXA16 matParentFinal = this->pParent->matFinal;
		D3DXVec3TransformNormal(&result, &result, &matParentFinal);
	}

	return result;
}
D3DXVECTOR3 cTransform::GetUnitAxis(int axisNum) const
{
	D3DXVECTOR3 result;
	D3DXVec3Normalize(&result, this->axis + axisNum);

	//부모가 있다면..
	if (this->pParent){
		D3DXMATRIXA16 matParentFinal = this->pParent->matFinal;
		D3DXVec3TransformNormal(&result, &result, &matParentFinal);
	}

	return result;
}
D3DXVECTOR3 cTransform::GetForward(bool bNormalize) const
{
	if (bNormalize)
		return this->GetUnitAxis(AXIS_Z);

	return this->GetScaledAxis(AXIS_Z);
}
D3DXVECTOR3 cTransform::GetUp(bool bNormalize) const
{
	if (bNormalize)
		return this->GetUnitAxis(AXIS_Y);

	return this->GetScaledAxis(AXIS_Y);
}
D3DXVECTOR3 cTransform::GetRight(bool bNormalize) const
{
	if (bNormalize)
		return this->GetUnitAxis(AXIS_X);

	return this->GetScaledAxis(AXIS_X);
}


//Transform 을 업데이트 한다 ( Trasform 의 정보가 갱신되었을대 사용된다 )
void cTransform::UpdateTransform()
{
	//자신의 정보로 matLocal 행렬을 갱신한다.
	D3DXMatrixIdentity(&this->matLocal);

	//스케일 먹은 축정보
	D3DXVECTOR3 scaledRight = this->right * this->scale.x;
	D3DXVECTOR3 scaledUp = this->up * this->scale.y;
	D3DXVECTOR3 scaledForward = this->forward * this->scale.z;

	memcpy(&this->matLocal._11, &scaledRight, sizeof(D3DXVECTOR3));
	memcpy(&this->matLocal._21, &scaledUp, sizeof(D3DXVECTOR3));
	memcpy(&this->matLocal._31, &scaledForward, sizeof(D3DXVECTOR3));
	memcpy(&this->matLocal._41, &this->position, sizeof(D3DXVECTOR3));

	//나의 최종 행렬
	if (this->pParent == NULL)
	{
		this->matFinal = matLocal;
	}

	//내가 부모가 있다면...
	else{
		this->matFinal = matLocal * this->pParent->matFinal;
	}

	//자식 업데이트
	cTransform* pChild = this->pFirstChild;

	//자식들아 너희들도 업데이트
	while (pChild != NULL)
	{
		pChild->UpdateTransform();
		pChild = pChild->pNextSibling;
	}

}

//Device 에 자신의 Tansform 을 월드로 셋팅
void cTransform::SetDeviceWorld(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetTransform(D3DTS_WORLD, &matFinal);
}

//Device 에 자신의 Tansform 을 뷰로 셋팅
void cTransform::SetDeviceView(LPDIRECT3DDEVICE9 pDevice)
{
	D3DXMATRIXA16 matView;
	D3DXMatrixInverse(&matView, NULL, &matFinal);

	pDevice->SetTransform(D3DTS_VIEW, &matView);

}


//디폴트 컨트롤 을해준다.
void cTransform::DefaultControl(float timeDelta)
{
	//디폴트 컨트롤을 위한 카메라 Angle 값
	static float nowAngleH = 0.0f;			//수평앵글
	static float nowAngleV = 0.0f;			//수직앵글
	static float maxAngleV = 85.0f;			//수직 최대 앵글
	static float minAngleV = -85.0f;			//수직 최저 앵글
	static float sensitivityH = 1.0f;					//가로 민감도
	static float sensitivityV = 1.0f;					//세로 민감도 ( 이값이 음수면 Invert Mouse )
	static D3DXVECTOR3 nowVelocity(0, 0, 0);			//현제 방향과 속도를 가진 벡터

	static float accelate = 30.0f;						//초당 이동 증가값
	static float nowSpeed = 3.0f;						//현재 속도
	static float maxSpeed = 10.0f;						//최고 속도 


	//최초 누를때는 마우스 위치를 가운데로 놓고 시작
	if (KEY_MGR->IsOnceDown(VK_RBUTTON)){

		//화면의 중심위치
		int screenCenterX = WINSIZE_X / 2;
		int screenCenterY = WINSIZE_Y / 2;

		//다시 마우스 위치를 센터로...
		SetMousePos(screenCenterX, screenCenterY);
	}

	//우클릭을 할때만 Default Control 을 한다
	else if (KEY_MGR->IsStayDown(VK_RBUTTON)){

		//
		// 이동 처리
		//

		//입력 방향벡터
		D3DXVECTOR3 inputVector(0, 0, 0);

		if (KEY_MGR->IsStayDown('W')){
			inputVector.z = 1.0f;
		}

		else if (KEY_MGR->IsStayDown('S')){
			inputVector.z = -1.0f;
		}

		if (KEY_MGR->IsStayDown('A')){
			inputVector.x = -1.0f;
		}

		else if (KEY_MGR->IsStayDown('D')){
			inputVector.x = 1.0f;
		}

		if (KEY_MGR->IsStayDown('R')){
			inputVector.y = 1.0f;
		}

		else if (KEY_MGR->IsStayDown('F')){
			inputVector.y = -1.0f;
		}

		//제로 벡터가 아닐때
		if (VECTORZERO(inputVector) == false)
		{
			//정규화
			D3DXVec3Normalize(&inputVector, &inputVector);
		}


		/*
		//
		// 가속개념 있는 이동
		//

		//타겟벡터
		D3DXVECTOR3 target = inputVector * maxSpeed;
		D3DXVec3TransformNormal( &target, &target, &this->matFinal );


		//타겟벡터와 현제Velocity 크기차
		float targetLength = D3DXVec3Length( &target ); //D3DXVec3Length 벡터의 크기를 얻는다.
		float nowVelocityLength = D3DXVec3Length( &nowVelocity );
		float dist = abs( targetLength - nowVelocityLength );

		//타겟과 차이가 존재한다면..
		if( FLOATZERO( dist ) == false )
		{
		//변화 허용량
		float delta = accelate * timeDelta;

		//보간값
		float t = Clamp01( delta / dist );

		//이번프레임에 타겟까지 변화 하는데 충분하다
		if( FLOATEQUAL( t, 1.0f ) )
		{
		nowVelocity = target;
		}

		else
		{
		nowVelocity = VecLerp( nowVelocity, target, t );
		}
		}

		//이동 벡터는
		D3DXVECTOR3 moveDeltaVector = nowVelocity * timeDelta;
		//이동 해라
		this->MovePositionWorld( moveDeltaVector );
		*/



		//
		// 가속개념 없는 이동
		//

		//이동 해라
		//타겟벡터 
		D3DXVECTOR3 target = inputVector * maxSpeed;
		this->MovePositionSelf(target * timeDelta);




		//
		// 회전 처리
		// 
		//화면의 중심위치
		int screenCenterX = WINSIZE_X / 2;
		int screenCenterY = WINSIZE_Y / 2;

		//현재 마우스 위치
		POINT mousePos = GetMousePos();

		//이동량 ( 중앙에서 멀어진 량 )
		float deltaX = mousePos.x - screenCenterX;
		float deltaY = mousePos.y - screenCenterY;

		//앵글 추가
		nowAngleH += deltaX * sensitivityH;
		nowAngleV += deltaY * sensitivityV;

		//앵글값을 min max 범위 안으로
		nowAngleV = Clamp(nowAngleV, minAngleV, maxAngleV);

		//다시 마우스 위치를 센터로...
		SetMousePos(screenCenterX, screenCenterY);


		this->SetRotateWorld(nowAngleV * ONE_RAD, nowAngleH * ONE_RAD, 0.0f);

	}


}

void cTransform::DefaultControl2(float timeDelta)
{
	float deltaMove = 3.0f * timeDelta;
	float deltaAngle = 90.0f * ONE_RAD * timeDelta;

	if (KEY_MGR->IsStayDown(VK_LSHIFT))
		deltaMove *= 5.0f;

	if (KEY_MGR->IsStayDown(VK_RBUTTON) == false)
	{
		if (KEY_MGR->IsStayDown('A'))
			this->MovePositionSelf(-deltaMove, 0.0f, 0.0f);
		else if (KEY_MGR->IsStayDown('D'))
			this->MovePositionSelf(deltaMove, 0.0f, 0.0f);

		if (KEY_MGR->IsStayDown('W'))
			this->MovePositionSelf(0.0f, 0.0f, deltaMove);
		else if (KEY_MGR->IsStayDown('S'))
			this->MovePositionSelf(0.0f, 0.0f, -deltaMove);

		if (KEY_MGR->IsStayDown(VK_LEFT))
			this->RotateSelf(0.0f, -deltaAngle, 0.0f);
		else if (KEY_MGR->IsStayDown(VK_RIGHT))
			this->RotateSelf(0.0f, deltaAngle, 0.0f);

		if (KEY_MGR->IsStayDown(VK_UP))
			this->RotateSelf(-deltaAngle, 0.0f, 0.0f);
		else if (KEY_MGR->IsStayDown(VK_DOWN))
			this->RotateSelf(deltaAngle, 0.0f, 0.0f);
	}
}



//Transform 에 대한 기즈모를 그린다.
void cTransform::RenderGimozo(bool applyScale) const
{
	D3DXVECTOR3 worldPos = this->GetWorldPosition();
	D3DXVECTOR3 axis[3];

	if (applyScale)
		this->GetScaledAxies(axis);
	else
		this->GetUnitAxies(axis);

	GIZMO_MGR->Line(worldPos, worldPos + axis[2], 0xff0000ff);
	GIZMO_MGR->Line(worldPos, worldPos + axis[1], 0xff00ff00);
	GIZMO_MGR->Line(worldPos, worldPos + axis[0], 0xffff0000);
}


//쉐이크 명령이 있으면 Update 를 해주어야 한다.
void cTransform::ShakeUpdate(float timeDelta)
{
	//위치에 대한 흔들림 파워가 있다면..
	if (this->ShakePosPower > 0.0f && this->ShakePosFlag != 0)
	{
		D3DXVECTOR3 localPos(0, 0, 0);
		if (this->ShakePosFlag & SHAKE_X)
			localPos.x = RandomFloatRange(-this->ShakePosPower, this->ShakePosPower);
		if (this->ShakePosFlag & SHAKE_Y)
			localPos.y = RandomFloatRange(-this->ShakePosPower, this->ShakePosPower);
		if (this->ShakePosFlag & SHAKE_Z)
			localPos.z = RandomFloatRange(-this->ShakePosPower, this->ShakePosPower);

		this->SetLocalPosition(localPos);

		this->ShakePosPower -= this->ShakePosRelease * timeDelta;

		if (this->ShakePosPower < 0.0f)
			this->ShakePosPower = 0.0f;
	}


	//회전에 대한 흔들림 파워가 있다면..
	if (this->ShakeRotPower > 0.0f && this->ShakeRotFlag != 0)
	{
		D3DXVECTOR3 localRot(0, 0, 0);
		if (this->ShakeRotFlag & SHAKE_X)
			localRot.x = RandomFloatRange(-this->ShakeRotPower, this->ShakeRotPower);
		if (this->ShakeRotFlag & SHAKE_Y)
			localRot.y = RandomFloatRange(-this->ShakeRotPower, this->ShakeRotPower);
		if (this->ShakeRotFlag & SHAKE_Z)
			localRot.z = RandomFloatRange(-this->ShakeRotPower, this->ShakeRotPower);

		this->SetRotateLocal(localRot.y, localRot.x, localRot.z);

		this->ShakeRotPower -= this->ShakeRotRelease * timeDelta;

		if (this->ShakeRotPower < 0.0f)
			this->ShakeRotPower = 0.0f;
	}



}

//위치에 대한 흔들림을 준다.
void cTransform::ShakePos(float shakePower, float shakeRelase)
{
	this->ShakePosPower = shakePower;
	this->ShakePosRelease = shakeRelase;

}

//회전에 대한 흔들림을 준다.
void cTransform::ShakeRot(float shakePower, float shakeRelase)
{
	this->ShakeRotPower = shakePower;
	this->ShakeRotRelease = shakeRelase;

}

//흔들림 축 설정
void cTransform::SetShakePosFlag(int flag)
{
	this->ShakePosFlag = flag;

}

void cTransform::SetShakeRotFlag(int flag)
{
	this->ShakeRotFlag = flag;

}