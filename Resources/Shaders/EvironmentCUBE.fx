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
string Default_DirectX_Effect_Pass_0_Model : ModelData = "C:\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Sphere.x";

float4x4 matWVP : WorldViewProjection;
float4x4 matWorld : World;

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
   VS_OUTPUT Output;

   Output.Position = mul( Input.Position, matWVP );
   
   //WorldNormal
   Output.Normal = mul( Input.Normal, (float3x3)matWorld );
   //Output.Normal = Input.Normal;



   //LocalPosition To CubeTexUV
   //Output.Normal = Input.Position;
   
   
   return( Output );
   
}




struct PS_INPUT 
{
   float3 Normal : TEXCOORD0;
};

texture MyCube_Tex
<
   string ResourceName = "C:\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\grace_cube.dds";
>;
samplerCUBE MyCubeSamp = sampler_state
{
   Texture = (MyCube_Tex);
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};


float4 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main( PS_INPUT Input ) : COLOR0
{   
   return texCUBE( MyCubeSamp, normalize( Input.Normal ) );
   
}




//--------------------------------------------------------------//
// Technique Section for Default_DirectX_Effect
//--------------------------------------------------------------//
technique Default_DirectX_Effect
{
   pass Pass_0
   {
      CULLMODE = CW;
      ZWRITEENABLE = FALSE;

      VertexShader = compile vs_3_0 Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_3_0 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main();
   }

}

