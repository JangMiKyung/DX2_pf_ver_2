//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// Default_DirectX_Effect
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string Default_DirectX_Effect_Pass_0_Model : ModelData = "C:\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\ninjaHead.obj";

float4x4 matWorld : World;
float4x4 matView : View;
float4x4 matProjection : Projection;

float4 fEyePos : ViewPosition;

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float3 Normal : NORMAL0;
   float2 Texcoord : TEXCOORD0;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float3 Normal : TEXCOORD0;
   float2 Texcoord : TEXCOORD1;
   float3 ViewDir : TEXCOORD2;
};

VS_OUTPUT Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output = (VS_OUTPUT)0;
   
   float4 worldPos = mul( Input.Position, matWorld );
   Output.Position = mul( worldPos, matView );
   Output.Position = mul( Output.Position, matProjection );
   
   Output.Normal = mul( Input.Normal, (float3x3)matWorld );
   
   Output.Texcoord = Input.Texcoord;
   
   Output.ViewDir = fEyePos.xyz - worldPos.xyz;
   
   return Output;
}




float3 fLightDir
<
   string UIName = "fLightDir";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float3( -1.00, -0.86, -0.12 );
texture MyTex_Tex
<
   string ResourceName = "..\\Testures\\Sample1.png";
>;
sampler2D MySamp = sampler_state
{
   Texture = (MyTex_Tex);
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
   MAGFILTER = LINEAR;
};

float fSpecPower
<
   string UIName = "fSpecPower";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float( 10.00 );


struct PS_INPUT 
{
   float3 Normal : TEXCOORD0;
   float2 Texcoord : TEXCOORD1;
   float3 ViewDir : TEXCOORD2;
};


float4 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main( PS_INPUT Input ) : COLOR0
{   
   float3 worldNormal = normalize( Input.Normal );
   float3 viewDir = normalize( Input.ViewDir );
   
   float lightDot =  dot( normalize(-fLightDir), worldNormal );
   float diffuse = saturate(lightDot);
   
   float3 lightRefl = fLightDir + worldNormal * 2 * lightDot;
   lightRefl = normalize( lightRefl );
   
   
   float spec = dot( lightRefl, viewDir );
   spec = pow( spec, fSpecPower );
   
   float3 texColor = tex2D( MySamp, Input.Texcoord ).rgb;
   
   float3 finalColor = ( texColor * diffuse ) + float3( 1, 1, 1 ) * spec;
   
   
   return float4( finalColor, 1.0f );
   
}




//--------------------------------------------------------------//
// Technique Section for Default_DirectX_Effect
//--------------------------------------------------------------//
technique Default_DirectX_Effect
{
   pass Pass_0
   {
      VertexShader = compile vs_3_0 Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_3_0 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main();
   }

}

