#include "ModelPixel.fx"

//------------------------------------------------------------------------------------
// SkinnedMesh Vertex Processing
//------------------------------------------------------------------------------------

#ifndef MATRIX_PALETTE_SIZE_DEFAULT
#define MATRIX_PALETTE_SIZE_DEFAULT 35
#endif

const int MATRIX_PALETTE_SIZE = MATRIX_PALETTE_SIZE_DEFAULT;
float4x3 amPalette[ MATRIX_PALETTE_SIZE_DEFAULT ];

struct VS_SKIN_INPUT
{
    float4      Position;
    float3      BlendWeights;
    int4        BlendIndices;
	float3		Normal;
	float3		Binormal;
	float3		Tangent;
};

struct VS_SKIN_OUTPUT
{
    float4		Position;			//���� ��ȯ �� ����
	float3		Normal;				//�븻 ��ȯ ��
	float3		Binormal;			//���̳븻 ��ȯ ��
	float3		Tangent;			//ź��Ʈ ��ȯ ��
};

VS_SKIN_OUTPUT VS_Skin(  VS_SKIN_INPUT Input, int iNumBones )
{
    VS_SKIN_OUTPUT Output = (VS_SKIN_OUTPUT) 0;

    float fLastWeight = 1.0;
    float fWeight = 0.0f;
    float afBlendWeights[ 3 ] = (float[ 3 ])Input.BlendWeights;
    //int aiIndices[ 4 ] = (int[ 4 ]) D3DCOLORtoUBYTE4( Input.BlendIndices );
    int aiIndices[ 4 ] = (int[4] )Input.BlendIndices;

    for( int iBone = 0; (iBone < 3) && (iBone < iNumBones - 1); ++ iBone )
    {
        fWeight = afBlendWeights[ iBone ];
        fLastWeight -= fWeight;
        Output.Position.xyz += mul( Input.Position, amPalette[ aiIndices[ iBone ] ] ) * fWeight;

		Output.Normal     += mul( Input.Normal, amPalette[ aiIndices[ iBone ] ] ) * fWeight;
		Output.Binormal   += mul( Input.Binormal, amPalette[ aiIndices[ iBone ] ] ) * fWeight;
		Output.Tangent    += mul( Input.Tangent, amPalette[ aiIndices[ iBone ] ] ) * fWeight;
    }
    
    Output.Position.xyz += mul( Input.Position, amPalette[ aiIndices[ iNumBones - 1 ] ] ) * fLastWeight;
	Output.Normal		+= mul( Input.Normal, amPalette[ aiIndices[ iNumBones - 1 ] ] ) * fWeight;
	Output.Binormal		+= mul( Input.Binormal, amPalette[ aiIndices[ iNumBones - 1 ] ] ) * fWeight;
	Output.Tangent		+= mul( Input.Tangent, amPalette[ aiIndices[ iNumBones - 1 ] ] ) * fWeight;

    return Output;
}


struct VS_SKIN_INPUT2
{
    float4      Position;
    float3      BlendWeights;
    int4        BlendIndices;
};

struct VS_SKIN_OUTPUT2
{
    float4		Position;			//���� ��ȯ �� ����
};

VS_SKIN_OUTPUT2 VS_Skin2(  VS_SKIN_INPUT2 Input, int iNumBones )
{
    VS_SKIN_OUTPUT2 Output = (VS_SKIN_OUTPUT2) 0;

    float fLastWeight = 1.0;
    float fWeight = 0.0f;
    float afBlendWeights[ 3 ] = (float[ 3 ])Input.BlendWeights;
    //int aiIndices[ 4 ] = (int[ 4 ]) D3DCOLORtoUBYTE4( Input.BlendIndices );
    int aiIndices[ 4 ] = (int[4] )Input.BlendIndices;

    for( int iBone = 0; (iBone < 3) && (iBone < iNumBones - 1); ++ iBone )
    {
        fWeight = afBlendWeights[ iBone ];
        fLastWeight -= fWeight;
        Output.Position.xyz += mul( Input.Position, amPalette[ aiIndices[ iBone ] ] ) * fWeight;
    }
    
    Output.Position.xyz += mul( Input.Position, amPalette[ aiIndices[ iNumBones - 1 ] ] ) * fLastWeight;

    return Output;
}


struct VS_SKIN_INPUT3
{
    float4      Position;
    float3      BlendWeights;
    int4        BlendIndices;
	float3		Normal;
};

struct VS_SKIN_OUTPUT3
{
    float4		Position;			//���� ��ȯ �� ����
	float3		Normal;				//�븻 ��ȯ ��
};

