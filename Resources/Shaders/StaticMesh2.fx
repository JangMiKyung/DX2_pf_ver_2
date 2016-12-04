
float4x4 matWorld : World;
float4x4 matViewProjection : ViewProjection;
float4 vEyePos : ViewPosition;


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
};

VS_OUTPUT vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output = (VS_OUTPUT)0;

   float4 worldPos = mul( Input.Position, matWorld );
   Output.Position = mul( worldPos, matViewProjection );
   
   Output.Texcoord = Input.Texcoord;
   
   Output.Normal = mul( Input.Normal, (float3x3)matWorld );
   Output.Binormal = mul( Input.Binormal, (float3x3)matWorld );  
   Output.Tangent = mul( Input.Tangent, (float3x3)matWorld ); 
   
   Output.viewDir = vEyePos.xyz - worldPos.xyz;
   Output.worldPos = worldPos.xyz;

   return( Output );
}




struct PS_INPUT
{
   float2 Texcoord : TEXCOORD0;
   float3 Normal : TEXCOORD1;
   float3 Binormal : TEXCOORD2;
   float3 Tangent : TEXCOORD3;
   float3 viewDir : TEXCOORD4;
   float3 worldPos : TEXCOORD5;
};

texture Diffuse_Tex;
sampler2D Diffuse = sampler_state
{
   Texture = (Diffuse_Tex);
   MAGFILTER = ANISOTROPIC;
   MIPFILTER = ANISOTROPIC;
   MINFILTER = ANISOTROPIC;
};
texture Normal_Tex;
sampler2D Normal = sampler_state
{
   Texture = (Normal_Tex);
   MAGFILTER = ANISOTROPIC;
   MINFILTER = ANISOTROPIC;
   MIPFILTER = ANISOTROPIC;
};

texture Specular_Tex;
sampler2D Specular = sampler_state
{
   Texture = (Specular_Tex);
   MAGFILTER = ANISOTROPIC;
   MINFILTER = ANISOTROPIC;
   MIPFILTER = ANISOTROPIC;
};


texture Emission_Tex;
sampler2D Emission = sampler_state
{
   Texture = (Emission_Tex);
   MAGFILTER = ANISOTROPIC;
   MIPFILTER = ANISOTROPIC;
   MINFILTER = ANISOTROPIC;
};

float fSpecPower;

// 라이트 행렬
// Pos.x,         Pos.y,          Pos.z,         lightType ( 0 Direction, 1 Point, 2 Spot )
// Dir.x,         Dir.y,          Dir.z,         DistancePow;
// Color.r,       Color.g,        Color.b,       strength
// MaxRange,      MinRange,       InAngle        OutAngle 
float4x4 matLights[10];
int LightNum = 0;


void ComputeLight( 
		out float3 addDiffuse,			//계산되서 나가는 DeffuseColor
		out float3 addSpecular,			//계산되서 나가는 SpecularColor
		float3 position,				//위치 값
		float3 normal,					//노말
		float3 viewDir,					//시선 방향
		int lightIndex					//처리되는 라이트 인덱스
		)
{
	float4x4 matLight = matLights[ lightIndex ];

	float3 lightColor = float3( matLight._31, matLight._32, matLight._33 ) * matLight._34;

	//Direction 처리
	if( matLight._14 == 0.0f )
	{
		//광원의 방향
		float3 dir = float3( matLight._21, matLight._22, matLight._23 );
		float3 lightDir = -dir;

		float NdotL = dot( lightDir, normal );
		float diff = saturate( NdotL );

		//라이트 반사
		float3 lightRefl = normalize( dir + 2.0f * NdotL * normal );
		float spec = saturate( dot( lightRefl, viewDir ) );
		spec = pow( spec, fSpecPower );
		
		addDiffuse = lightColor * diff;
		addSpecular =  lightColor * spec * diff;
	}

	//Point Light and Spot 처리
	else
	{
		float3 lightPosition = float3( matLight._11, matLight._12, matLight._13 );
		float maxRange = matLight._41;
		float minRange = matLight._42;
		float DistancePow = matLight._24;

		//광원의 방향
		float3 lightDir = lightPosition - position;

		//광원의 거리
		float dist = length( lightDir );
		
		//정규화
		lightDir *= ( 1.0f / dist );

		//광원 량....
		float NdotL = dot( lightDir, normal );
		float diff = saturate( NdotL );

		//거리에 따른 감쇠율
		float t = saturate( ( dist - minRange ) / ( maxRange - minRange ) );
		t = 1.0f - pow( t, DistancePow );
		
		//만약 Spot 이라면....

		

		diff *= t;			//받는 광원 량에 감쇠율 적용

		//라이트 반사
		float3 lightRefl = normalize( -lightDir + 2.0f * NdotL * normal );
		float spec = saturate( dot( lightRefl, viewDir ) );
		spec = pow( spec, fSpecPower );
		
		addDiffuse = lightColor * diff;
		addSpecular =  lightColor * spec * diff;		//Spec 은 Diff 의 영향을 받는다.


	}

	


}


