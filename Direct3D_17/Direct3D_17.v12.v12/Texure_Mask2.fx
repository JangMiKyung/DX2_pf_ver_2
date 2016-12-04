//
// 전역 변수
//
float4x4	matWVP;			//월드 뷰 투영 3단합체 행렬
float4		vColor1;			//출력 컬러
float4		vColor2;			//출력 컬러
float4		vColor3;			//출력 컬러
float		tilingX;		//가로 타일수
float		tilingY;		//세로 타일수

//
// Texture 를 받는다.
//
texture MyTex;

//
// MyTex 의 SamplingFilter 를 설정
//
sampler2D MyTexSamp = sampler_state
{
	Texture = MyTex;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};

//
// Texture 를 받는다.
//
texture MaskTex;

//
// MyTex 의 SamplingFilter 를 설정
//
sampler2D MyMaskSamp = sampler_state
{
	Texture = MaskTex;
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
	Output.Position = mul(float4(Input.Position, 1.0), matWVP);

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
	//Texture UV 를 컬러로...
	//return float4( In.Texcoord.x, In.Texcoord.y, 0, 1 );

	//UV 좌표 정보로 Sampler2D 에서 컬러를 뽑자
	//해당UV 에 Sampling 된 컬러 = tex2D( Sampler2D 변수, float2 UV 정보 )
	//float4 texColor = tex2D( MyTexSamp, In.Texcoord );

	//sampling 된 UV 값에 타일 정보를 곱한다.
	float2 uv = In.Texcoord;
	uv.x *= tilingX;
	uv.y *= tilingY;

	//셰이더에선 실수끼리의 나머지 연산이 된다.
	uv.x = uv.x % 1.0f;
	uv.y = uv.y % 1.0f;

	//Texture 컬러
	float4 texColor = tex2D(MyTexSamp, uv);

		//MaksTexture 컬러
		float3 maskColor = tex2D(MyMaskSamp, uv);


		//각컬러 량이 어떻게 되니?
		float r = maskColor.r;
	float g = maskColor.g;
	float b = maskColor.b;

	//black 의 비율이 얼만큼 쎄니?
	float black = saturate(1.0f - (r + g + b));

	//각색을 비율로 곱한다.
	float3 color1 = texColor * vColor1 * r;
		float3 color2 = texColor * vColor2 * g;
		float3 color3 = texColor * vColor3 * b;
		float3 color4 = texColor * black;

		float3 finalColor = color1 + color2 + color3 + color4;





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
