#include "AddLight.fx"

float camNear;			//카메라 근거리 평면
float camFar;			//카메라 원거리 평면

texture Diffuse_Tex;
sampler2D Diffuse = sampler_state
{
	Texture = (Diffuse_Tex);
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
};
texture Normal_Tex;
sampler2D Normal = sampler_state
{
	Texture = (Normal_Tex);
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
};

texture Specular_Tex;
sampler2D Specular = sampler_state
{
	Texture = (Specular_Tex);
	MAGFILTER = LINEAR;
	MINFILTER = LINEAR;
	MIPFILTER = LINEAR;
};


texture Emission_Tex;
sampler2D Emission = sampler_state
{
	Texture = (Emission_Tex);
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
	MINFILTER = LINEAR;
};




float4x4 baseDirectionLight;			//기본라이팅 행렬


struct PS_INPUT
{
	float2 Texcoord : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 Binormal : TEXCOORD2;
	float3 Tangent : TEXCOORD3;
	float3 viewDir : TEXCOORD4;
	float3 worldPos : TEXCOORD5;
	float4 FinalPos : TEXCOORD6;
};

//픽셀셰이더 출력 구조체
struct PS_OUTPUT
{
	float4 baseColor : COLOR0;			//0번 스테이지 컬러
	float4 normalDepth : COLOR1;		//1번 스테이지 컬러 ( RGB 노말, A 뎁스 )
};

//---------------------------------------------------------------
// Render Base 관련
//---------------------------------------------------------------

PS_OUTPUT ps_main(PS_INPUT Input)
{
	PS_OUTPUT Output = (PS_OUTPUT)0;

	float4 diffTex = tex2D(Diffuse, Input.Texcoord);
		clip(diffTex.a - 0.1f);

	//TBN Matrix
	float3x3 TBN = float3x3(
		normalize(Input.Tangent),
		normalize(Input.Binormal),
		normalize(Input.Normal));

	//   
	// WorldNormal
	//
	float3 norColor = tex2D(Normal, Input.Texcoord).rgb;

		//Tangent Space Normal
		float3 spaceNor = (norColor * 2.0f) - 1.0f;

		float3 worldNormal = mul(spaceNor, TBN);
		float3 viewDir = normalize(Input.viewDir);

		//최종 색
		float3 finalDiffuse = float3(0, 0, 0);
		float3 finalSpecular = float3(0, 0, 0);

		//기본 라이팅 처리

		//광원의 방향
		float3 dir = float3(baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23);
		float3 lightDir = -dir;

		//광원의 컬러
		float3 lightColor = float3(baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33) * baseDirectionLight._34;


		float NdotL = dot(lightDir, worldNormal);

	//Ambient
	//float diff = saturate( NdotL );

	//float diff = max( 0.1f, NdotL );

	float diff = NdotL;
	if (diff < 0.0)
		diff = abs(NdotL) * 0.3f;


	//라이트 반사
	float3 lightRefl = normalize(dir + 2.0f * NdotL * worldNormal);
		float spec = saturate(dot(lightRefl, viewDir));
	spec = pow(spec, fSpecPower);

	//기본 광원 적용
	finalDiffuse = lightColor * diff;
	finalSpecular = lightColor * spec * diff;		//Specular 에 diff 안곱하면 Specular 에 의한 역광이 나온다...

	//추가되는 광원
	float3 addDiffuse = float3(0, 0, 0);
		float3 addSpecular = float3(0, 0, 0);
	for (int i = 0; i < LightNum; i++)
	{
		//i 인덱스의 광원이 계산되어 addDiffuse, addSpecular 에 대입된다.
		//AddLingt.fx
		ComputeLight(
			addDiffuse,
			addSpecular,
			Input.worldPos,
			worldNormal,
			viewDir,
			i);

		//최종 광원 량에 쌓인다.
		finalDiffuse += addDiffuse;
		finalSpecular += addSpecular;
	}


	//
	// Diffuse
	//
	float3 diffuseColor = diffTex.rgb * finalDiffuse;

		//
		// Specular 
		//
		float3 specularColor = tex2D(Specular, Input.Texcoord).rgb * finalSpecular;

		//
		// Emission
		//
		float3 emissionColor = tex2D(Emission, Input.Texcoord).rgb;

		//
		// Final Color 
		//
		float3 finalColor = diffuseColor + specularColor + emissionColor;


		//행렬변환을 거친 값 z 에 행렬변환에서 얻는 가중치 w 를 나누면 0 ~ 1 사이의 깊이 값이 된다.
		float depth = Input.FinalPos.z / Input.FinalPos.w;

	//위의 depth 값을 카메라의 near 와 far 를 이용하여 선형으로 펴준다....
	//Perspective Projection Linear Depth
	float z = depth;
	float a = camFar / (camFar - camNear);
	float b = -camNear / (camFar - camNear);
	depth = b / (z - a);


	Output.baseColor = float4(finalColor, 1);
	Output.normalDepth = float4(worldNormal, depth);		//alpha 값에 뎁스를 썼다.


	return Output;
}




