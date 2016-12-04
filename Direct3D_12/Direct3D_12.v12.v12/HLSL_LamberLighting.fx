//
// ���� ����
//
float4x4	matWorld;		//���� ���
float4x4	matView;		//�� ���
float4x4	matProjection;	//���� ���
float4		vColor;			//��� �÷�
float3		vLightDir;		//������ ���� ( ����ȭ �Ǿ� ���Դ� )
float4x4	matInvWorld;	//������ �����

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

	//���� ���� ��ȯ
	Output.Position = mul(float4(Position, 1.0), matWorld);

	//���� �� ��ȯ
	Output.Position = mul(Output.Position, matView);

	//���� ���� ��ȯ
	Output.Position = mul(Output.Position, matProjection);

	//���� �븻
	//float3 worldNormal = mul( Normal, (float3x3)matWorld );

	//���� �븻 ����ȭ
	//worldNormal = normalize( worldNormal );

	//�ڽ��� ��ġ�� ���� ������ ����
	//float3 toLight = -vLightDir;

	//����Ʈ
	//float diffuse = dot( toLight, worldNormal );


	//���� �븻
	float3 localNormal = Normal;

		//���� ������ ������ ���÷� �����
		float3 localLightDir = mul(vLightDir, (float3x3)matInvWorld);

		//���� ���� ���� ����ȭ
		localLightDir = normalize(localLightDir);

	float3 toLocalLight = -localLightDir;

		//����Ʈ ����
		float diffuse = dot(toLocalLight, localNormal);

	//0~1 ���̷� ©���ִ� �Լ� Clamp01 �� �������
	diffuse = saturate(diffuse);

	//�÷� ����
	Output.Color = vColor * diffuse;


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

