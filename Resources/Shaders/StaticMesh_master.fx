float4x4 matWorld : World;
float4x4 matViewProjection : ViewProjection;
float4 vEyePos : ViewPosition;
float vTerrainNormalDot = 1;
float4x4 matLightViewProjection;
float bias = 0.002f;

//
// Vertex Shader
//

struct VS_INPUT
{
	float4 Position : POSITION0;
	float2 Texcoord : TEXCOORD0;
	float3 Normal : NORMAL0;
	float3 Binormal : BINORMAL0;
	float3 Tangent : TANGENT0;
};

struct VS_OUTPUT
{
	float4 Position : POSITION0;
	float2 Texcoord : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 Binormal : TEXCOORD2;
	float3 Tangent : TEXCOORD3;
	float3 viewDir : TEXCOORD4;
	float3 worldPos : TEXCOORD5;
	float4 FinalPos : TEXCOORD6;
	float4 LightClipPos : TEXCOORD7;
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;

	float4 worldPos = mul(Input.Position, matWorld);
	Output.Position = mul(worldPos, matViewProjection);

	Output.Texcoord = Input.Texcoord;

	Output.Normal = mul(Input.Normal, (float3x3)matWorld);
	Output.Binormal = mul(Input.Binormal, (float3x3)matWorld);
	Output.Tangent = mul(Input.Tangent, (float3x3)matWorld);

	Output.viewDir = vEyePos.xyz - worldPos.xyz;
	Output.worldPos = worldPos.xyz;

	Output.FinalPos = Output.Position;
	Output.LightClipPos = mul(worldPos, matLightViewProjection);

	return Output;
}


//
// Pixel Shader
//

struct PS_INPUT
{
	float2 Texcoord : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 Binormal : TEXCOORD2;
	float3 Tangent : TEXCOORD3;
	float3 viewDir : TEXCOORD4;
	float3 worldPos : TEXCOORD5;
	float4 FinalPos : TEXCOORD6;
	float4 LightClipPos : TEXCOORD7;
};

texture Diffuse_Tex;
sampler2D DiffuseSample = sampler_state
{
	Texture = Diffuse_Tex;
	MAGFILTER = ANISOTROPIC;
	MIPFILTER = ANISOTROPIC;
	MINFILTER = ANISOTROPIC;
};

texture Normal_Tex;
sampler2D NormalSample = sampler_state
{
	Texture = Normal_Tex;
	MAGFILTER = ANISOTROPIC;
	MINFILTER = ANISOTROPIC;
	MIPFILTER = ANISOTROPIC;
};

texture Specular_Tex;
sampler2D SpecularSample = sampler_state
{
	Texture = Specular_Tex;
	MAGFILTER = ANISOTROPIC;
	MINFILTER = ANISOTROPIC;
	MIPFILTER = ANISOTROPIC;
};


texture Emission_Tex;
sampler2D EmissionSample = sampler_state
{
	Texture = Emission_Tex;
	MAGFILTER = ANISOTROPIC;
	MIPFILTER = ANISOTROPIC;
	MINFILTER = ANISOTROPIC;
};

texture Shadow_Tex;
sampler2D ShadowSampler = sampler_state
{
	Texture = Shadow_Tex;
	ADDRESSU = BORDER;
	ADDRESSV = BORDER;
	BORDERCOLOR = 0xFFFFFFFF;
};

float fSpecPower;
float4x4 matLights[10];
int LightNum;

// Pos.x			Pos.y				Pos.z			lightType ( 0 Direction, 1 Point, 2 Spot )
// Dir.x			Dir.y				Dir.z			DistancePow
// Color.r			Color.g			Color.b		strength
// MaxRange		MinRange		InAngle		OutAngle

