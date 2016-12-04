float4x4 matWorld : World;
float4x4 matViewProjection : ViewProjection;

float camNear;			//카메라 근거리 평면
float camFar;			//카메라 원거리 평면




//-----------------------------------------------------------------------------
// Base
//-----------------------------------------------------------------------------

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float3 Normal : NORMAL0;
   float3 BiNormal : BINORMAL0;
   float3 Tangent : TANGENT0;
   float2 BaseUV : TEXCOORD0;
   float2 TileUV : TEXCOORD1;
   
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float2 TileUV : TEXCOORD0;
   float2 ControlUV : TEXCOORD1;
   float3 Normal : TEXCOORD2;
   float4 FinalPos : TEXCOORD3;
};

VS_OUTPUT vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output = (VS_OUTPUT)0;

   Output.Position = mul( Input.Position, matWorld );
   Output.Position = mul( Output.Position, matViewProjection );
  
   Output.TileUV = Input.TileUV;
   Output.ControlUV = Input.BaseUV;
     
   Output.Normal = mul( Input.Normal, (float3x3)matWorld );
  
   Output.FinalPos = Output.Position;
   return( Output );
}



struct PS_INPUT 
{
   float2 TileUV : TEXCOORD0;
   float2 ControlUV : TEXCOORD1;
   float3 Normal : TEXCOORD2;
   float4 FinalPos : TEXCOORD3;
};

//픽셀셰이더 출력 구조체
struct PS_OUTPUT
{
	float4 baseColor : COLOR0;			//0번 스테이지 컬러
	float4 normalDepth : COLOR1;		//1번 스테이지 컬러 ( RGB 노말, A 뎁스 )
};


texture Terrain0_Tex;
sampler2D Terrain0 = sampler_state
{
   Texture = (Terrain0_Tex);
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};
texture Terrain1_Tex;
sampler2D Terrain1 = sampler_state
{
   Texture = (Terrain1_Tex);
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};
texture Terrain2_Tex;
sampler2D Terrain2 = sampler_state
{
   Texture = (Terrain2_Tex);
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
   MAGFILTER = LINEAR;
};
texture Terrain3_Tex;
sampler2D Terrain3 = sampler_state
{
   Texture = (Terrain3_Tex);
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
   MAXANISOTROPY = 0;
};

texture TerrainControl_Tex;
sampler2D TerrainControl = sampler_state
{
   Texture = (TerrainControl_Tex);
   MAGFILTER = ANISOTROPIC;
   MAXANISOTROPY = 16;
   MINFILTER = ANISOTROPIC;
   MIPFILTER = ANISOTROPIC;
};



float4 worldLightDir
<
   string UIName = "worldLightDir";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 1.00, -1.00, 1.60, 1.00 );

float4 worldLightColor
<
   string UIName = "worldLightColor";
   string UIWidget = "Color";
   bool UIVisible =  true;
> = float4( 1.00, 1.00, 1.00, 1.00 );


