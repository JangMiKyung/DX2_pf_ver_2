//
// ���� ����
//
float4x4	matWorld;		//���� ���
float4x4	matView;		//�� ���
float4x4	matProjection;	//���� ���

float4		vColor;			//��� �÷�
float3		vLightDir;		//������ ���� ( ����ȭ �Ǿ� ���Դ� )
float3		vEyePos;		//���� ��ġ


//
// ���������Ϳ��� �ȼ����̴��� �Ѱ��ִ� ������ ����ü 
//
struct VS_OUTPUT
{
	float4 Position : POSITION;		//���� �������� ��ȯ������ ������ �Ѱܾ� �Ѵ�. ( Pixel �Լ����� �Ȱ��� )
	float4 Color : COLOR0;
};


//
// ���� ���̴� �Լ�
//

//�������� ���� ������ �Ű������� ���´�
VS_OUTPUT vs_main(float3 Position : POSITION, float3 Normal : NORMAL)
{
	VS_OUTPUT Output;

	//���� ���� ��ġ
	float4 worldPos = mul(float4(Position, 1.0), matWorld);

		//���� �� ��ȯ
		Output.Position = mul(worldPos, matView);

	//���� ���� ��ȯ
	Output.Position = mul(Output.Position, matProjection);



	//���� �븻
	float3 worldNormal = mul(Normal, (float3x3)matWorld);

		//���� �븻 ����ȭ
		worldNormal = normalize(worldNormal);




	//���� ������ ����
	float3 eyeDir = vEyePos - worldPos.xyz; //float4 �� worldPos �� worldPos.xyz ������ xyz ������ �����п����� float3 �κ��� ( ������ ���� )
		eyeDir = normalize(eyeDir);

	//�������� ���� ���� ����
	float3 toLightDir = normalize(-vLightDir);

		//������ ���� ����
		float3 inLightDir = -toLightDir;

		//���� �ݻ������ ����.
		float3 lightReflect = inLightDir + 2 * (dot(worldNormal, -inLightDir) * worldNormal);

		//�ݻ� ����
		lightReflect = normalize(lightReflect);

	//���� �ݻ�� ����� ���� ������ �������� �۰� ���ü��� ���ݻ� ���� ���ϴ�.
	float spec = saturate(dot(eyeDir, lightReflect));

	//���ݻ緮�� ��ī�Ӱ� ����
	spec = pow(spec, 10);




	//����Ʈ
	float diffuse = dot(toLightDir, worldNormal);

	//0~1 ���̷� ©���ִ� �Լ� Clamp01 �� �������
	diffuse = saturate(diffuse);

	//�÷� ����
	Output.Color = vColor * diffuse + float4(1, 1, 1, 1) * spec;


	//���� 
	return Output;
}

//
// �ȼ� ���̴� �Լ�
//
//�������̴����� ���ϵ� ������ �Ű������� ���´�
//�̶� �ȼ������� �����Ǿ� ������ 
//POSITION ���� ���������� �ȴ�.
float4 ps_main(VS_OUTPUT In) : COLOR0
{
	//�׳� �ƹ����������� In �� ���� �÷� ����
	return In.Color;
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

