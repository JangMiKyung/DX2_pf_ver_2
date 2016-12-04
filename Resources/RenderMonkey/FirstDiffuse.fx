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
string Default_DirectX_Effect_Pass_0_Model : ModelData = "C:\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Hebe.3ds";

float4x4 matWorld;
float4x4 matView;
float4x4 matProjection;

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float3 Normal : NORMAL0;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float3 Normal : TEXCOORD0;
};

VS_OUTPUT Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output = (VS_OUTPUT)0;
   
   Output.Position = mul( Input.Position, matWorld );
   Output.Position = mul( Output.Position, matView );
   Output.Position = mul( Output.Position, matProjection );
   
   Output.Normal = mul( Input.Normal, (float3x3)matWorld );
   
   return Output;
}




float3 fLightDir
<
   string UIName = "fLightDir";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float3( -1.00, 0.00, 0.00 );

struct PS_INPUT 
{
   float3 Normal : TEXCOORD0;
};


float4 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main( PS_INPUT Input ) : COLOR0
{   
   float3 worldNormal = normalize( Input.Normal );
   float diffuse = saturate( dot( -fLightDir, worldNormal ) );
   float3 color = float3( 1.0f, 1.0f, 0 ) * diffuse;
   
   
   return float4( color, 1.0f );
   
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

