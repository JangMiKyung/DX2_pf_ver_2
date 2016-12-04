
//
// ��������
//
float4x4 matWVP;			//World * View * Projection Matrix
float4x4 matWorld;			//World Matrix
float3 vLightDir;			//���� ����
float3 vEyePos;				//���� ��ġ
float specPower;					//Specular �Ŀ�

//�⺻ DiffuseTexture
texture DiffuseTex;
sampler2D DiffuseSamp = sampler_state
{
	Texture = DiffuseTex;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	MIPFILTER = LINEAR;
};


//���� �Է� ����ü
struct VS_INPUT
{
	float4 Position : POSITION;
	float2 Texcoord : TEXCOORD0;
	float3 Normal : NORMAL0;
};


//���� ��� ����ü
struct VS_OUTPUT{
	float4 Position : POSITION;
	float2 Texcoord : TEXCOORD0;
	float3 ViewDir : TEXCOORD1;			//�������� ���� �ü��� ����
	float3 LightDir : TEXCOORD2;		//�������� ���� �������� ����
	float3 Normal : TEXCOORD3;			//���� ��ȯ�̵� ������ �븻
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Ouput = (VS_OUTPUT)0;		//ZeroMemory �� ���� ȿ��

	//���� ��ȯ
	Ouput.Position = mul(Input.Position, matWVP);

	//Texcoord 
	Ouput.Texcoord = Input.Texcoord;

	//���� �븻
	Ouput.Normal = mul(Input.Normal, (float3x3)matWorld);

	//���� 
	Ouput.LightDir = -vLightDir;

	//���� ������ġ
	float3 worldPos = mul(Input.Position, matWorld).xyz;

		//���� ������ �ü���ġ
		Ouput.ViewDir = vEyePos - worldPos;

	return Ouput;
}

//�ȼ����̴� �Է� ����ü
struct PS_INPUT
{
	float2 Texcoord : TEXCOORD0;
	float3 ViewDir : TEXCOORD1;			//�������� ���� �ü��� ����
	float3 LightDir : TEXCOORD2;		//�������� ���� �������� ����
	float3 Normal : TEXCOORD3;			//���� ��ȯ�̵� ������ �븻
};


float4 ps_main(PS_INPUT Input) : COLOR0{

	//�ϴ� ��Ȯ�� ����� ���� Normalize
	float3 viewDir = normalize(Input.ViewDir);
	float3 lightDir = normalize(Input.LightDir);
	float3 normal = normalize(Input.Normal);

	//Ȯ�걤�� ��
	float d = dot(normal, lightDir);
	float diffuse = saturate(d);

	//Light Reflect
	float3 reflect = -lightDir + 2.0f * d * normal;
		reflect = normalize(reflect);

	//Specular 
	float spec = saturate(dot(reflect, viewDir));

	//SpecularPower ����
	spec = pow(spec, specPower);


	//Texture �÷�
	float3 diffuseTex = tex2D(DiffuseSamp, Input.Texcoord).rgb;

		//DiffuseColor
		float3 diffuseColor = diffuseTex * diffuse;

		//SpecularColor
		float3 specularColor = float3(1, 1, 1) * spec;


		//FinalColor 
		float3 finalColor = diffuseColor + specularColor;


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



