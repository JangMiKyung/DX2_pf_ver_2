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
string Default_DirectX_Effect_Pass_0_Model : ModelData = "C:\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\ScreenAlignedQuad.3ds";

float4x4 matWorld : World;
float4x4 matViewProj : ViewProjection;

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   float4 ClipPos : TEXCOORD1; 
};

VS_OUTPUT Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output;

   Output.Position = mul( Input.Position, matWorld );
   Output.Position = mul( Output.Position, matViewProj );
   
   Output.ClipPos = Output.Position;
   
   Output.Texcoord = Input.Texcoord;
   
   return( Output );
   
}





struct PS_INPUT
{
   float2 Texcoord : TEXCOORD0;
   float4 ClipPos : TEXCOORD1;
};

texture grabTexture_Tex
<
   string ResourceName = "..\\..\\Resource\\Textures\\terrain4.png";
>;
sampler2D grabSampler = sampler_state
{
   Texture = (grabTexture_Tex);
};
texture distortNormal_Tex
<
   string ResourceName = "..\\..\\Resource\\Textures\\WaterNormal.jpg";
>;
sampler2D distortSampler = sampler_state
{
   Texture = (distortNormal_Tex);
};

float DistortPower
<
   string UIName = "DistortPower";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 0.00;
> = float( 0.09 );
float CamNear : NearClipPlane;
float CamFar : FarClipPlane;


float4 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main( PS_INPUT Input ) : COLOR0
{   
   //? ? ( -1 ~ 1, -1 ~ 1, 0 ~ 1 )
   float3 pos = Input.ClipPos.xyz / Input.ClipPos.w;

   float depth = pos.z;
   
   //¤|  <\ @?
   float a = CamFar / ( CamFar - CamNear );
   float b = -CamNear / ( CamFar - CamNear );
   depth = b / ( pos.z - a );


   //( -1 ~ 1 )
   float2 screenPos = pos.xy;
  
   //( 0 ~ 1 )
   screenPos = ( screenPos + 1.0f ) * 0.5f;
   screenPos.y = 1.0f - screenPos.y; 
   
  
  
   //( 0 ~ 1 )
   float3 distortColor = tex2D( distortSampler, Input.Texcoord );
   
   //x?)?( -1 ~ 1 )
   float2 distorDir = ( distortColor.xy * 2.0f ) - 1.0f;  
  
   screenPos += -distorDir * DistortPower * ( 1.0f - depth );
  
   
   //\?μμ?TtX| UV \ X?grabSampler μμ?
   float4 finalColor = tex2D( grabSampler, screenPos );

   return finalColor;
  
   //return float4( Input.Normal, 1.0f );
   
   //return float4( depth, depth, depth, 1.0f );

}




//--------------------------------------------------------------//
// Technique Section for Default_DirectX_Effect
//--------------------------------------------------------------//
technique Default_DirectX_Effect
{
   pass Pass_0
   {
      CULLMODE = NONE;
      ZENABLE = FALSE;

      VertexShader = compile vs_2_0 Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main();
   }

}

