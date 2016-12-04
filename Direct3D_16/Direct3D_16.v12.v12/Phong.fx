//
// 전역 변수
//
float4x4	matWorld;		//월드 행렬
float4x4	matView;		//뷰 행렬
float4x4	matProjection;	//투영 행렬

float4		vMaterialDiffuseColor;			//표면 Diffuse 출력 컬러
float4		vMaterialSpecularColor;			//표면 Specular 출력 컬러


float3		vLightDir;		//광원의 방향 ( 정규화 되어 들어왔다 )
float3		vEyePos;		//눈의 위치
float		fPower;			//정반사 날카롭기 ( 이게 크면 날카로워 짐 )

//
// 정점에서 넘겨받을 데이터 구조체
//
struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
};



//
// 정점셰이터에서 픽섹셰이더로 넘겨주는 정보의 구조체 
//
struct VS_OUTPUT
{
	float4 Position : POSITION;		//예는 동차영역 변환정보로 무조건 넘겨야 한다. ( Pixel 함수까지 안간다 )
	float3 Normal : TEXCOORD0;		//월드 노말
	float3 EyeDir : TEXCOORD1;		//눈의 방향
};


//
// 정점 셰이더 함수
//

//정점에서 받을 정보가 매개변수로 들어온다
VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	//정점 월드 위치
	float4 worldPos = mul(float4(Input.Position, 1.0), matWorld);

		//정점 뷰 변환
		Output.Position = mul(worldPos, matView);

	//정점 투영 변환
	Output.Position = mul(Output.Position, matProjection);

	//월드 노말
	float3 worldNormal = mul(Input.Normal, (float3x3)matWorld);
		worldNormal = normalize(worldNormal);

	//출력에 노말 전달
	Output.Normal = worldNormal;

	//눈의 방향을 얻자
	float3 eyeDir = vEyePos - worldPos.xyz; //float4 인 worldPos 을 worldPos.xyz 만쓰면 xyz 순서로 각성분에대한 float3 로본다 ( 스위즐링 문법 )
		eyeDir = normalize(eyeDir);

	//눈의 방향 전달
	Output.EyeDir = eyeDir;


	//리턴 
	return Output;
}


//
// 픽셀 셰이더 함수
//
//정점셰이더에서 리턴된 정보가 매개변수로 들어온다
//이때 픽셀단위로 보간되어 들어오고 
//POSITION 값은 빠져서오게 된다.
float4 ps_main(VS_OUTPUT Input) : COLOR0
{
	//정확한 계산을 위해 보간되 들어온방향값들을 정규화 해서쓰자.
	float3 normal = normalize(Input.Normal);
	float3 eyedir = normalize(Input.EyeDir);

	//월드 광월 방향
	float3 toLight = -vLightDir;

	//Diffuse 광산광의 량
	float diffuse = saturate(dot(toLight, normal));


	// LightReflection 계산
	float3 inLight = -toLight;
		float3 lightReflect = inLight + 2 * (dot(-inLight, normal) * normal);
		lightReflect = normalize(lightReflect);

	//
	// Specular 계산
	//	
	float spec = saturate(dot(lightReflect, eyedir));

	//Power 증가
	spec = pow(spec, fPower);

	//spec *= diffuse;

	//
	// 최종 컬러 계산
	//

	float4 finalColor =
		(vMaterialDiffuseColor * diffuse) +		//Diffuse 량 추가
		(vMaterialSpecularColor * spec);

	return finalColor;
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