//---------------------------------------------------------------
// CreateShadow 관련
//---------------------------------------------------------------

struct PS_INPUT_SHADOW
{
	float4 FinalPos : TEXCOORD0;
	float2 Texcoord : TEXCOORD1;
};

float4 ps_CreateShadow(PS_INPUT_SHADOW Input) : COLOR0
{
	//행렬변환을 거친 값 z 에 행렬변환에서 얻는 가중치 w 를 나누면 0 ~ 1 사이의 깊이 값이 된다.
	float depth = Input.FinalPos.z / Input.FinalPos.w;

	float4 diffTex = tex2D(Diffuse, Input.Texcoord);
		clip(diffTex.a - 0.1f);

	return float4(depth.xxx, 1);
}


//--------------------------------------------------------------//
// Render With ShadowMap 
//--------------------------------------------------------------//


float4x4 matLightViewProjection;			//방향성 광원 ViewProjection 행렬

//쉐도우 Texture
texture Shadow_Tex;
sampler2D ShadowSampler = sampler_state
{
	Texture = (Shadow_Tex);
	ADDRESSU = BORDER;
	ADDRESSV = BORDER;
	BORDERCOLOR = 0xFFFFFFFF;
};

float bias = 0.01f;



struct PS_INPUT_RECIVESHADOW
{
	float2 Texcoord : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 Binormal : TEXCOORD2;
	float3 Tangent : TEXCOORD3;
	float3 viewDir : TEXCOORD4;
	float3 worldPos : TEXCOORD5;

	float4 FinalPos : TEXCOORD6;
	float4 LightClipPos : TEXCOORD7;		//광원 입장에서 바라본 위치
};


PS_OUTPUT ps_ReciveShadow(PS_INPUT_RECIVESHADOW Input)
{
	PS_OUTPUT Output = (PS_OUTPUT)0;

	float4 diffTex = tex2D(Diffuse, Input.Texcoord);
		clip(diffTex.a - 0.1f);

	//광원 입장에서 바라본 위치의 뎁스 값 ( 라이트 행렬을 직교이기때문에 선형으로 안핀다 )
	float lightDepth = Input.LightClipPos.z / Input.LightClipPos.w;

	//Shadow 맵의 UV 추적
	//( -1 ~ 1 )
	float2 shadowUV = Input.LightClipPos.xy / Input.LightClipPos.w;
		shadowUV.y = -shadowUV.y;		//y반전...
	//0 ~ 1 의 범위
	shadowUV = (shadowUV * 0.5f) + 0.5f;

	//현제 그려지는 놈이 위치에 DirectionLight 입장에서 써진 깊이값...
	float shadowDepth = tex2D(ShadowSampler, shadowUV).r;

	//그림자가 그려지는 상황은 shadowDepth + bias 값 보다 lightDepth 가 큰경우이다.




	//TBN Matrix
	float3x3 TBN = float3x3(
		normalize(Input.Tangent),
		normalize(Input.Binormal),
		normalize(Input.Normal));

	//   
	// WorldNormal
	//
	float3 norColor = tex2D(Normal, Input.Texcoord).rgb;

		//Tangent Space Normal
		float3 spaceNor = (norColor * 2.0f) - 1.0f;

		float3 worldNormal = mul(spaceNor, TBN);
		worldNormal = normalize(worldNormal);
	float3 viewDir = normalize(Input.viewDir);

		//최종 색
		float3 finalDiffuse = float3(0, 0, 0);
		float3 finalSpecular = float3(0, 0, 0);

		//기본 라이팅 처리

		//광원의 방향
		float3 dir = float3(baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23);
		float3 lightDir = -dir;

		//광원의 컬러
		float3 lightColor = float3(baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33) * baseDirectionLight._34;


		float NdotL = dot(lightDir, worldNormal);

	//Ambient
	//float diff = saturate( NdotL );

	//float diff = max( 0.1f, NdotL );

	float diff = NdotL;

	//그림자가 그려져야 한다면 diff 는 무조건 음수가 된다 
	if (shadowDepth + bias < lightDepth && lightDepth < 1.0f)
		diff = abs(diff) * -1.0f;

	if (diff < 0.0)
		diff = abs(diff) * 0.3f;

	//라이트 반사
	float3 lightRefl = normalize(dir + 2.0f * NdotL * worldNormal);
		float spec = saturate(dot(lightRefl, viewDir));
	spec = pow(spec, fSpecPower);

	//기본 광원 적용
	finalDiffuse = lightColor * diff;
	finalSpecular = lightColor * spec * diff;		//Specular 에 diff 안곱하면 Specular 에 의한 역광이 나온다...


	//추가되는 광원
	float3 addDiffuse = float3(0, 0, 0);
		float3 addSpecular = float3(0, 0, 0);
	for (int i = 0; i < LightNum; i++)
	{
		//i 인덱스의 광원이 계산되어 addDiffuse, addSpecular 에 대입된다.
		ComputeLight(
			addDiffuse,
			addSpecular,
			Input.worldPos,
			worldNormal,
			viewDir,
			i);

		//최종 광원 량에 쌓인다.
		finalDiffuse += addDiffuse;
		finalSpecular += addSpecular;
	}


	//
	// Diffuse
	//
	float3 diffuseColor = diffTex.rgb * finalDiffuse;

		//
		// Specular 
		//
		float3 specularColor = tex2D(Specular, Input.Texcoord).rgb * finalSpecular;

		//
		// Emission
		//
		float3 emissionColor = tex2D(Emission, Input.Texcoord).rgb;

		//
		// Final Color 
		//
		float3 finalColor = diffuseColor + specularColor + emissionColor;



		//행렬변환을 거친 값 z 에 행렬변환에서 얻는 가중치 w 를 나누면 0 ~ 1 사이의 깊이 값이 된다.
		float depth = Input.FinalPos.z / Input.FinalPos.w;

	//위의 depth 값을 카메라의 near 와 far 를 이용하여 선형으로 펴준다....
	//Perspective Projection Linear Depth
	float z = depth;
	float a = camFar / (camFar - camNear);
	float b = -camNear / (camFar - camNear);
	depth = b / (z - a);


	Output.baseColor = float4(finalColor, 1);
	Output.normalDepth = float4(worldNormal, depth);		//alpha 값에 뎁스를 썼다.

	return Output;
}




