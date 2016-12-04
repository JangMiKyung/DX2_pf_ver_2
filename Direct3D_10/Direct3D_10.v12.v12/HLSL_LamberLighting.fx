//
// 전역 변수
//
float4x4	matWorld;		//월드 행렬
float4x4	matView;		//뷰 행렬
float4x4	matProjection;	//투영 행렬
float4		vColor;			//출력 컬러
float3		vLightDir;		//광원의 방향 ( 정규화 되어 들어왔다 )
float4x4	matInvWorld;	//월드의 역행렬

//
// 정점셰이터에서 픽섹셰이더로 넘겨주는 정보의 구조체 
//
struct VS_OUTPUT
{
	float4 Position : POSITION;		//예는 동차영역 변환정보로 무조건 넘겨야 한다. ( Pixel 함수까지 안간다 )
	float4 Color : COLOR0;
};


//
// 정점 셰이더 함수
//

//정점에서 받을 정보가 매개변수로 들어온다
VS_OUTPUT vs_main(float3 Position : POSITION, float3 Normal : NORMAL)
{
	VS_OUTPUT Output;

	//정점 월드 변환
	Output.Position = mul(float4(Position, 1.0), matWorld);

	//정점 뷰 변환
	Output.Position = mul(Output.Position, matView);

	//정점 투영 변환
	Output.Position = mul(Output.Position, matProjection);

	//월드 노말
	//float3 worldNormal = mul( Normal, (float3x3)matWorld );

	//월드 노말 정규화
	//worldNormal = normalize( worldNormal );

	//자신의 위치로 부터 광원의 방향
	//float3 toLight = -vLightDir;

	//램버트
	//float diffuse = dot( toLight, worldNormal );


	//로컬 노말
	float3 localNormal = Normal;

		//월드 광원의 방향을 로컬로 땡긴다
		float3 localLightDir = mul(vLightDir, (float3x3)matInvWorld);

		//로컬 광원 방향 정규화
		localLightDir = normalize(localLightDir);

	float3 toLocalLight = -localLightDir;

		//램버트 내적
		float diffuse = dot(toLocalLight, localNormal);

	//0~1 사이로 짤라주는 함수 Clamp01 과 같은기능
	diffuse = saturate(diffuse);

	//컬러 대입
	Output.Color = vColor * diffuse;


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
	//그냥 아무짓하지말고 In 에 들어온 컬러 리턴
	return In.Color;
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

