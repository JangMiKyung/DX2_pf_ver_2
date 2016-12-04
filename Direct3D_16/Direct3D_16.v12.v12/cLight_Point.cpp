#include "stdafx.h"
#include "cLight_Point.h"
#include "cTransform.h"

cLight_Point::cLight_Point()
{
}


cLight_Point::~cLight_Point()
{
}

D3DXMATRIXA16 cLight_Point::GetLightMatrix()
{
	// 라이트 행렬
	// Pos.x,         Pos.y,          Pos.z,         lightType ( 0 Direction, 1 Point, 2 Spot )
	// Dir.x,         Dir.y,          Dir.z,         DistancePow;
	// Color.r,       Color.g,        Color.b,       strength
	// MaxRange,      MinRange,       InAngle        OutAngle 

	D3DXMATRIXA16 matLight;
	D3DXMatrixIdentity(&matLight);
	matLight._14 = 1;		//Point Light Type
	D3DXVECTOR3 pos = this->pTransform->GetWorldPosition();
	memcpy(&matLight._11, &pos, sizeof(D3DXVECTOR3));
	memcpy(&matLight._31, &Color, sizeof(FLOAT) * 3);
	matLight._24 = distancePow;
	matLight._41 = this->maxRange;
	matLight._42 = this->minRange;
	matLight._34 = this->Intensity;

	return matLight;
}

void cLight_Point::RenderGizmo()
{
	this->pTransform->RenderGimozo();
}