//--------------------------------------------------------------//
// Render Toon
//--------------------------------------------------------------//

PS_OUTPUT ps_Toon(PS_INPUT Input)
{
	PS_OUTPUT Output = (PS_OUTPUT)0;


	float4 diffTex = tex2D(Diffuse, Input.Texcoord);
		clip(diffTex.a - 0.1f);

	float3 worldNormal = normalize(Input.Normal);

	//광원의 방향
	float3 dir = float3(baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23);
	float3 lightDir = -dir;

	//광원의 컬러
	float3 lightColor = float3(baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33) * baseDirectionLight._34;


	float diff = dot(worldNormal, lightDir);
	diff = (diff + 1.0f) * 0.5f;

	float toon = 0.0f;

	//광원의 적용량을 단계 별로 나눈다.
	float interval = 1.0 / 4.0f;		//5단계
	toon = floor(diff / interval) * interval;		//floor 가 소숫점재낀다..
	toon = max(0.3f, toon);



	//최종 색
	float3 finalDiffuse = lightColor * toon;

		//추가되는 광원
		float3 addDiffuse = float3(0, 0, 0);
	for (int i = 0; i < LightNum; i++)
	{
		//i 인덱스의 광원이 계산되어 addDiffuse, addSpecular 에 대입된다.
		ComputeLightToon(
			addDiffuse,
			Input.worldPos,
			worldNormal,
			i);

		//최종 광원 량에 쌓인다.
		finalDiffuse += addDiffuse;
	}


	//RampTex 를 가져온다.
	//float3 rampRgb = tex2D( Ramp, float2( diff, 0 ) ).rgb;
	//float3 diffuseColor = tex2D( Diffuse, Input.Texcoord ).rgb * rampRgb;

	float3 finalColor = diffTex.rgb * finalDiffuse;



		//행렬변환을 거친 값 z 에 행렬변환에서 얻는 가중치 w 를 나누면 0 ~ 1 사이의 깊이 값이 된다.
		float depth = Input.FinalPos.z / Input.FinalPos.w;

	//위의 depth 값을 카메라의 near 와 far 를 이용하여 선형으로 펴준다....
	//Perspective Projection Linear Depth
	float z = depth;
	float a = camFar / (camFar - camNear);
	float b = -camNear / (camFar - camNear);
	depth = b / (z - a);


	Output.baseColor = float4(finalColor, 1);
	Output.normalDepth = float4(worldNormal, depth);		//alpha 값에 뎁스를 썼다.

	return Output;
}


//--------------------------------------------------------------//
// Render Distort
//--------------------------------------------------------------//

//이전까지 그려진 화면의 내용
texture GrabTex;
sampler2D GrabSampler = sampler_state
{
	Texture = (GrabTex);
	ADDRESSU = BORDER;
	ADDRESSV = BORDER;
	BORDERCOLOR = 0xFFFFFFFF;
};