PS_OUTPUT ps_main( PS_INPUT Input )
{   
	PS_OUTPUT  Output = (PS_OUTPUT)0;

   // Terrain Tile 컬러를 얻는다.
   float3 terrain0 = tex2D( Terrain0, Input.TileUV ).rgb;
   float3 terrain1 = tex2D( Terrain1, Input.TileUV ).rgb;
   float3 terrain2 = tex2D( Terrain2, Input.TileUV ).rgb;
   float3 terrain3 = tex2D( Terrain3, Input.TileUV ).rgb;
  
   //Control Texture 의 컬러를 얻는다.
   float3 controlColor = tex2D( TerrainControl, Input.ControlUV ).rgb;
   

   //컨트롤 Texture 에서 색상비율을 얻는다.
   float redFactor = controlColor.r;
   float greenFactor = controlColor.g;
   float blueFactor = controlColor.b;
   float blackFactor = saturate( 1.0f - ( redFactor +  greenFactor + blueFactor ) );

   float3 texColor1 = terrain0 * redFactor;
   float3 texColor2 = terrain1 * greenFactor;
   float3 texColor3 = terrain2 * blueFactor;
   float3 texColor4 = terrain3 * blackFactor;
   
   float3 finalColor = texColor1 + texColor2 + texColor3 + texColor4;

   float3 worldNormal = normalize(Input.Normal);
   float diff = dot(worldNormal, normalize(-worldLightDir.xyz));
   //if( diff < 0.0f )
   //  diff = saturate( abs( diff ) * 0.3f + 0.1f );
   diff = saturate( diff );
   



   //행렬변환을 거친 값 z 에 행렬변환에서 얻는 가중치 w 를 나누면 0 ~ 1 사이의 깊이 값이 된다.
   float depth = Input.FinalPos.z / Input.FinalPos.w;

   //위의 depth 값을 카메라의 near 와 far 를 이용하여 선형으로 펴준다....
   //Perspective Projection Linear Depth
   float z = depth;
   float a = camFar / (camFar - camNear);
   float b = -camNear / (camFar - camNear);
   depth = b / (z - a);


   Output.baseColor = float4(finalColor * diff, 1);
   Output.normalDepth = float4(worldNormal, depth);		//alpha 값에 뎁스를 썼다.

   return Output;

}



//---------------------------------------------------------------
// CreateShadow 관련
//---------------------------------------------------------------

struct VS_INPUT_DEPTH
{
   float4 Position : POSITION0;
};

struct VS_OUTPUT_DEPTH
{
   float4 Position : POSITION0;
   float4 FinalPos : TEXCOORD0;
};


VS_OUTPUT_DEPTH vs_depth( VS_INPUT_DEPTH Input )
{
   VS_OUTPUT_DEPTH Output = (VS_OUTPUT_DEPTH)0;

   float4 worldPos = mul( Input.Position, matWorld );
   Output.Position = mul( worldPos, matViewProjection );
   
   Output.FinalPos = Output.Position;

   return( Output );
}

