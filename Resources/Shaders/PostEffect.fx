
float pixelSizeU;			//
float pixelSizeV;
float pixelHalfSizeU;
float pixelHalfSizeV;

struct VS_INPUT{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
};

struct VS_OUTPUT{
	float4 pos : POSITION;
	float2 uv : TEXCOORD0;
};


VS_OUTPUT vs_Base(VS_INPUT Input)
{
	VS_OUTPUT Output;
	Output.pos = Input.pos;
	Output.uv = Input.uv;

	return Output;
}


// Base ///////////////////////////////////////////////////////////////

//Sampler
texture screenTex;
sampler2D screenSampler = sampler_state{
	Texture = screenTex;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
	
};

float4 ps_Base( VS_OUTPUT Input ) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	return tex2D( screenSampler, Input.uv );

	//return float4(tex2D(screenSampler, Input.uv).www, 1);

}


// OutLine ///////////////////////////////////////////////////////////////

//Sampler
texture normalTex;
sampler2D normalSampler = sampler_state{
	Texture = normalTex;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
};


float mask[9] = {
	-1, -1, -1,
	-1,  8, -1,
	-1, -1, -1 };		//라플라스 필터

float coordX[3] = { -1, 0, 1 };
float coordY[3] = { -1, 0, 1 };


float4 ps_Outline(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float outlineStrength = 0.0f;

	float pX = pixelSizeU;
	float pY = pixelSizeV;

	//자신을 포함한 주변 픽셀정보 9개 
	for (int i = 0; i < 9; i++)
	{
		float2 uv = Input.uv + float2(coordX[i % 3] * pX, coordY[i / 3] * pY);

			float3 normal = tex2D(normalSampler, uv).rgb * mask[i];

		outlineStrength += normal.x;
		outlineStrength += normal.y;
		outlineStrength += normal.z;
	}

	outlineStrength = saturate( abs( outlineStrength ) );
	
	float4 outLineColor = float4(0, 0, 0, 1);
	float4 baseColor = tex2D(screenSampler, Input.uv);

	float4 finalColor = lerp(baseColor, outLineColor, outlineStrength);

	return finalColor;
}

// BlurX ///////////////////////////////////////////////////////////////

float BlurWeights[13] =
{
	0.002216,		// -6
	0.008764,       // -5
	0.026995,       // -4
	0.064759,       // -3
	0.120985,       // -2
	0.176033,       // -1
	0.199471,		//Center
	0.176033,		//	1
	0.120985,		//	2
	0.064759,		//	3
	0.026995,		//	4
	0.008764,		//	5
	0.002216		//	6
};

float bulrPos[13] =
{
	-6,		
	-5,     
	-4,     
	-3,     
	-2,     
	-1,     
	0,		
	1,		
	2,		
	3,		
	4,		
	5,		
	6
};

float blurScale = 1.0f;			//블러스케일

float4 ps_BlurX(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float4 finalColor = float4(0, 0, 0, 1);

	for (int i = 0; i < 13; i++)
	{
		float2 uv = Input.uv + float2(bulrPos[i] * pixelSizeU, 0) * blurScale;

		//uv = saturate(uv);
		uv.x = max(pixelHalfSizeU, uv.x);
		uv.x = min(1 - pixelHalfSizeU, uv.x);

		uv.y = max(pixelHalfSizeV, uv.y);
		uv.y = min(1 - pixelHalfSizeV, uv.y);

		finalColor += tex2D(screenSampler, uv) * BlurWeights[i];
	}

	return finalColor;
}



float4 ps_BlurY(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float4 finalColor = float4(0, 0, 0, 1);

	for (int i = 0; i < 13; i++)
	{
		float2 uv = Input.uv + float2(0, bulrPos[i] * pixelSizeV) * blurScale;
		//uv = saturate(uv);
		uv.x = max(pixelHalfSizeU, uv.x);
		uv.x = min(1 - pixelHalfSizeU, uv.x);

		uv.y = max(pixelHalfSizeV, uv.y);
		uv.y = min(1 - pixelHalfSizeV, uv.y);

		finalColor += tex2D(screenSampler, uv) * BlurWeights[i];
	}

	return finalColor;
}


