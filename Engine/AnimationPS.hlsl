


struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

struct PointLight
{
	float4 Color;
	float3 Position;
};


cbuffer externalData : register(b0)
{
	DirectionalLight light;
	DirectionalLight secondaryLight;
	PointLight pointLight;
	float3 cameraPosition;
	bool UseNormal;
}

/*
cbuffer externalData : register(b0)
{
	DirectionalLight light1;
};
*/

struct VertexToPixel
{

	float4 position		: SV_POSITION;
	float3 normal       : NORMAL;
	float2 uv			: TEXCOORD;
	float3 worldPos		: POSITION;
	float3 tangent		: TANGENT;
};

Texture2D diffuseTexture : register(t0);
Texture2D normalTexture : register(t1);
SamplerState basicSampler : register(s0);

float4 calculateDirectionalLight(float3 normal, DirectionalLight light)
{
	float3 dirToLight = normalize(-light.Direction);
	float NdotL = dot(normal, dirToLight);
	NdotL = saturate(NdotL);
	return light.DiffuseColor * NdotL + light.AmbientColor;
}

float calculateSpecular(float3 normal, float3 worldPos, float3 dirToPointLight, float3 camPos)
{
	float3 dirToCamera = normalize(camPos - worldPos);
	float3 refl = reflect(-dirToPointLight, normal);
	float specExp = 64;
	float spec = pow(saturate(dot(dirToCamera, refl)), specExp);
	return spec;
}

float4 calculatePointLight(float3 normal, float3 worldPos, PointLight light)
{
	float3 dirToPointLight = normalize(light.Position - worldPos);
	float pointNdotL = dot(normal, dirToPointLight);
	pointNdotL = saturate(pointNdotL);
	float spec = calculateSpecular(normal, worldPos, dirToPointLight, cameraPosition);
	return spec + light.Color * pointNdotL;
}

float3 calculateNormalFromMap(float2 uv, float3 normal, float3 tangent)
{
	float3 normalFromTexture = normalTexture.Sample(basicSampler, uv).xyz;
	float3 unpackedNormal = normalFromTexture * 2.0f - 1.0f;
	float3 N = normal;
	float3 T = normalize(tangent - N * dot(tangent, N));
	float3 B = cross(N, T);
	float3x3 TBN = float3x3(T, B, N);
	return normalize(mul(unpackedNormal, TBN));
}


float4 main(VertexToPixel input) : SV_TARGET
{
	/*
	input.normal = normalize(input.normal);

	//Light Calculations
	float3 lightDir = -normalize(light.Direction);
	float dirNdotL1 = dot(input.normal, lightDir);
	dirNdotL1 = saturate(dirNdotL1);

	return float4((dirNdotL1*light.DiffuseColor)  + light.AmbientColor);
	*/
	
	
	float4 surfaceColor = diffuseTexture.Sample(basicSampler, input.uv);
	float3 finalNormal = calculateNormalFromMap(input.uv, input.normal, input.tangent);
	input.normal = normalize(input.normal);
	finalNormal = normalize(finalNormal);
	float4 dirLight = calculateDirectionalLight(finalNormal, light) * surfaceColor;
	float4 secDirLight = calculateDirectionalLight(finalNormal, secondaryLight) * surfaceColor;
	float4 pLight = calculatePointLight(finalNormal, input.worldPos, pointLight)  * surfaceColor;
	return dirLight + secDirLight + pLight;
	
}