#include "stdafx.h"
#include "State.h"
#include "cTransform.h"

//AI나 자동이동 구현할때 쓸듯함, 목적지와 출발지를 설정하면 보간법 써서 이동함~
void State::Move(float timeUpdate, cTransform Sour, cTransform Dest)
{
	cTransform Trans;
	D3DXVECTOR3 Dist = Sour.GetLocalPosition() - Dest.GetLocalPosition();
	Trans.LookPosition(Dist);

	//보간을 이용해 가야하는데 거리가 길든 짧든간에 스피드는 일정해야 하니,  스피드 다시 계산
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

// 플레이어 조종용으로 만든 함수임... 플레이어 말곤 딱히 안쓸듯.
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


// 보간하여 Angle만큼 회전함.
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