VS_SKIN_OUTPUT3 VS_Skin3( VS_SKIN_INPUT3 Input, int iNumBones )
{
    VS_SKIN_OUTPUT3 Output = (VS_SKIN_OUTPUT3) 0;

    float fLastWeight = 1.0;
    float fWeight = 0.0f;
    float afBlendWeights[ 3 ] = (float[ 3 ])Input.BlendWeights;
    //int aiIndices[ 4 ] = (int[ 4 ]) D3DCOLORtoUBYTE4( Input.BlendIndices );
    int aiIndices[ 4 ] = (int[4] )Input.BlendIndices;

    for( int iBone = 0; (iBone < 3) && (iBone < iNumBones - 1); ++ iBone )
    {
        fWeight = afBlendWeights[ iBone ];
        fLastWeight -= fWeight;
        Output.Position.xyz += mul( Input.Position, amPalette[ aiIndices[ iBone ] ] ) * fWeight;

		Output.Normal     += mul( Input.Normal, amPalette[ aiIndices[ iBone ] ] ) * fWeight;
    }
    
    Output.Position.xyz += mul( Input.Position, amPalette[ aiIndices[ iNumBones - 1 ] ] ) * fLastWeight;
	Output.Normal		+= mul( Input.Normal, amPalette[ aiIndices[ iNumBones - 1 ] ] ) * fWeight;

    return Output;
}







//------------------------------------------------------------------------------------
// SkinnedMesh Base 
//------------------------------------------------------------------------------------

float4x4 matWorld : World;
float4x4 matViewProjection : ViewProjection;
float4 vEyePos;


struct VS_INPUT
{
    float4  Position        : POSITION;
    float3  BlendWeights    : BLENDWEIGHT;
    float4  BlendIndices    : BLENDINDICES;
    float3  Normal          : NORMAL;
    float3  Binormal        : Binormal;
    float3  Tangent         : TANGENT;
    float2  Texcoord        : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4  Position		: POSITION;
    float2  Texcoord        : TEXCOORD0;
	float3  Normal          : TEXCOORD1;
	float3  Binormal        : TEXCOORD2;
	float3  Tangent         : TEXCOORD3;
	float3  viewDir			: TEXCOORD4;
	float3  worldPos		: TEXCOORD5;
	float4 FinalPos : TEXCOORD6;
};

//���ý� ����
VS_OUTPUT VertSkinning( VS_INPUT Input, uniform int iNumBones )
{
    VS_OUTPUT Output;

    VS_SKIN_INPUT vsi = { Input.Position, Input.BlendWeights, Input.BlendIndices, Input.Normal, Input.Binormal, Input.Tangent };
    VS_SKIN_OUTPUT vso = VS_Skin( vsi, iNumBones );

	//���� ���� ������ ���� �Ϸ�.....
	float4 worldPos = float4( vso.Position.xyz, 1.0f );
	
	//Input �� Tangent �� Binormal �� Normal �� 
	//VS_SKIN_OUTPUT ����ü vso �� �������� ��ü �ȴ�.
	/*
	struct VS_SKIN_OUTPUT
	{
	    float4		Position;			//���� ��ȯ �� ����
		float3		Normal;				//�븻 ��ȯ ��
		float3		Binormal;			//���̳븻 ��ȯ ��
		float3		Tangent;			//ź��Ʈ ��ȯ ��
	};
	*/


	Output.worldPos = worldPos.xyz;
    Output.Position = mul( worldPos, matViewProjection );

	Output.viewDir = vEyePos.xyz - Output.Position.xyz;

    Output.Normal = mul( vso.Normal, (float3x3)matWorld );
    Output.Binormal = mul( vso.Binormal, (float3x3)matWorld );
    Output.Tangent = mul( vso.Tangent, (float3x3)matWorld );
   
    Output.Texcoord = Input.Texcoord;

	Output.FinalPos = Output.Position;

    return Output;
}

//------------------------------------------------------------------------------------
// SkinnedMesh CreateShadow 
//------------------------------------------------------------------------------------

struct VS_INPUT_DEPTH
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   float3  BlendWeights    : BLENDWEIGHT;
   float4  BlendIndices    : BLENDINDICES;
};

struct VS_OUTPUT_DEPTH
{
   float4 Position : POSITION0;
   float4 FinalPos : TEXCOORD0;
   float2 Texcoord : TEXCOORD1;
};