float4 ps_main( PS_INPUT Input ) : COLOR0
{   
     //TBN Matrix
     float3x3 TBN = float3x3(
        normalize( Input.Tangent ),
        normalize( Input.Binormal ),
        normalize( Input.Normal ) );
     
	 //   
     // WorldNormal
     //
     float3 norColor = tex2D( Normal, Input.Texcoord ).rgb;
     
	 //Tangent Space Normal
     float3 spaceNor = ( norColor * 2.0f ) - 1.0f;

     float3 worldNormal = mul( spaceNor, TBN );
	 worldNormal = normalize(worldNormal);

     float3 viewDir = normalize( Input.viewDir );
	 float4x4 baseDirectionLight = matLights[0];
  
	 //최종 색
	 float3 finalDiffuse = float3( 0, 0, 0 );
	 float3 finalSpecular = float3( 0, 0, 0 );

	 ///////////////

	 float3 dir = float3(baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23);
	 float3 lightColor = float3(baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33) * baseDirectionLight._34;
	 float NdotL = dot(-dir, worldNormal);
	 float diff = saturate(NdotL);
	 float fIntensity = baseDirectionLight._33;

	/* if (shadowDepth + bias < lightDepth && lightDepth < 1.0f)
		 diff = 0;*/

	 diff = max(diff, fIntensity * 0.3f);

	 // 기본 광원 적용
	 finalDiffuse = lightColor * diff;
	 ////////////////


	 //추가되는 색
	 float3 addDiffuse = float3( 0, 0, 0 );
	 float3 addSpecular = float3( 0, 0, 0 );
	 
	 for( int i = 0 ; i < LightNum ; i++ )
	 {
		//i 인덱스의 광원이 계산되어 addDiffuse, addSpecular 에 대입된다.
		ComputeLight( 
			addDiffuse, 
			addSpecular,
			Input.worldPos, 
			worldNormal,
			viewDir,
			i );

		//최종 광원 량에 쌓인다.
		finalDiffuse += addDiffuse;
		finalSpecular += addSpecular;
	 }


	 // Diffuse Texture Color
	 float4 diffuseTexColor = tex2D(Diffuse, Input.Texcoord).rgba;

	 clip(diffuseTexColor.a - 0.5f);
     //
     // Diffuse
     //
     //float3 diffuseColor = tex2D( Diffuse, Input.Texcoord ).rgb * finalDiffuse;

	// Diffuse
	float4 diffuseColor = diffuseTexColor* float4(finalDiffuse, 1);

     //
     // Specular 
     //
	 float3 specularColor = tex2D(Diffuse, Input.Texcoord).rgb * finalSpecular * 2.0f;
     
     //
     // Emission
     //
     float3 emissionColor = tex2D( Emission, Input.Texcoord ).rgb;
		

	 // Ambient
	 float3 ambientColor = diffuseTexColor.rgb * 0.2f * fIntensity;

     //
     // Final Color 
     //
    // float3 finalColor = diffuseColor + specularColor + emissionColor;
	float4 finalColor = diffuseColor + float4(specularColor, 1);

	finalColor = max(finalColor, float4(ambientColor, 1));

	return finalColor;
   
}


//--------------------------------------------------------------//
// Technique Section for Mesh
//--------------------------------------------------------------//
technique Mesh
{
   pass Pass_0
   {
      VertexShader = compile vs_3_0 vs_main();
      PixelShader = compile ps_3_0 ps_main();
   }
}

technique Base
{
   pass Pass_0
   {
      VertexShader = compile vs_3_0 vs_main();
      PixelShader = compile ps_3_0 ps_main();
   }
}

technique ReceiveShadow
{
	pass Pass_0
	{
		CULLMODE = NONE;

		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
	}
}