// BBO ///////////////////////////////////////////////////////////////


//Sampler
texture blurTex;
sampler2D blurSampler = sampler_state{
	Texture = blurTex;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
};

float blurAmout = 3.0f;

float4 ps_BBO(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;
	
	float4 orignalTex = tex2D(screenSampler, Input.uv);
	float4 blurTex = tex2D(blurSampler, Input.uv);

	return orignalTex + blurTex * blurAmout;
}

// Depth Of Field ///////////////////////////////////////////////////////////////

float focus = 0.01f;
float fieldRange = 5.0f;

float4 ps_DepthOfField(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float4 orignalTex = tex2D(screenSampler, Input.uv);
	float4 blurTex = tex2D(blurSampler, Input.uv);
	float depth = tex2D(normalSampler, Input.uv).a;


	return lerp(orignalTex, blurTex, saturate(fieldRange * abs(focus - depth)));
}

// Gray ///////////////////////////////////////////////////////////////

float4 ps_Gray(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float4 texColor = tex2D(screenSampler, Input.uv);

	float gray = texColor.r * 0.299f + texColor.g * 0.587f + texColor.b * 0.114f;
	return float4(gray, gray, gray, 1);
}



// ColorLevel ///////////////////////////////////////////////////////////////

float InBlack = 0.0f;			//컬러를 0 ~ 255 로 볼때 이값 미만이면 0 으로 처리한다.
float InWhite = 255.0f;			//컬러를 0 ~ 255 로 볼때 이값 이상이면 1 으로 처리한다.
float Gamma = 3.0f;				//증폭 
float OutBlack = 0.0f;			//감마 처리 이후 얻어진 값을 0 ~ 255 로 볼때 이값 미만이면 0 으로 처리한다.
float OutWhite = 255.0f;		//감마 처리 이후 얻어진 값을 0 ~ 255 로 볼때 이값 이상이면 1 으로 처리한다.



float LevelSet(float value){

	// 0 ~ 1 사이의 범위 의 컬러 값을 0 ~ 255 범위로...
	float c = value * 255.0f;

	//0 밑으로 안내려 가게....
	c = max(0, c - InBlack);

	//컬러 값 InBlack 에서 InWhite 에 대한 범위의 Factor 값 
	//이과정에서 c 는 0 ~ 1 사이의 값이 된다.
	c = saturate(c / ( InWhite - InBlack));

	//saturate ( value )  =  value 값은 0 ~ 1 사이의 값으로 Clamp 된다.

	//감마 처리 
	c = pow(c, Gamma);

	//컬러 Out 처리 	( 여기서 다시 0 ~ 255 의 컬러 범위 값이 된다 )	
	// c = 0.5
	// OutBlack = 100
	// OutWhite = 200
	// 최종값은 = 150 
	c = c * ( OutWhite - OutBlack) +  OutBlack;

	//최종 컬러 
	c = saturate(c / 255.0f);

	return c;
}


float4 ps_ColorLevel(VS_OUTPUT Input) : COLOR0
{
	Input.uv.x += pixelHalfSizeU;
	Input.uv.y += pixelHalfSizeV;

	float4 texColor = tex2D(screenSampler, Input.uv);

	float4 finalColor;
	finalColor.r = LevelSet(texColor.r);
	finalColor.g = LevelSet(texColor.g);
	finalColor.b = LevelSet(texColor.b);
	finalColor.a = 1.0f;

	return finalColor;
}






technique Base
{
    pass p0
    {
		ZWRITEENABLE = FALSE;
        VertexShader = compile vs_3_0 vs_Base();
        PixelShader = compile ps_3_0 ps_Base();
    }
}

technique OutLine
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_Outline();
	}
}

technique BlurX
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_BlurX();
	}
}

technique BlurY
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_BlurY();
	}
}


technique BBo
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_BBO();
	}
}


technique DepthOfField
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_DepthOfField();
	}
}


technique Gray
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_Gray();
	}
}


technique ColorLevel
{
	pass p0
	{
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 vs_Base();
		PixelShader = compile ps_3_0 ps_ColorLevel();
	}
}