float4 ps_CreateShadow( VS_OUTPUT_DEPTH Input ) : COLOR0
{   
	//행렬변환을 거친 값 z 에 행렬변환에서 얻는 가중치 w 를 나누면 0 ~ 1 사이의 깊이 값이 된다.
	float depth = Input.FinalPos.z / Input.FinalPos.w;

    //return float4( depth, depth, depth, 1 );

	return float4( depth.xxx, 1 );
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


struct VS_INPUT_RECIVESHADOW
{
   float4 Position : POSITION0;
   float3 Normal : NORMAL0;
   float3 Binormal : BINORMAL0;
   float3 Tangent : TANGENT0;

   float2 BaseUV : TEXCOORD0;
   float2 TileUV : TEXCOORD1;
};

struct VS_OUTPUT_RECIVESHADOW
{
   float4 Position : POSITION0;
   float2 TileUV : TEXCOORD0;
   float2 ControlUV : TEXCOORD1;
   float3 Normal : TEXCOORD2;

   float4 FinalPos : TEXCOORD6;
   float4 LightClipPos : TEXCOORD7;		//광원 입장에서 바라본 위치
};

VS_OUTPUT_RECIVESHADOW vs_ReciveShadow( VS_INPUT_RECIVESHADOW Input )
{
   VS_OUTPUT_RECIVESHADOW Output = (VS_OUTPUT_RECIVESHADOW)0;

   float4 worldPos = mul( Input.Position, matWorld );
   Output.Position = mul( worldPos, matViewProjection );
  
   Output.TileUV = Input.TileUV;
   Output.ControlUV = Input.BaseUV;
     
   Output.Normal = mul( Input.Normal, (float3x3)matWorld );

   Output.FinalPos = Output.Position;
   Output.LightClipPos = mul( worldPos, matLightViewProjection );

  
   return( Output );
}



PS_OUTPUT ps_ReciveShadow( VS_OUTPUT_RECIVESHADOW Input )
{   
	PS_OUTPUT Output = (PS_OUTPUT)0;


	//광원 입장에서 바라본 위치의 뎁스 값 ( 라이트 행렬을 직교이기때문에 선형으로 안핀다 )
	float lightDepth = Input.LightClipPos.z / Input.LightClipPos.w;

	//Shadow 맵의 UV 추적
	//( -1 ~ 1 )
	float2 shadowUV = Input.LightClipPos.xy / Input.LightClipPos.w;
	shadowUV.y = -shadowUV.y;		//y반전...
	//0 ~ 1 의 범위
	shadowUV = ( shadowUV * 0.5f ) + 0.5f;	

	//현제 그려지는 놈이 위치에 DirectionLight 입장에서 써진 깊이값...
	float shadowDepth = tex2D( ShadowSampler, shadowUV ).r;

	//그림자가 그려지는 상황은 shadowDepth + bias 값 보다 lightDepth 가 큰경우이다.

   // Terrain Tile 컬러를 얻는다.
   float3 terrain0 = tex2D( Terrain0, Input.TileUV ).rgb;
   float3 terrain1 = tex2D( Terrain1, Input.TileUV ).rgb;
   float3 terrain2 = tex2D( Terrain2, Input.TileUV ).rgb;
   float3 terrain3 = tex2D( Terrain3, Input.TileUV ).rgb;
  
   //Control Texture 의 컬러를 얻는다.
   float3 controlColor = tex2D( TerrainControl, Input.ControlUV ).rgb;
   

   //컨트롤 Texture 에서 색상비율을 얻는다.
   float redFactor = controlColor.r;
   float greenFactor = controlColor.g;
   float blueFactor = controlColor.b;
   float blackFactor = saturate( 1.0f - ( redFactor +  greenFactor + blueFactor ) );

   float3 texColor1 = terrain0 * redFactor;
   float3 texColor2 = terrain1 * greenFactor;
   float3 texColor3 = terrain2 * blueFactor;
   float3 texColor4 = terrain3 * blackFactor;
   
   float3 finalColor = texColor1 + texColor2 + texColor3 + texColor4;
   float3 worldNormal = normalize(Input.Normal);
   float diff = dot(worldNormal, normalize(-worldLightDir.xyz));

   //그림자가 그려져야 한다면 diff 는 무조건 음수가 된다 
   if( shadowDepth + bias < lightDepth && lightDepth < 1.0f )
	diff = abs( diff ) * -1.0f;

   diff = max( 0.3f, diff );		//Ambient 는 0.3f
   

   //행렬변환을 거친 값 z 에 행렬변환에서 얻는 가중치 w 를 나누면 0 ~ 1 사이의 깊이 값이 된다.
   float depth = Input.FinalPos.z / Input.FinalPos.w;

   //위의 depth 값을 카메라의 near 와 far 를 이용하여 선형으로 펴준다....
   //Perspective Projection Linear Depth
   float z = depth;
   float a = camFar / (camFar - camNear);
   float b = -camNear / (camFar - camNear);
   depth = b / (z - a);


   Output.baseColor = float4(finalColor * diff, 1);
   Output.normalDepth = float4(worldNormal, depth);		//alpha 값에 뎁스를 썼다.

   return Output;
}




technique Base
{
   pass Pass_0
   {
      VertexShader = compile vs_3_0 vs_main();
      PixelShader = compile ps_3_0 ps_main();
   }
}


technique CreateShadow
{
   pass Pass_0
   {
      VertexShader = compile vs_3_0 vs_depth();
      PixelShader = compile ps_3_0 ps_CreateShadow();
   }
}



technique ReciveShadow
{
   pass Pass_0
   {
      VertexShader = compile vs_3_0 vs_ReciveShadow();
      PixelShader = compile ps_3_0 ps_ReciveShadow();
   }
}
