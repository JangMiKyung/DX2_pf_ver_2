//
// ���� ����
//
float4x4	matWVP;			//���� �� ���� 3����ü ���
float		tiling0;		//0�� Ÿ�� Ÿ�ϼ�
float		tiling1;		//1�� Ÿ�� Ÿ�ϼ�
float		tiling2;		//2�� Ÿ�� Ÿ�ϼ�
float		tiling3;		//3�� Ÿ�� Ÿ�ϼ�


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
	Output.Position = mul(float4(Input.Position, 1.0f), matWVP);

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
	//�� Ÿ���� ������ �̴´�.
	float3 tileColor0 = tex2D(SampTile0, In.Texcoord * tiling0).rgb;
	float3 tileColor1 = tex2D(SampTile1, In.Texcoord * tiling1).rgb;
	float3 tileColor2 = tex2D(SampTile2, In.Texcoord * tiling2).rgb;
	float3 tileColor3 = tex2D(SampTile3, In.Texcoord * tiling3).rgb;

	//ControlTexture �÷�
	float3 maskColor = tex2D(SampControl, In.Texcoord);

	//���÷� ���� ��� �Ǵ�?
	float r = maskColor.r;
	float g = maskColor.g;
	float b = maskColor.b;

	//black �� ������ ��ŭ ���?
	float black = saturate(1.0f - (r + g + b));

	float3 finalColor = tileColor0 * black + tileColor1 * r + tileColor2 * g + tileColor3 * b;

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

