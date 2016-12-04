#pragma once

class cTransform;

class cLight
{
public:
	cTransform*			pTransform;			//라이트 트랜스폼
	D3DXCOLOR			Color;				//라이트 컬러
	float				Intensity;			//라이트 강도


public:
	cLight();
	~cLight();

	// 라이트 행렬
	// Pos.x,         Pos.y,          Pos.z,         lightType ( 0 Direction, 1 Point, 2 Spot )
	// Dir.x,         Dir.y,          Dir.z,         DistancePow;
	// Color.r,       Color.g,        Color.b,       strength
	// MaxRange,      MinRange,       InAngle        OutAngle 

	//라이트 행렬을 얻는다.
	virtual D3DXMATRIXA16 GetLightMatrix() = 0;

	//라이트 기즈모를 그린다.
	virtual void RenderGizmo() = 0;
};

