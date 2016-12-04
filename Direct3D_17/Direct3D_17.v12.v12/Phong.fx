//
// ���� ����
//
float4x4	matWorld;		//���� ���
float4x4	matView;		//�� ���
float4x4	matProjection;	//���� ���

float4		vMaterialDiffuseColor;			//ǥ�� Diffuse ��� �÷�
float4		vMaterialSpecularColor;			//ǥ�� Specular ��� �÷�


float3		vLightDir;		//������ ���� ( ����ȭ �Ǿ� ���Դ� )
float3		vEyePos;		//���� ��ġ
float		fPower;			//���ݻ� ��ī�ӱ� ( �̰� ũ�� ��ī�ο� �� )

//
// �������� �Ѱܹ��� ������ ����ü
//
struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Normal : NORMAL;
};



//
// ���������Ϳ��� �ȼ����̴��� �Ѱ��ִ� ������ ����ü 
//
struct VS_OUTPUT
{
	float4 Position : POSITION;		//���� �������� ��ȯ������ ������ �Ѱܾ� �Ѵ�. ( Pixel �Լ����� �Ȱ��� )
	float3 Normal : TEXCOORD0;		//���� �븻
	float3 EyeDir : TEXCOORD1;		//���� ����
};


//
// ���� ���̴� �Լ�
//

//�������� ���� ������ �Ű������� ���´�
VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	//���� ���� ��ġ
	float4 worldPos = mul(float4(Input.Position, 1.0), matWorld);

		//���� �� ��ȯ
		Output.Position = mul(worldPos, matView);

	//���� ���� ��ȯ
	Output.Position = mul(Output.Position, matProjection);

	//���� �븻
	float3 worldNormal = mul(Input.Normal, (float3x3)matWorld);
		worldNormal = normalize(worldNormal);

	//��¿� �븻 ����
	Output.Normal = worldNormal;

	//���� ������ ����
	float3 eyeDir = vEyePos - worldPos.xyz; //float4 �� worldPos �� worldPos.xyz ������ xyz ������ �����п����� float3 �κ��� ( ������ ���� )
		eyeDir = normalize(eyeDir);

	//���� ���� ����
	Output.EyeDir = eyeDir;


	//���� 
	return Output;
}


//
// �ȼ� ���̴� �Լ�
//
//�������̴����� ���ϵ� ������ �Ű������� ���´�
//�̶� �ȼ������� �����Ǿ� ������ 
//POSITION ���� ���������� �ȴ�.
float4 ps_main(VS_OUTPUT Input) : COLOR0
{
	//��Ȯ�� ����� ���� ������ ���¹��Ⱚ���� ����ȭ �ؼ�����.
	float3 normal = normalize(Input.Normal);
	float3 eyedir = normalize(Input.EyeDir);

	//���� ���� ����
	float3 toLight = -vLightDir;

	//Diffuse ���걤�� ��
	float diffuse = saturate(dot(toLight, normal));


	// LightReflection ���
	float3 inLight = -toLight;
		float3 lightReflect = inLight + 2 * (dot(-inLight, normal) * normal);
		lightReflect = normalize(lightReflect);

	//
	// Specular ���
	//	
	float spec = saturate(dot(lightReflect, eyedir));

	//Power ����
	spec = pow(spec, fPower);

	//spec *= diffuse;

	//
	// ���� �÷� ���
	//

	float4 finalColor =
		(vMaterialDiffuseColor * diffuse) +		//Diffuse �� �߰�
		(vMaterialSpecularColor * spec);

	return finalColor;
}


//
// ��ũ��
//
technique MyShader
{
	pass P0
	{
		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
	}
}

