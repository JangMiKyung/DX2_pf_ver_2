

// 라이트 행렬
// Pos.x,         Pos.y,          Pos.z,         lightType ( 0 Direction, 1 Point, 2 Spot )
// Dir.x,         Dir.y,          Dir.z,         DistancePow;
// Color.r,       Color.g,        Color.b,       strength
// MaxRange,      MinRange,       InAngle        OutAngle 
float4x4 matLights[10];
int LightNum = 0;
float fSpecPower;

void ComputeLight(
	out float3 addDiffuse,			//계산되서 나가는 DeffuseColor
	out float3 addSpecular,			//계산되서 나가는 SpecularColor
	float3 position,				//위치 값
	float3 normal,					//노말
	float3 viewDir,					//시선 방향
	int lightIndex					//처리되는 라이트 인덱스
	)
{
	float4x4 matLight = matLights[lightIndex];

		float3 lightColor = float3(matLight._31, matLight._32, matLight._33) * matLight._34;

		//Direction 처리
	if (matLight._14 == 0.0f)
	{
		//광원의 방향
		float3 dir = float3(matLight._21, matLight._22, matLight._23);
			float3 lightDir = -dir;

			float NdotL = dot(lightDir, normal);
		float diff = saturate(NdotL);

		//라이트 반사
		float3 lightRefl = normalize(dir + 2.0f * NdotL * normal);
			float spec = saturate(dot(lightRefl, viewDir));
		spec = pow(spec, fSpecPower);

		addDiffuse = lightColor * diff;
		addSpecular = lightColor * spec * diff;
	}

	//Point Light and Spot 처리
	else
	{
		float3 lightPosition = float3(matLight._11, matLight._12, matLight._13);
			float maxRange = matLight._41;
		float minRange = matLight._42;
		float DistancePow = matLight._24;

		//광원의 방향
		float3 lightDir = lightPosition - position;

		//광원의 거리
		float dist = length(lightDir);

		//정규화
		lightDir *= (1.0f / dist);

		//광원 량....
		float NdotL = dot(lightDir, normal);
		float diff = saturate(NdotL);

		//거리에 따른 감쇠율
		float t = saturate((dist - minRange) / (maxRange - minRange));
		t = 1.0f - pow(t, DistancePow);

		//만약 Spot 이라면....
		diff *= t;			//받는 광원 량에 감쇠율 적용

		//라이트 반사
		float3 lightRefl = normalize(-lightDir + 2.0f * NdotL * normal);
			float spec = saturate(dot(lightRefl, viewDir));
		spec = pow(spec, fSpecPower);

		addDiffuse = lightColor * diff;
		addSpecular = lightColor * spec * diff;		//Spec 은 Diff 의 영향을 받는다.
	}
}


void ComputeLightToon(
	out float3 addDiffuse,			//계산되서 나가는 DeffuseColor
	float3 position,				//위치 값
	float3 normal,					//노말
	int lightIndex					//처리되는 라이트 인덱스
	)
{
	float4x4 matLight = matLights[lightIndex];

		float3 lightColor = float3(matLight._31, matLight._32, matLight._33) * matLight._34;

		//Direction 처리
	if (matLight._14 == 0.0f)
	{
		//광원의 방향
		float3 dir = float3(matLight._21, matLight._22, matLight._23);
			float3 lightDir = -dir;

			float NdotL = dot(lightDir, normal);
		float diff = saturate(NdotL);

		//광원의 적용량을 단계 별로 나눈다.
		float interval = 1.0 / 4.0f;		//5단계
		float toon = floor(diff / interval) * interval;		//floor 가 소숫점재낀다..

		addDiffuse = lightColor * toon;
	}

	//Point Light and Spot 처리
	else
	{
		float3 lightPosition = float3(matLight._11, matLight._12, matLight._13);
			float maxRange = matLight._41;
		float minRange = matLight._42;
		float DistancePow = matLight._24;

		//광원의 방향
		float3 lightDir = lightPosition - position;

			//광원의 거리
			float dist = length(lightDir);

		//정규화
		lightDir *= (1.0f / dist);

		//광원 량....
		float NdotL = dot(lightDir, normal);
		float diff = saturate(NdotL);

		//거리에 따른 감쇠율
		float t = saturate((dist - minRange) / (maxRange - minRange));
		t = 1.0f - pow(t, DistancePow);

		//만약 Spot 이라면....
		diff *= t;			//받는 광원 량에 감쇠율 적용

		//광원의 적용량을 단계 별로 나눈다.
		float interval = 1.0 / 4.0f;		//5단계
		float toon = floor(diff / interval) * interval;		//floor 가 소숫점재낀다..


		addDiffuse = lightColor * toon;

	}
}
