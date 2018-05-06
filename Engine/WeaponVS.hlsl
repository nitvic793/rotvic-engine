cbuffer externalData : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
	matrix characterWorld;
};

struct Bones
{
	matrix BoneTransform;
	matrix InvBoneTransform;
};


cbuffer bone : register(b1)
{
	Bones bone;
	Bones bone2;
	float blendWeight;
}

struct VertexShaderInput
{
	float3 position		: POSITION;     // XYZ position
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 tangent		: TANGENT;
};


struct VertexToPixel
{
	float4 position		: SV_POSITION;	// XYZW position (System Value Position)
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
	float3 worldPos		: POSITION;
	float3 tangent		: TANGENT;
};

VertexToPixel main(VertexShaderInput input)
{
	// Set up output struct
	VertexToPixel output;
	matrix worldViewProj = mul(mul(characterWorld, view), projection);


	matrix bonetransform = 0;

	bonetransform = mul(bone.BoneTransform, bone.InvBoneTransform) * blendWeight;
	bonetransform += mul(bone2.BoneTransform, bone2.InvBoneTransform) * (1-blendWeight);
	
	output.position = mul(mul(bonetransform, float4(input.position, 1.0f)), worldViewProj);
	output.normal = mul(mul(bonetransform, input.normal), (float3x3)world);
	output.worldPos = mul(input.position, world).xyz;
	output.uv = input.uv;
	output.tangent = normalize(mul(mul(bonetransform, input.tangent), (float3x3)world));
	
	/*
	output.position = mul(float4(input.position, 1.0f), worldViewProj);
	output.worldPos = mul(float4(input.position, 1.0f), world).xyz;
	output.normal = mul(input.normal, (float3x3)world);
	output.normal = normalize(output.normal);
	output.uv = input.uv;
	output.tangent = normalize(mul(input.tangent, (float3x3)world));
	*/
	return output;
}