VS_OUTPUT_DEPTH vs_CreateShadow( VS_INPUT_DEPTH Input, uniform int iNumBones )
{
   VS_OUTPUT_DEPTH Output = (VS_OUTPUT_DEPTH)0;

   VS_SKIN_INPUT2 vsi = { Input.Position, Input.BlendWeights, Input.BlendIndices };
   VS_SKIN_OUTPUT2 vso = VS_Skin2( vsi, iNumBones );


   //���� ���� ������ ���� �Ϸ�.....
   float4 worldPos = float4( vso.Position.xyz, 1.0f );

   Output.Position = mul( worldPos, matViewProjection );
   
   Output.FinalPos = Output.Position;

   Output.Texcoord = Input.Texcoord;

   return( Output );
}


//--------------------------------------------------------------//
// Render With ShadowMap 
//--------------------------------------------------------------//
struct VS_INPUT_RECIVESHADOW
{
   float4 Position : POSITION0;
   float3  BlendWeights    : BLENDWEIGHT;
   float4  BlendIndices    : BLENDINDICES;
   float2 Texcoord : TEXCOORD0;
   float3 Normal : NORMAL0;
   float3 Binormal : BINORMAL0;
   float3 Tangent : TANGENT0;
};

struct VS_OUTPUT_RECIVESHADOW
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   float3 Normal : TEXCOORD1;
   float3 Binormal : TEXCOORD2;
   float3 Tangent : TEXCOORD3;
   float3 viewDir : TEXCOORD4;
   float3 worldPos : TEXCOORD5;

   float4 FinalPos : TEXCOORD6;
   float4 LightClipPos : TEXCOORD7;		//���� ���忡�� �ٶ� ��ġ
};


VS_OUTPUT_RECIVESHADOW vs_ReciveShadow( VS_INPUT_RECIVESHADOW Input, uniform int iNumBones  )
{
   VS_OUTPUT_RECIVESHADOW Output = (VS_OUTPUT_RECIVESHADOW)0;


   VS_SKIN_INPUT vsi = { Input.Position, Input.BlendWeights, Input.BlendIndices, Input.Normal, Input.Binormal, Input.Tangent };
   VS_SKIN_OUTPUT vso = VS_Skin( vsi, iNumBones );

   //���� ���� ������ ���� �Ϸ�.....
   float4 worldPos = float4( vso.Position.xyz, 1.0f );
   
   Output.Position = mul( worldPos, matViewProjection );
   
   Output.Texcoord = Input.Texcoord;
   
   Output.Normal = mul( Input.Normal, (float3x3)matWorld );
   Output.Binormal = mul( Input.Binormal, (float3x3)matWorld );  
   Output.Tangent = mul( Input.Tangent, (float3x3)matWorld ); 
   
   Output.viewDir = vEyePos.xyz - worldPos.xyz;
   Output.worldPos = worldPos;


   Output.FinalPos = Output.Position;		//��ȯ ����
   Output.LightClipPos = mul( worldPos, matLightViewProjection );	//���� ���忡�� �� ��ġ

   return( Output );
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


int CurNumBones = 1;

VertexShader vsArray20[ 4 ] = { compile vs_3_0 VertSkinning( 1 ),
                                compile vs_3_0 VertSkinning( 2 ),
                                compile vs_3_0 VertSkinning( 3 ),
                                compile vs_3_0 VertSkinning( 4 ) };


VertexShader CreateShadowArr[ 4 ] = { compile vs_3_0 vs_CreateShadow( 1 ),
									  compile vs_3_0 vs_CreateShadow( 2 ),
									  compile vs_3_0 vs_CreateShadow( 3 ),
								      compile vs_3_0 vs_CreateShadow( 4 ) };


VertexShader ReciveShadowArr[ 4 ] = { compile vs_3_0 vs_ReciveShadow( 1 ),
									  compile vs_3_0 vs_ReciveShadow( 2 ),
									  compile vs_3_0 vs_ReciveShadow( 3 ),
								      compile vs_3_0 vs_ReciveShadow( 4 ) };


//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------

technique Base
{
    pass p0
    {
        VertexShader = ( vsArray20[ CurNumBones ] );
		PixelShader = compile ps_3_0 ps_main();
    }
}

technique Toon
{
	pass p0
	{
		VertexShader = (vsArray20[CurNumBones]);
		PixelShader = compile ps_3_0 ps_Toon();
	}
}



technique CreateShadow
{
    pass p0
    {
        VertexShader = ( CreateShadowArr[ CurNumBones ] );
        PixelShader = compile ps_3_0 ps_CreateShadow();
    }
}


technique ReciveShadow
{
    pass p0
    {
        VertexShader = ( ReciveShadowArr[ CurNumBones ] );
        PixelShader = compile ps_3_0 ps_ReciveShadow();
    }
}





technique ReciveShadowToon
{
	pass Pass_0
	{
		VertexShader = (ReciveShadowArr[CurNumBones]);
		PixelShader = compile ps_3_0 ps_ReciveShadowToon();
	}
}
