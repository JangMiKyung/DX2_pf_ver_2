#include "AddLight.fx"

float camNear;			//ī�޶� �ٰŸ� ���
float camFar;			//ī�޶� ���Ÿ� ���

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




float4x4 baseDirectionLight;			//�⺻������ ���


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

//�ȼ����̴� ��� ����ü
struct PS_OUTPUT
{
	float4 baseColor : COLOR0;			//0�� �������� �÷�
	float4 normalDepth : COLOR1;		//1�� �������� �÷� ( RGB �븻, A ���� )
};

//---------------------------------------------------------------
// Render Base ����
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

		//���� ��
		float3 finalDiffuse = float3(0, 0, 0);
		float3 finalSpecular = float3(0, 0, 0);

		//�⺻ ������ ó��

		//������ ����
		float3 dir = float3(baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23);
		float3 lightDir = -dir;

		//������ �÷�
		float3 lightColor = float3(baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33) * baseDirectionLight._34;


		float NdotL = dot(lightDir, worldNormal);

	//Ambient
	//float diff = saturate( NdotL );

	//float diff = max( 0.1f, NdotL );

	float diff = NdotL;
	if (diff < 0.0)
		diff = abs(NdotL) * 0.3f;


	//����Ʈ �ݻ�
	float3 lightRefl = normalize(dir + 2.0f * NdotL * worldNormal);
		float spec = saturate(dot(lightRefl, viewDir));
	spec = pow(spec, fSpecPower);

	//�⺻ ���� ����
	finalDiffuse = lightColor * diff;
	finalSpecular = lightColor * spec * diff;		//Specular �� diff �Ȱ��ϸ� Specular �� ���� ������ ���´�...

	//�߰��Ǵ� ����
	float3 addDiffuse = float3(0, 0, 0);
		float3 addSpecular = float3(0, 0, 0);
	for (int i = 0; i < LightNum; i++)
	{
		//i �ε����� ������ ���Ǿ� addDiffuse, addSpecular �� ���Եȴ�.
		//AddLingt.fx
		ComputeLight(
			addDiffuse,
			addSpecular,
			Input.worldPos,
			worldNormal,
			viewDir,
			i);

		//���� ���� ���� ���δ�.
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


		//��ĺ�ȯ�� ��ģ �� z �� ��ĺ�ȯ���� ��� ����ġ w �� ������ 0 ~ 1 ������ ���� ���� �ȴ�.
		float depth = Input.FinalPos.z / Input.FinalPos.w;

	//���� depth ���� ī�޶��� near �� far �� �̿��Ͽ� �������� ���ش�....
	//Perspective Projection Linear Depth
	float z = depth;
	float a = camFar / (camFar - camNear);
	float b = -camNear / (camFar - camNear);
	depth = b / (z - a);


	Output.baseColor = float4(finalColor, 1);
	Output.normalDepth = float4(worldNormal, depth);		//alpha ���� ������ ���.


	return Output;
}




//---------------------------------------------------------------
// CreateShadow ����
//---------------------------------------------------------------

struct PS_INPUT_SHADOW
{
	float4 FinalPos : TEXCOORD0;
	float2 Texcoord : TEXCOORD1;
};

float4 ps_CreateShadow(PS_INPUT_SHADOW Input) : COLOR0
{
	//��ĺ�ȯ�� ��ģ �� z �� ��ĺ�ȯ���� ��� ����ġ w �� ������ 0 ~ 1 ������ ���� ���� �ȴ�.
	float depth = Input.FinalPos.z / Input.FinalPos.w;

	float4 diffTex = tex2D(Diffuse, Input.Texcoord);
		clip(diffTex.a - 0.1f);

	return float4(depth.xxx, 1);
}


//--------------------------------------------------------------//
// Render With ShadowMap 
//--------------------------------------------------------------//


float4x4 matLightViewProjection;			//���⼺ ���� ViewProjection ���

//������ Texture
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
	float4 LightClipPos : TEXCOORD7;		//���� ���忡�� �ٶ� ��ġ
};


