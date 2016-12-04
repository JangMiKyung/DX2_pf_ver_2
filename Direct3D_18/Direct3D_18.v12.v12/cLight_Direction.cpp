#include "stdafx.h"
#include "cLight_Direction.h"
#include "cTransform.h"

cLight_Direction::cLight_Direction()
{
}


cLight_Direction::~cLight_Direction()
{
}

D3DXMATRIXA16 cLight_Direction::GetLightMatrix()
{
	// 라이트 행렬
	// Pos.x,         Pos.y,          Pos.z,         lightType ( 0 Direction, 1 Point, 2 Spot )
	// Dir.x,         Dir.y,          Dir.z,         DistancePow;
	// Color.r,       Color.g,        Color.b,       strength
	// MaxRange,      MinRange,       InAngle        OutAngle 

	D3DXMATRIXA16 matLight;
	D3DXMatrixIdentity(&matLight);
	matLight._14 = 0;		//Directional Light Type
	D3DXVECTOR3 forward = this->pTransform->GetForward();
	memcpy(&matLight._21, &forward, sizeof(D3DXVECTOR3));
	memcpy(&matLight._31, &Color, sizeof(FLOAT) * 3);
	matLight._34 = this->Intensity;

	return matLight;
}

void cLight_Direction::RenderGizmo()
{
	this->pTransform->RenderGimozo();
}