void ComputeLight(out float3 addDiffuse, out float3 addSpecular, float3 position, float3 normal, float3 viewDir, int lightIndex)
{
	float4x4 matLight = matLights[lightIndex];

	float3 lightColor = float3(matLight._31, matLight._32, matLight._33) * matLight._34;

	// Directional Light
	if (matLight._14 == 0.0f)
	{
		// Light Direction
		float3 dir = float3(matLight._21, matLight._22, matLight._23);
		float NdotL = dot(-dir, normal);
		float diff = saturate(NdotL);

		// Light Reflection
		float3 lightRefl = normalize(dir + 2.0f * NdotL * normal);
		float spec = saturate(dot(lightRefl, viewDir));
		spec = pow(spec, fSpecPower);

		addDiffuse = lightColor * diff;
		addSpecular = lightColor * spec;
	}
	// Point Light and Spot Light
	else
	{
		float3 lightPosition = float3(matLight._11, matLight._12, matLight._13);
		float maxRange = matLight._41;
		float minRange = matLight._42;
		float DistancePow = matLight._24;

		float3 lightDir = lightPosition - position;

		float dist = length(lightDir);

		// Normalize
		lightDir *= (1.0f / dist);

		float NdotL = dot(lightDir, normal);
		float diff = saturate(NdotL);

		float t = saturate((dist - minRange) / (maxRange - minRange));
		t = 1.0f - pow(t, DistancePow);

		float tSpot = 1.0f;

		// Spot Light
		if (matLight._14 == 2.0f)
		{
			float inAngleCos = cos(matLight._43);
			float outAngleCos = cos(matLight._44);

			float3 dir = normalize(float3(matLight._21, matLight._22, matLight._23));
			float dotLightPixel = dot(dir, -lightDir);

			tSpot = saturate((inAngleCos - dotLightPixel) / (inAngleCos - outAngleCos));
			tSpot = 1.0f - pow(tSpot, DistancePow);
		}

		diff = diff * t * tSpot;

		float3 lightRefl = normalize(-lightDir + 2.0f * NdotL * normal);
		float spec = saturate(dot(lightRefl, viewDir));
		spec = pow(spec, fSpecPower);

		addDiffuse = lightColor * diff;
		addSpecular = lightColor * spec * diff;
	}
}

float4 ps_main(PS_INPUT Input) : COLOR0
{
	float lightDepth = Input.LightClipPos.z / Input.LightClipPos.w;

	float2 shadowUV = Input.LightClipPos.xy / Input.LightClipPos.w;
	shadowUV.y = -shadowUV.y;
	shadowUV = (shadowUV * 0.5f) + 0.5f;

	float shadowDepth = tex2D(ShadowSampler, shadowUV).r;

	// TBN Matrix
	float3x3 TBN = float3x3(
	normalize(Input.Tangent),
	normalize(Input.Binormal),
	normalize(Input.Normal));

	// WorldNormal
	float3 norColor = tex2D(NormalSample, Input.Texcoord).rgb;

	// Tangent Space Normal
	float3 spaceNor = (norColor * 2.0f) - 1.0f;

	float3 worldNormal = mul(spaceNor, TBN);
	worldNormal = normalize(worldNormal);
	float3 viewDir = normalize(Input.viewDir);

	float3 finalDiffuse = float3(0, 0, 0);
	float3 finalSpecular = float3(0, 0, 0);

	float4x4 baseDirectionLight = matLights[0];

	float3 dir = float3(baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23);
	float3 lightColor = float3(baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33) * baseDirectionLight._34;
	float NdotL = dot(-dir, worldNormal);
	float diff = saturate(NdotL);
	float fIntensity = baseDirectionLight._33;

	if (shadowDepth + bias < lightDepth && lightDepth < 1.0f)
		diff = 0;

	diff = max(diff, fIntensity * 0.3f);

	// 기본 광원 적용
	finalDiffuse = lightColor * diff;

	float3 addDiffuse = float3(0, 0, 0);
	float3 addSpecular = float3(0, 0, 0);

	for (int i = 0; i < LightNum; i++)
	{
		ComputeLight(addDiffuse, addSpecular, Input.worldPos, worldNormal, viewDir, i);
		
		finalDiffuse += addDiffuse;
		finalSpecular += addSpecular;

	}
	
	// Diffuse Texture Color
	float4 diffuseTexColor = tex2D(DiffuseSample, Input.Texcoord).rgba;

	clip(diffuseTexColor.a - 0.5f);

	// Diffuse
	float4 diffuseColor = diffuseTexColor* float4(finalDiffuse, 1);

	// Specular 
	//
	float3 specularColor = tex2D(Specular, Input.Texcoord).rgb * finalSpecular;


	// Ambient
	float3 ambientColor = diffuseTexColor.rgb * 0.2f * fIntensity;

	// 지형 노말 적용
	//float3 terrainNormalColor = diffuseTexColor.rgb * vTerrainNormalDot * fIntensity;

	// Final Color
	float4 finalColor = diffuseColor /*+ float4(terrainNormalColor, 0)*/;

	finalColor = max(finalColor, float4(ambientColor, 1));

	return finalColor;
}

//
// technique
//

technique ReceiveShadow
{
	pass Pass_0
	{
		CULLMODE = NONE;

		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
	}
}
