//
//�������� ���ش�
//

float4x4 matWorld; //���� ���
float4x4 matView;  //view ���
float4x4 matProjection; //�������
float4   vColor;

//
//���� ���̴����� �ȼ� ���̴��� �Ѱ��ִ� ������ ����ü
//
struct VS_OUTPUT
{
	float4 Position : POSITION; //��� �������� ��ȯ������ ������ �Ѱܾ��Ѵ�.(pixel �Լ����� �� ����)
	float4 Color : COLOR0;		
};

//
//���� ���̴� �Լ�
//

VS_OUTPUT vs_main(float3 Position : POSITION)
{
	VS_OUTPUT Output;

	//���� ���� ��ȯ
	Output.Position = mul(float4(Position, 1.0), matWorld);

	//���� �� ��ȯ
	Output.Position = mul(Output.Position, matView);

	//���� ���� ��ȯ
	Output.Position = mul(Output.Position, matProjection);

	//�÷� ����
	Output.Color = vColor;

	return Output;
}

//
//�ȼ� ���̴� �Լ�
//
//���� ���̴����� ���ϵ� ������ �Ű������� ���´�.
//�̶� �ȼ� ������ ���� �Ǿ� ������
//POSITION ���� ���������� �ȴ�.

float4 ps_main(VS_OUTPUT In) : COLOR0
{
	//�׳� �ƹ��������� ���� In�� ���� �÷� ����
	return In.Color;
}


//
//��ũ��
//

technique MyShader
{
	pass P0
	{
		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
	}
}

