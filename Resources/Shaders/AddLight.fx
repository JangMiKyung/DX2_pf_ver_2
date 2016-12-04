

// ����Ʈ ���
// Pos.x,         Pos.y,          Pos.z,         lightType ( 0 Direction, 1 Point, 2 Spot )
// Dir.x,         Dir.y,          Dir.z,         DistancePow;
// Color.r,       Color.g,        Color.b,       strength
// MaxRange,      MinRange,       InAngle        OutAngle 
float4x4 matLights[10];
int LightNum = 0;
float fSpecPower;

void ComputeLight(
	out float3 addDiffuse,			//���Ǽ� ������ DeffuseColor
	out float3 addSpecular,			//���Ǽ� ������ SpecularColor
	float3 position,				//��ġ ��
	float3 normal,					//�븻
	float3 viewDir,					//�ü� ����
	int lightIndex					//ó���Ǵ� ����Ʈ �ε���
	)
{
	float4x4 matLight = matLights[lightIndex];

		float3 lightColor = float3(matLight._31, matLight._32, matLight._33) * matLight._34;

		//Direction ó��
	if (matLight._14 == 0.0f)
	{
		//������ ����
		float3 dir = float3(matLight._21, matLight._22, matLight._23);
			float3 lightDir = -dir;

			float NdotL = dot(lightDir, normal);
		float diff = saturate(NdotL);

		//����Ʈ �ݻ�
		float3 lightRefl = normalize(dir + 2.0f * NdotL * normal);
			float spec = saturate(dot(lightRefl, viewDir));
		spec = pow(spec, fSpecPower);

		addDiffuse = lightColor * diff;
		addSpecular = lightColor * spec * diff;
	}

	//Point Light and Spot ó��
	else
	{
		float3 lightPosition = float3(matLight._11, matLight._12, matLight._13);
			float maxRange = matLight._41;
		float minRange = matLight._42;
		float DistancePow = matLight._24;

		//������ ����
		float3 lightDir = lightPosition - position;

		//������ �Ÿ�
		float dist = length(lightDir);

		//����ȭ
		lightDir *= (1.0f / dist);

		//���� ��....
		float NdotL = dot(lightDir, normal);
		float diff = saturate(NdotL);

		//�Ÿ��� ���� ������
		float t = saturate((dist - minRange) / (maxRange - minRange));
		t = 1.0f - pow(t, DistancePow);

		//���� Spot �̶��....
		diff *= t;			//�޴� ���� ���� ������ ����

		//����Ʈ �ݻ�
		float3 lightRefl = normalize(-lightDir + 2.0f * NdotL * normal);
			float spec = saturate(dot(lightRefl, viewDir));
		spec = pow(spec, fSpecPower);

		addDiffuse = lightColor * diff;
		addSpecular = lightColor * spec * diff;		//Spec �� Diff �� ������ �޴´�.
	}
}


void ComputeLightToon(
	out float3 addDiffuse,			//���Ǽ� ������ DeffuseColor
	float3 position,				//��ġ ��
	float3 normal,					//�븻
	int lightIndex					//ó���Ǵ� ����Ʈ �ε���
	)
{
	float4x4 matLight = matLights[lightIndex];

		float3 lightColor = float3(matLight._31, matLight._32, matLight._33) * matLight._34;

		//Direction ó��
	if (matLight._14 == 0.0f)
	{
		//������ ����
		float3 dir = float3(matLight._21, matLight._22, matLight._23);
			float3 lightDir = -dir;

			float NdotL = dot(lightDir, normal);
		float diff = saturate(NdotL);

		//������ ���뷮�� �ܰ� ���� ������.
		float interval = 1.0 / 4.0f;		//5�ܰ�
		float toon = floor(diff / interval) * interval;		//floor �� �Ҽ����糤��..

		addDiffuse = lightColor * toon;
	}

	//Point Light and Spot ó��
	else
	{
		float3 lightPosition = float3(matLight._11, matLight._12, matLight._13);
			float maxRange = matLight._41;
		float minRange = matLight._42;
		float DistancePow = matLight._24;

		//������ ����
		float3 lightDir = lightPosition - position;

			//������ �Ÿ�
			float dist = length(lightDir);

		//����ȭ
		lightDir *= (1.0f / dist);

		//���� ��....
		float NdotL = dot(lightDir, normal);
		float diff = saturate(NdotL);

		//�Ÿ��� ���� ������
		float t = saturate((dist - minRange) / (maxRange - minRange));
		t = 1.0f - pow(t, DistancePow);

		//���� Spot �̶��....
		diff *= t;			//�޴� ���� ���� ������ ����

		//������ ���뷮�� �ܰ� ���� ������.
		float interval = 1.0 / 4.0f;		//5�ܰ�
		float toon = floor(diff / interval) * interval;		//floor �� �Ҽ����糤��..


		addDiffuse = lightColor * toon;

	}
}