float distortAmout = 0.01f;
float scrollU = 0.0f;

PS_OUTPUT ps_Distort(PS_INPUT Input)
{

	PS_OUTPUT Output = (PS_OUTPUT)0;

	//지금 내가 그려지는 위치의 화면 좌표를 컬러로 그려보자...
	float2 screenUV = Input.FinalPos.xy / Input.FinalPos.w;

	//-1 과 1 사이의 값을 0 ~ 1 사이의 값으로..
	screenUV = (screenUV + 1.0f) * 0.5f;

	//Y 값 반전
	screenUV.y = 1.0f - screenUV.y;
	
	float2 texUV = Input.Texcoord;
	texUV.y += scrollU;

	//노말값 얻는다.
	float2 norXY = tex2D(Normal, texUV).xy;
	//노말값을 -1 ~ 1 사이로..
	norXY = (norXY * 2.0f) - 1.0f;

	//노말 값만큼 UV 외곡
	float2 uv = screenUV + norXY * distortAmout;
	//uv = saturate(uv);


	Output.baseColor = tex2D(GrabSampler, uv);

	return Output;
}



//--------------------------------------------------------------//
// Render With ShadowMap Toon
//--------------------------------------------------------------//

PS_OUTPUT ps_ReciveShadowToon(PS_INPUT_RECIVESHADOW Input) 
{
	PS_OUTPUT Output = (PS_OUTPUT)0;

	float4 diffTex = tex2D(Diffuse, Input.Texcoord);
		clip(diffTex.a - 0.1f);

	//광원 입장에서 바라본 위치의 뎁스 값 ( 라이트 행렬을 직교이기때문에 선형으로 안핀다 )
	float lightDepth = Input.LightClipPos.z / Input.LightClipPos.w;

	//Shadow 맵의 UV 추적
	//( -1 ~ 1 )
	float2 shadowUV = Input.LightClipPos.xy / Input.LightClipPos.w;
		shadowUV.y = -shadowUV.y;		//y반전...
	//0 ~ 1 의 범위
	shadowUV = (shadowUV * 0.5f) + 0.5f;

	//현제 그려지는 놈이 위치에 DirectionLight 입장에서 써진 깊이값...
	float shadowDepth = tex2D(ShadowSampler, shadowUV).r;

	//그림자가 그려지는 상황은 shadowDepth + bias 값 보다 lightDepth 가 큰경우이다.


	float3 worldNormal = normalize(Input.Normal);

		//광원의 방향
		float3 dir = float3(baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23);
		float3 lightDir = -dir;

		//광원의 컬러
		float3 lightColor = float3(baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33) * baseDirectionLight._34;


		float diff = dot(worldNormal, lightDir);
	diff = (diff + 1.0f) * 0.5f;

	float toon = 0.0f;
	//그림자가 그려져야 한다면 diff 는 0.3f 가 된다 
	if (shadowDepth + bias < lightDepth && lightDepth < 1.0f)
		toon = 0.3f;

	else
	{
		//광원의 적용량을 단계 별로 나눈다.
		float interval = 1.0 / 4.0f;		//5단계
		toon = floor(diff / interval) * interval;		//floor 가 소숫점재낀다..
		toon = max(0.3f, toon);

	}



	//최종 색
	float3 finalDiffuse = lightColor * toon;

		//추가되는 광원
		float3 addDiffuse = float3(0, 0, 0);
	for (int i = 0; i < LightNum; i++)
	{
		//i 인덱스의 광원이 계산되어 addDiffuse, addSpecular 에 대입된다.
		ComputeLightToon(
			addDiffuse,
			Input.worldPos,
			worldNormal,
			i);

		//최종 광원 량에 쌓인다.
		finalDiffuse += addDiffuse;
	}


	//RampTex 를 가져온다.
	//float3 rampRgb = tex2D( Ramp, float2( diff, 0 ) ).rgb;
	//float3 diffuseColor = tex2D( Diffuse, Input.Texcoord ).rgb * rampRgb;

	float3 finalColor = diffTex.rgb * finalDiffuse;



	//행렬변환을 거친 값 z 에 행렬변환에서 얻는 가중치 w 를 나누면 0 ~ 1 사이의 깊이 값이 된다.
	float depth = Input.FinalPos.z / Input.FinalPos.w;

	//위의 depth 값을 카메라의 near 와 far 를 이용하여 선형으로 펴준다....
	//Perspective Projection Linear Depth
	float z = depth;
	float a = camFar / (camFar - camNear);
	float b = -camNear / (camFar - camNear);
	depth = b / (z - a);

	Output.baseColor = float4(finalColor, 1);
	Output.normalDepth = float4(worldNormal, depth);		//alpha 값에 뎁스를 썼다.

	return Output;
}
