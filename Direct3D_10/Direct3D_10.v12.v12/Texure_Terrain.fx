//
// 전역 변수
//
float4x4	matWVP;			//월드 뷰 투영 3단합체 행렬
float		tiling0;		//0번 타일 타일수
float		tiling1;		//1번 타일 타일수
float		tiling2;		//2번 타일 타일수
float		tiling3;		//3번 타일 타일수


texture TexControl;
sampler2D SampControl = sampler_state
{
	Texture = TexControl;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};

texture TexTile0;
sampler2D SampTile0 = sampler_state
{
	Texture = TexTile0;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};


texture TexTile1;
sampler2D SampTile1 = sampler_state
{
	Texture = TexTile1;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};

texture TexTile2;
sampler2D SampTile2 = sampler_state
{
	Texture = TexTile2;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};


texture TexTile3;
sampler2D SampTile3 = sampler_state
{
	Texture = TexTile3;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};


//
// 정점에서 넘겨받을 데이터 구조체
//
struct VS_INPUT
{
	float3 Position : POSITION;			//정점위치
	float2 Texcoord : TEXCOORD0;		//정점UV
};


//
// 정점셰이터에서 픽섹셰이더로 넘겨주는 정보의 구조체 
//
struct VS_OUTPUT
{
	float4 Position : POSITION;		//예는 동차영역 변환정보로 무조건 넘겨야 한다. ( Pixel 함수까지 안간다 )
	float2 Texcoord : TEXCOORD0;	//정점의 UV 좌표
};


//
// 정점 셰이더 함수
//

//정점에서 받을 정보가 매개변수로 들어온다
VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	//정점 합체 변환
	Output.Position = mul(float4(Input.Position, 1.0f), matWVP);

	//UV 대입
	Output.Texcoord = Input.Texcoord;

	//리턴 
	return Output;
}

//
// 픽셀 셰이더 함수
//
//정점셰이더에서 리턴된 정보가 매개변수로 들어온다
//이때 픽셀단위로 보간되어 들어오고 
//POSITION 값은 빠져서오게 된다.
float4 ps_main(VS_OUTPUT In) : COLOR0
{
	//각 타일의 색상을 뽑는다.
	float3 tileColor0 = tex2D(SampTile0, In.Texcoord * tiling0).rgb;
	float3 tileColor1 = tex2D(SampTile1, In.Texcoord * tiling1).rgb;
	float3 tileColor2 = tex2D(SampTile2, In.Texcoord * tiling2).rgb;
	float3 tileColor3 = tex2D(SampTile3, In.Texcoord * tiling3).rgb;

	//ControlTexture 컬러
	float3 maskColor = tex2D(SampControl, In.Texcoord);

	//각컬러 량이 어떻게 되니?
	float r = maskColor.r;
	float g = maskColor.g;
	float b = maskColor.b;

	//black 의 비율이 얼만큼 쎄니?
	float black = saturate(1.0f - (r + g + b));

	float3 finalColor = tileColor0 * black + tileColor1 * r + tileColor2 * g + tileColor3 * b;

		return float4(finalColor, 1);
}


//
// 테크닉
//
technique MyShader
{
	pass P0
	{
		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
	}
}

