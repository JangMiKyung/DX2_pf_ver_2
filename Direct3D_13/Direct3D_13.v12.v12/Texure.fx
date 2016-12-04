//
// ���� ����
//
float4x4	matWVP;			//���� �� ���� 3����ü ���
float4		vColor;			//��� �÷�
float		tilingX;		//���� Ÿ�ϼ�
float		tilingY;		//���� Ÿ�ϼ�

//
// Texture �� �޴´�.
//
texture MyTex;

//
// MyTex �� SamplingFilter �� ����
//
sampler2D MyTexSamp = sampler_state
{
	Texture = MyTex;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};





//
// �������� �Ѱܹ��� ������ ����ü
//
struct VS_INPUT
{
	float3 Position : POSITION;			//������ġ
	float2 Texcoord : TEXCOORD0;		//����UV
};


//
// ���������Ϳ��� �ȼ����̴��� �Ѱ��ִ� ������ ����ü 
//
struct VS_OUTPUT
{
	float4 Position : POSITION;		//���� �������� ��ȯ������ ������ �Ѱܾ� �Ѵ�. ( Pixel �Լ����� �Ȱ��� )
	float2 Texcoord : TEXCOORD0;	//������ UV ��ǥ
};


//
// ���� ���̴� �Լ�
//

//�������� ���� ������ �Ű������� ���´�
VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	//���� ��ü ��ȯ
	Output.Position = mul(float4(Input.Position, 1.0), matWVP);

	//UV ����
	Output.Texcoord = Input.Texcoord;


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
	//Texture UV �� �÷���...
	//return float4( In.Texcoord.x, In.Texcoord.y, 0, 1 );

	//UV ��ǥ ������ Sampler2D ���� �÷��� ����
	//�ش�UV �� Sampling �� �÷� = tex2D( Sampler2D ����, float2 UV ���� )
	//float4 texColor = tex2D( MyTexSamp, In.Texcoord );

	//sampling �� UV ���� Ÿ�� ������ ���Ѵ�.
	float2 uv = In.Texcoord;
	uv.x *= tilingX;
	uv.y *= tilingY;

	//���̴����� �Ǽ������� ������ ������ �ȴ�.
	uv.x = uv.x % 1.0f;
	uv.y = uv.y % 1.0f;


	float4 texColor = tex2D(MyTexSamp, uv);





		//���ԵǴ� vector3 �� bgr �� �����ϸ� b �� (r)x �� g �� (g)y �� r �� (b)z 
		//float3 finalColor = texColor.bgr;

		float3 finalColor = texColor.rgb;

		/*
		//������ ������ ���� xyzw rgba �ε� �����ִ�.
		finalColor.r = texColor.g;
		finalColor.g = texColor.g;
		finalColor.b = texColor.g;
		*/

		//finalColor.x = finalColor.x * vColor.x;
		//finalColor.y = finalColor.y * vColor.y;
		//finalColor.z = finalColor.z * vColor.z;
		//���̴����� ���� ������ ���ͳ����� ���� ����

		finalColor = finalColor * vColor.rgb;

	return float4(finalColor, 1);
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