PS_OUTPUT ps_ReciveShadow(PS_INPUT_RECIVESHADOW Input)
{
	PS_OUTPUT Output = (PS_OUTPUT)0;

	float4 diffTex = tex2D(Diffuse, Input.Texcoord);
		clip(diffTex.a - 0.1f);

	//���� ���忡�� �ٶ� ��ġ�� ���� �� ( ����Ʈ ����� �����̱⶧���� �������� ���ɴ� )
	float lightDepth = Input.LightClipPos.z / Input.LightClipPos.w;

	//Shadow ���� UV ����
	//( -1 ~ 1 )
	float2 shadowUV = Input.LightClipPos.xy / Input.LightClipPos.w;
		shadowUV.y = -shadowUV.y;		//y����...
	//0 ~ 1 �� ����
	shadowUV = (shadowUV * 0.5f) + 0.5f;

	//���� �׷����� ���� ��ġ�� DirectionLight ���忡�� ���� ���̰�...
	float shadowDepth = tex2D(ShadowSampler, shadowUV).r;

	//�׸��ڰ� �׷����� ��Ȳ�� shadowDepth + bias �� ���� lightDepth �� ū����̴�.




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

		//���� ��
		float3 finalDiffuse = float3(0, 0, 0);
		float3 finalSpecular = float3(0, 0, 0);

		//�⺻ ������ ó��

		//������ ����
		float3 dir = float3(baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23);
		float3 lightDir = -dir;

		//������ �÷�
		float3 lightColor = float3(baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33) * baseDirectionLight._34;


		float NdotL = dot(lightDir, worldNormal);

	//Ambient
	//float diff = saturate( NdotL );

	//float diff = max( 0.1f, NdotL );

	float diff = NdotL;

	//�׸��ڰ� �׷����� �Ѵٸ� diff �� ������ ������ �ȴ� 
	if (shadowDepth + bias < lightDepth && lightDepth < 1.0f)
		diff = abs(diff) * -1.0f;

	if (diff < 0.0)
		diff = abs(diff) * 0.3f;

	//����Ʈ �ݻ�
	float3 lightRefl = normalize(dir + 2.0f * NdotL * worldNormal);
		float spec = saturate(dot(lightRefl, viewDir));
	spec = pow(spec, fSpecPower);

	//�⺻ ���� ����
	finalDiffuse = lightColor * diff;
	finalSpecular = lightColor * spec * diff;		//Specular �� diff �Ȱ��ϸ� Specular �� ���� ������ ���´�...


	//�߰��Ǵ� ����
	float3 addDiffuse = float3(0, 0, 0);
		float3 addSpecular = float3(0, 0, 0);
	for (int i = 0; i < LightNum; i++)
	{
		//i �ε����� ������ ���Ǿ� addDiffuse, addSpecular �� ���Եȴ�.
		ComputeLight(
			addDiffuse,
			addSpecular,
			Input.worldPos,
			worldNormal,
			viewDir,
			i);

		//���� ���� ���� ���δ�.
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



		//��ĺ�ȯ�� ��ģ �� z �� ��ĺ�ȯ���� ��� ����ġ w �� ������ 0 ~ 1 ������ ���� ���� �ȴ�.
		float depth = Input.FinalPos.z / Input.FinalPos.w;

	//���� depth ���� ī�޶��� near �� far �� �̿��Ͽ� �������� ���ش�....
	//Perspective Projection Linear Depth
	float z = depth;
	float a = camFar / (camFar - camNear);
	float b = -camNear / (camFar - camNear);
	depth = b / (z - a);


	Output.baseColor = float4(finalColor, 1);
	Output.normalDepth = float4(worldNormal, depth);		//alpha ���� ������ ���.

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

	//������ ����
	float3 dir = float3(baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23);
	float3 lightDir = -dir;

	//������ �÷�
	float3 lightColor = float3(baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33) * baseDirectionLight._34;


	float diff = dot(worldNormal, lightDir);
	diff = (diff + 1.0f) * 0.5f;

	float toon = 0.0f;

	//������ ���뷮�� �ܰ� ���� ������.
	float interval = 1.0 / 4.0f;		//5�ܰ�
	toon = floor(diff / interval) * interval;		//floor �� �Ҽ����糤��..
	toon = max(0.3f, toon);



	//���� ��
	float3 finalDiffuse = lightColor * toon;

		//�߰��Ǵ� ����
		float3 addDiffuse = float3(0, 0, 0);
	for (int i = 0; i < LightNum; i++)
	{
		//i �ε����� ������ ���Ǿ� addDiffuse, addSpecular �� ���Եȴ�.
		ComputeLightToon(
			addDiffuse,
			Input.worldPos,
			worldNormal,
			i);

		//���� ���� ���� ���δ�.
		finalDiffuse += addDiffuse;
	}


	//RampTex �� �����´�.
	//float3 rampRgb = tex2D( Ramp, float2( diff, 0 ) ).rgb;
	//float3 diffuseColor = tex2D( Diffuse, Input.Texcoord ).rgb * rampRgb;

	float3 finalColor = diffTex.rgb * finalDiffuse;



		//��ĺ�ȯ�� ��ģ �� z �� ��ĺ�ȯ���� ��� ����ġ w �� ������ 0 ~ 1 ������ ���� ���� �ȴ�.
		float depth = Input.FinalPos.z / Input.FinalPos.w;

	//���� depth ���� ī�޶��� near �� far �� �̿��Ͽ� �������� ���ش�....
	//Perspective Projection Linear Depth
	float z = depth;
	float a = camFar / (camFar - camNear);
	float b = -camNear / (camFar - camNear);
	depth = b / (z - a);


	Output.baseColor = float4(finalColor, 1);
	Output.normalDepth = float4(worldNormal, depth);		//alpha ���� ������ ���.

	return Output;
}


//--------------------------------------------------------------//
// Render Distort
//--------------------------------------------------------------//

//�������� �׷��� ȭ���� ����
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

	//���� ���� �׷����� ��ġ�� ȭ�� ��ǥ�� �÷��� �׷�����...
	float2 screenUV = Input.FinalPos.xy / Input.FinalPos.w;

	//-1 �� 1 ������ ���� 0 ~ 1 ������ ������..
	screenUV = (screenUV + 1.0f) * 0.5f;

	//Y �� ����
	screenUV.y = 1.0f - screenUV.y;
	
	float2 texUV = Input.Texcoord;
	texUV.y += scrollU;

	//�븻�� ��´�.
	float2 norXY = tex2D(Normal, texUV).xy;
	//�븻���� -1 ~ 1 ���̷�..
	norXY = (norXY * 2.0f) - 1.0f;

	//�븻 ����ŭ UV �ܰ�
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

	//���� ���忡�� �ٶ� ��ġ�� ���� �� ( ����Ʈ ����� �����̱⶧���� �������� ���ɴ� )
	float lightDepth = Input.LightClipPos.z / Input.LightClipPos.w;

	//Shadow ���� UV ����
	//( -1 ~ 1 )
	float2 shadowUV = Input.LightClipPos.xy / Input.LightClipPos.w;
		shadowUV.y = -shadowUV.y;		//y����...
	//0 ~ 1 �� ����
	shadowUV = (shadowUV * 0.5f) + 0.5f;

	//���� �׷����� ���� ��ġ�� DirectionLight ���忡�� ���� ���̰�...
	float shadowDepth = tex2D(ShadowSampler, shadowUV).r;

	//�׸��ڰ� �׷����� ��Ȳ�� shadowDepth + bias �� ���� lightDepth �� ū����̴�.


	float3 worldNormal = normalize(Input.Normal);

		//������ ����
		float3 dir = float3(baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23);
		float3 lightDir = -dir;

		//������ �÷�
		float3 lightColor = float3(baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33) * baseDirectionLight._34;


		float diff = dot(worldNormal, lightDir);
	diff = (diff + 1.0f) * 0.5f;

	float toon = 0.0f;
	//�׸��ڰ� �׷����� �Ѵٸ� diff �� 0.3f �� �ȴ� 
	if (shadowDepth + bias < lightDepth && lightDepth < 1.0f)
		toon = 0.3f;

	else
	{
		//������ ���뷮�� �ܰ� ���� ������.
		float interval = 1.0 / 4.0f;		//5�ܰ�
		toon = floor(diff / interval) * interval;		//floor �� �Ҽ����糤��..
		toon = max(0.3f, toon);

	}



	//���� ��
	float3 finalDiffuse = lightColor * toon;

		//�߰��Ǵ� ����
		float3 addDiffuse = float3(0, 0, 0);
	for (int i = 0; i < LightNum; i++)
	{
		//i �ε����� ������ ���Ǿ� addDiffuse, addSpecular �� ���Եȴ�.
		ComputeLightToon(
			addDiffuse,
			Input.worldPos,
			worldNormal,
			i);

		//���� ���� ���� ���δ�.
		finalDiffuse += addDiffuse;
	}


	//RampTex �� �����´�.
	//float3 rampRgb = tex2D( Ramp, float2( diff, 0 ) ).rgb;
	//float3 diffuseColor = tex2D( Diffuse, Input.Texcoord ).rgb * rampRgb;

	float3 finalColor = diffTex.rgb * finalDiffuse;



	//��ĺ�ȯ�� ��ģ �� z �� ��ĺ�ȯ���� ��� ����ġ w �� ������ 0 ~ 1 ������ ���� ���� �ȴ�.
	float depth = Input.FinalPos.z / Input.FinalPos.w;

	//���� depth ���� ī�޶��� near �� far �� �̿��Ͽ� �������� ���ش�....
	//Perspective Projection Linear Depth
	float z = depth;
	float a = camFar / (camFar - camNear);
	float b = -camNear / (camFar - camNear);
	depth = b / (z - a);

	Output.baseColor = float4(finalColor, 1);
	Output.normalDepth = float4(worldNormal, depth);		//alpha ���� ������ ���.

	return Output;
}
