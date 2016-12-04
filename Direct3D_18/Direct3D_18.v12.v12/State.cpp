#include "stdafx.h"
#include "State.h"
#include "cTransform.h"

//AI�� �ڵ��̵� �����Ҷ� ������, �������� ������� �����ϸ� ������ �Ἥ �̵���~
void State::Move(float timeUpdate, cTransform Sour, cTransform Dest)
{
	cTransform Trans;
	D3DXVECTOR3 Dist = Sour.GetLocalPosition() - Dest.GetLocalPosition();
	Trans.LookPosition(Dist);

	//������ �̿��� �����ϴµ� �Ÿ��� ��� ª�簣�� ���ǵ�� �����ؾ� �ϴ�,  ���ǵ� �ٽ� ���
	float Speed = fabs(m_Speed / (float)D3DXVec3Length(&Dist));

	if (isActive)
	{
		MoveLerfPt += Speed;
		m_Trans->SetLocalPosition(MyUtil::VecLerp(Sour.GetWorldPosition(), Dest.GetWorldPosition(), MoveLerfPt));

		if(RotateLerfPt <= 1.0)
		m_Trans->RotateSlerp(Sour , Trans, RotateLerfPt += m_RotateSpeed);

		if (MoveLerfPt >= 1.0)
		{
			RotateLerfPt = 0;
			MoveLerfPt = 0;
			isActive = false;
		}
	}
}

// �÷��̾� ���������� ���� �Լ���... �÷��̾� ���� ���� �Ⱦ���.
void State::Move(float timeUpdate)
{
	//	float deltaAngle = 90.0f * ONE_RAD * timeDelta;
	if (KEY_MGR->IsStayDown('A'))
	{
		m_Trans->MovePositionSelf(m_Speed * timeUpdate, 0.0f, 0.0f);
	}
	else if (KEY_MGR->IsStayDown('D'))
	{
		m_Trans->MovePositionSelf(-m_Speed* timeUpdate, 0.0f, 0.0f);
	}

	if (KEY_MGR->IsStayDown('W'))
	{
		m_Trans->MovePositionSelf(0.0f, 0.0f, -m_Speed* timeUpdate);
	}
	else if (KEY_MGR->IsStayDown('S'))
	{
		m_Trans->MovePositionSelf(0.0f, 0.0f, m_Speed* timeUpdate);
	}

}


// �����Ͽ� Angle��ŭ ȸ����.
void State::Rotate(float timeUpdate, float Angle)
{
	if (isRotateActive)
	{
		RotateLerfPt += m_RotateSpeed;

		if (RotateLerfPt < D3DXToDegree(Angle))
		{
			m_Trans->RotateSelf({ 0, D3DXToRadian(m_RotateSpeed) , 0 });
		}
		else
		{
			isRotateActive = false;
			RotateLerfPt = 0;
		}
	}
}


